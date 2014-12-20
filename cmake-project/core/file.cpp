#include "file.h"
#include "log.h"
#include <cstring>

File::File(std::string name, Inode *inode, FSPartition *fsPartition)
{
    this->name = name;
    this->inode = inode;
    this->fsPartition = fsPartition;
    this->fileReadBuffer = nullptr;
}

File::File()
{
    this->name = "";
    this->inode = 0;
    this->fsPartition = nullptr;
    this->fileReadBuffer = nullptr;
}

File::~File()
{
    if (fileReadBuffer != nullptr)
        delete fileReadBuffer;
}

uint64_t File::read(char *buffer, uint64_t length)
{
    if (fileReadBuffer == nullptr)
        fileReadBuffer = new FileReadBuffer(this, fsPartition->getBlockSize());
    return fileReadBuffer->read(buffer, length);
}

uint64_t File::read(char *buffer, uint64_t length, uint64_t offset)
{
    if (fileReadBuffer == nullptr)
        fileReadBuffer = new FileReadBuffer(this, fsPartition->getBlockSize());
    return fileReadBuffer->read(buffer, length, offset);
}

BlockSize File::readBlock(BlockCount index, char *buffer)
{
    if (index >= inode->getSizeInBlocks())
        throw BlockIndexOutOfRangeException();
    fsPartition->readDataBlock(inode->getDataBlockNumber(index), buffer);

    if (index == inode->getSizeInBlocks() - 1)
        return inode->getLastBlockByteCount();
    return fsPartition->getBlockSize();
}

void File::writeBlock(BlockCount index, char *buffer)
{
    if (index == inode->getSizeInBlocks())
        appendByBlock(buffer, fsPartition->getBlockSize());
    else
        rewriteBlock(index, buffer);
}

void File::rewriteBlock(BlockCount index, char *buffer)
{
    if (index > inode->getSizeInBlocks())
        throw BlockIndexOutOfRangeException();
    BlockId blockId = inode->getDataBlockNumber(index);
    fsPartition->writeDataBlock(blockId, buffer);
}

void File::appendByBlock(const char *buffer, BlockSize significantBytes)
{
    if (significantBytes > fsPartition->getBlockSize())
        throw TooManyDataForBlockException();
    if (significantBytes == 0)
        throw NoneDataForBlockException();

    BlockId newBlockId = fsPartition->getFreeBlock();
    inode->addDataBlock(newBlockId);
    fsPartition->writeDataBlock(newBlockId, buffer);
    inode->setLastBlockByteCount(significantBytes);
}

void File::append(const char *buffer, uint64_t significantBytes)
{
    BlockSize bytesWritten = appendLastBlock(buffer, significantBytes);
    significantBytes -= bytesWritten;
    buffer += bytesWritten;
    while (significantBytes > fsPartition->getBlockSize()) {
        Log::stream << "append significantBytes: " << significantBytes << std::endl;
        appendByBlock(buffer, fsPartition->getBlockSize());
        significantBytes -= fsPartition->getBlockSize();
        buffer += fsPartition->getBlockSize();
    }
    if (significantBytes > 0)
        appendByBlock(buffer, significantBytes);
    Log::stream << "append (end) significantBytes: " << significantBytes << std::endl;
}

///
/// \brief File::appendLastBlock
/// \param buffer
/// \param significantBytes
/// \return bytes written
///
BlockSize File::appendLastBlock(const char *buffer, uint64_t significantBytes) {
    if (inode->getLastBlockByteCount() >= fsPartition->getBlockSize())
        return 0;
    if (inode->getSizeInBlocks() == 0)
        return 0;

    char lastBlock[fsPartition->getBlockSize()];
    readBlock(inode->getSizeInBlocks() - 1, lastBlock);

    BlockSize freeSpace = fsPartition->getBlockSize() - inode->getLastBlockByteCount();
    BlockSize bytesToWrite = freeSpace < significantBytes ? freeSpace : significantBytes;

    for (unsigned i = 0; i < bytesToWrite; ++i) {
        lastBlock[inode->getLastBlockByteCount() + i] = buffer[i];
    }

    inode->setLastBlockByteCount(inode->getLastBlockByteCount() + bytesToWrite);
    rewriteBlock(inode->getSizeInBlocks() - 1, lastBlock);
    return bytesToWrite;
}

