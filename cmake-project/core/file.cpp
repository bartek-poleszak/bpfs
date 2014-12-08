#include "file.h"

File::File(std::string name, Inode *inode, FSPartition *fsPartition)
{
    this->name = name;
    this->inode = inode;
    this->fsPartition = fsPartition;
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

void File::appendByBlock(char *buffer, BlockSize significantBytes)
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

void File::append(char *buffer, uint64_t significantBytes)
{
    BlockSize bytesWritten = appendLastBlock(buffer, significantBytes);
    significantBytes -= bytesWritten;
    buffer += bytesWritten;
    while (significantBytes > fsPartition->getBlockSize()) {
        appendByBlock(buffer, fsPartition->getBlockSize());
        significantBytes -= bytesWritten;
        buffer += bytesWritten;
    }
    if (significantBytes > 0)
        appendByBlock(buffer, significantBytes);
}

///
/// \brief File::appendLastBlock
/// \param buffer
/// \param significantBytes
/// \return bytes written
///
BlockSize File::appendLastBlock(char *buffer, uint64_t significantBytes) {
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


bool File::isDirectory()
{
    if (inode->getPermissions()->getFileType() == FileType::DIRECTORY)
        return true;
    return false;
}

FileReadBuffer::FileReadBuffer(File *file, BlockSize blockSize)
{
    this->file = file;
    this->blockSize = blockSize;
    this->innerBuffer = new char[blockSize];
    this->currentBlock = 0;
    this->currentPositionInBuffer = 0;
    loadNextBlock();
}

FileReadBuffer::~FileReadBuffer()
{
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

int FileReadBuffer::nextByte()
{
    if (currentPositionInBuffer >= currentBlockSize)
        loadNextBlock();
    if (currentPositionInBuffer == END_OF_FILE)
        return END_OF_FILE;
    return innerBuffer[currentPositionInBuffer++];
}

void FileReadBuffer::loadNextBlock() {
    try {
        currentBlockSize = file->readBlock(currentBlock++, innerBuffer);
        currentPositionInBuffer = 0;
    }
    catch (BlockIndexOutOfRangeException e) {
        currentPositionInBuffer = END_OF_FILE;
    }
}