void File::write(std::ifstream &fileStream)
{
    char buffer[fsPartition->getBlockSize()];

    fileStream.read(buffer, fsPartition->getBlockSize());
    while (fileStream.gcount() == fsPartition->getBlockSize()) {
        appendByBlock(buffer, fsPartition->getBlockSize());
        fileStream.read(buffer, fsPartition->getBlockSize());
    }
    appendByBlock(buffer, fileStream.gcount());
}

uint64_t File::write(const char *buffer, uint64_t length, uint64_t offset)
{
    Log::stream << "File size: " << getTotalSizeInBytes() << " requested offset: " << offset << std::endl;
    if (getTotalSizeInBytes() == offset) {
        append(buffer, length);

        return length;
    }
    throw NotImplementedYetException();
}

void File::get(std::ofstream &fileStream)
{
    char buffer[fsPartition->getBlockSize()];

    for (BlockCount i = 0; i < inode->getSizeInBlocks() - 1; ++i) {
        readBlock(i, buffer);
        fileStream.write(buffer, fsPartition->getBlockSize());
    }
    readBlock(inode->getSizeInBlocks() - 1, buffer);
    fileStream.write(buffer, inode->getLastBlockByteCount());

}

Inode *File::getInode()
{
    return inode;
}

std::string &File::getName()
{
    return name;
}

uint64_t File::getTotalSizeInBytes()
{
    uint64_t fullBlocksSize = (inode->getSizeInBlocks()-1) * fsPartition->getBlockSize();
    return fullBlocksSize + inode->getLastBlockByteCount();
}

void File::appendWithZeroes(uint64_t size)
{
    char buffer[fsPartition->getBlockSize()];
    memset(buffer, 0, fsPartition->getBlockSize());
    while (size > 0) {
        BlockSize tmp = size < fsPartition->getBlockSize() ? size : fsPartition->getBlockSize();
        append(buffer, tmp);
        size -= tmp;
    }
}

void File::cutToSize(uint64_t size)
{
    throw NotImplementedYetException();
}

void File::truncate(uint64_t size)
{
    auto currentSize = getTotalSizeInBytes();
    if (size > currentSize)
        appendWithZeroes(size);
    else
        cutToSize(size);
}


bool File::isDirectory()
{
    if (inode->getPermissions()->getFileType() == FileType::DIRECTORY)
        return true;
    return false;
}

FileReadBuffer::FileReadBuffer(File *file, BlockSize blockSize)
{
    Log::stream << "FileReadBuffer constructor" << std::endl;
    this->file = file;
    this->blockSize = blockSize;
    this->innerBuffer = new char[blockSize];
    this->nextBlock = 0;
    this->currentPositionInBuffer = 0;
    loadNextBlock();
}

FileReadBuffer::~FileReadBuffer()
{
    Log::stream << "FileReadBuffer destructor" << std::endl;
    delete [] innerBuffer;
}

uint64_t FileReadBuffer::read(char *buffer, uint64_t length)
{
    unsigned i;
    for (i = 0; i < length; ++i) {
        int byte = nextByte();
        if (byte != END_OF_FILE)
            buffer[i] = byte;
        else
            break;
    }
    return i;
}

uint64_t FileReadBuffer::read(char *buffer, uint64_t length, uint64_t offset)
{
    loadBlock(offset / blockSize);
    currentPositionInBuffer = offset % blockSize;
    return read(buffer, length);
}

int FileReadBuffer::nextByte()
{
    if (currentPositionInBuffer >= currentBlockSize)
        loadNextBlock();
    if (currentPositionInBuffer == END_OF_FILE)
        return END_OF_FILE;
    return innerBuffer[currentPositionInBuffer++];
}

void FileReadBuffer::loadBlock(BlockCount blockId) {
    nextBlock = blockId;
    loadNextBlock();
}

void FileReadBuffer::loadNextBlock() {
    try {
        currentBlockSize = file->readBlock(nextBlock++, innerBuffer);
        currentPositionInBuffer = 0;
    }
    catch (BlockIndexOutOfRangeException e) {
        currentPositionInBuffer = END_OF_FILE;
    }
}
