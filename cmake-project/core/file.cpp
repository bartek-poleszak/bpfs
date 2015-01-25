#include <cstring>
#include "file.h"
#include "log.h"
#include "iindirectblock.h"
#include "indirectblock.h"
#include <cassert>

File::File(std::string name, Inode *inode, FSPartition *fsPartition)
{
    this->name = name;
    this->inode = inode;
    this->fsPartition = fsPartition;
    this->fileReadBuffer = nullptr;
    this->fileWriteBuffer = nullptr;
    this->blockIdCache = nullptr;
}

File::File()
{
    this->name = "";
    this->inode = nullptr;
    this->fsPartition = nullptr;
    this->fileReadBuffer = nullptr;
    this->blockIdCache = nullptr;
}

File::~File()
{
    if (fileReadBuffer != nullptr)
        delete fileReadBuffer;
    if (fileWriteBuffer != nullptr)
        delete fileWriteBuffer;
    if (blockIdCache != nullptr)
        delete blockIdCache;
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

void File::initializeBlockIdCacheIfNeeded()
{
    if (blockIdCache == nullptr)
        blockIdCache = new BlockIdCache(fsPartition, inode);
}

void File::unlink()
{
//    assert(inode->getHardLinkCount() != 0);
    if (inode->getHardLinkCount() > 0) {
        inode->removeLink();
        if (inode->getHardLinkCount() == 0)
            markBlocksAsFree();
    }
    else
        Log::stream << "Cannot unlink - hard link count is already 0" << std::endl;
}

void File::markBlocksAsFree()
{
    initializeBlockIdCacheIfNeeded();
    for (BlockCount i = 0; i < inode->getSizeInBlocks(); i++)
        fsPartition->markBlockAsFree(blockIdCache->getBlockId(i));
}

BlockSize File::readBlock(BlockCount index, char *buffer)
{
    if (index >= inode->getSizeInBlocks())
        throw BlockIndexOutOfRangeException();
    initializeBlockIdCacheIfNeeded();
    fsPartition->readDataBlock(blockIdCache->getBlockId(index), buffer);
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
    initializeBlockIdCacheIfNeeded();
    BlockId blockId = blockIdCache->getBlockId(index);
    fsPartition->writeDataBlock(blockId, buffer);
}

void File::appendByBlock(const char *buffer, BlockSize significantBytes)
{
    if (significantBytes > fsPartition->getBlockSize())
        throw TooManyDataForBlockException();
    if (significantBytes == 0)
        throw NoneDataForBlockException();

//    IIndirectBlock *indirectBlock = blockIdCache->getIndirectBlock(0);
    BlockId newBlockId = fsPartition->getFreeBlock();
    initializeBlockIdCacheIfNeeded();
    blockIdCache->addBlock(newBlockId);
//    indirectBlock->setBlockId(inode->getSizeInBlocks(), newBlockId);
//    inode->setSizeInBlocks(inode->getSizeInBlocks() + 1);
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
    if (fileStream.gcount() > 0)
        appendByBlock(buffer, fileStream.gcount());
}

uint64_t File::rewrite(const char *buffer, uint64_t length, uint64_t offset)
{
    if (fileWriteBuffer == nullptr)
        fileWriteBuffer = new FileWriteBuffer(this, fsPartition->getBlockSize());
    return fileWriteBuffer->write(buffer, length, offset);
}

uint64_t File::write(const char *buffer, uint64_t length, uint64_t offset)
{
    Log::stream << "File size: " << getTotalSizeInBytes() << " requested offset: " << offset << std::endl;
    if (getTotalSizeInBytes() == offset) {
        append(buffer, length);

        return length;
    }
    return rewrite(buffer, length, offset);
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
    if (inode->getSizeInBlocks() == 0)
        return 0;
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

BlockSize File::cutByBlock()
{
    BlockSize result = inode->getLastBlockByteCount();
    BlockId lastBlock = blockIdCache->getBlockId(inode->getSizeInBlocks() - 1);
    inode->setSizeInBlocks(inode->getSizeInBlocks() - 1);
    fsPartition->markBlockAsFree(lastBlock);
    if (inode->getSizeInBlocks() > 0)
        inode->setLastBlockByteCount(fsPartition->getBlockSize());
    else
        inode->setLastBlockByteCount(0);
    return result;
}

uint64_t File::cutFromLastBlock(uint64_t bytesToCut) {
    if (bytesToCut < inode->getLastBlockByteCount()) {
        inode->setLastBlockByteCount(inode->getLastBlockByteCount() - bytesToCut);
        return bytesToCut;
    }
    else if (bytesToCut == 0)
        return 0;
    return cutByBlock();
}

void File::cutToSize(uint64_t requestedSize)
{
    auto currentSize = getTotalSizeInBytes();
    uint64_t bytesToCut = currentSize - requestedSize;
    if (currentSize < requestedSize)
        throw FileCutException();
    bytesToCut -= cutFromLastBlock(bytesToCut);
    while (bytesToCut > fsPartition->getBlockSize())
        bytesToCut -= cutByBlock();
    cutFromLastBlock(bytesToCut);
}

void File::truncate(uint64_t size)
{
    initializeBlockIdCacheIfNeeded();
    auto currentSize = getTotalSizeInBytes();
    if (size > currentSize)
        appendWithZeroes(size - currentSize);
    else if (size == currentSize)
        return;
    else
        cutToSize(size);
}


bool File::isDirectory()
{
    if (inode->getPermissions()->getFileType() == FileType::DIRECTORY)
        return true;
    return false;
}

FileBuffer::FileBuffer(File *file, BlockSize blockSize)
{
//    Log::stream << "FileBuffer constructor" << std::endl;
    this->file = file;
    this->blockSize = blockSize;
    this->innerBuffer = new char[blockSize];
    this->nextBlock = 0;
//    this->currentBlock = INVALID_POSITION;
    this->currentPositionInBuffer = 0;
    loadNextBlock();
}

FileBuffer::~FileBuffer()
{
//    Log::stream << "FileBuffer destructor" << std::endl;
    delete [] innerBuffer;
}

void FileReadBuffer::onByteRecieved(BytePointer bytePtr, const char *positionInBuffer)
{
    *const_cast<char*>(positionInBuffer) = *bytePtr;
}

uint64_t FileReadBuffer::read(char *buffer, uint64_t length)
{
    return iterateStream(buffer, length);
}

uint64_t FileReadBuffer::read(char *buffer, uint64_t length, uint64_t offset)
{
    return iterateStream(buffer, length, offset);
}

BytePointer FileBuffer::nextByte()
{
    if (currentPositionInBuffer >= currentBlockSize)
        loadNextBlock();
    if (currentPositionInBuffer == END_OF_FILE)
        return nullptr;
    return innerBuffer + currentPositionInBuffer++;
}

void FileBuffer::loadBlock(BlockCount blockId) {
    nextBlock = blockId;
    loadNextBlock();
}

uint64_t FileBuffer::iterateStream(const char *buffer, uint64_t length)
{
    unsigned i;
    for (i = 0; i < length; ++i) {
        BytePointer bytePtr = nextByte();
        if (bytePtr != nullptr)
            onByteRecieved(bytePtr, buffer + i);
        else
            break;
    }
//    onBlockManipulationCompleted(innerBuffer, currentBlock);
    return i;
}

uint64_t FileBuffer::iterateStream(const char *buffer, uint64_t length, uint64_t offset)
{
    BlockCount block = offset / blockSize;
    loadBlock(block);
    currentPositionInBuffer = offset % blockSize;
    return iterateStream(buffer, length);
}

File *FileBuffer::getFile()
{
    return file;
}

void FileBuffer::loadNextBlock() {
    try {
//        if (currentBlock != nextBlock) {
//            if (currentBlock != FileBuffer::INVALID_POSITION)
//                onBlockManipulationCompleted(innerBuffer, currentBlock);
//            currentBlock = nextBlock;
            currentBlockSize = file->readBlock(nextBlock++, innerBuffer);
            currentPositionInBuffer = 0;
//        }
    }
    catch (BlockIndexOutOfRangeException e) {
        currentPositionInBuffer = END_OF_FILE;
    }
}


void BlockIdCache::cacheBlocksTo(unsigned index)
{
    while (!isCached(index)) {
        IIndirectBlock *lastCached = cache.at(cache.size() - 1);
        cache.push_back(new IndirectBlock(partition, lastCached->getNextIndirectBlockId()));
    }
}

bool BlockIdCache::isCached(unsigned index)
{
    return index < cache.size();
}

unsigned BlockIdCache::calculateIndex(BlockCount offset)
{
    IIndirectBlock *inodeInnerIndirectBlock = getIndirectBlock(0);
    if (offset < inodeInnerIndirectBlock->getMaxSize())
        return 0;
    offset -= inodeInnerIndirectBlock->getMaxSize();
    return 1 + (offset / IndirectBlock::getMaxSize(partition));
}

BlockCount BlockIdCache::calculatePosition(BlockCount offset)
{
    Log::stream << "calculate position offset: " << offset << std::endl;
    IIndirectBlock *inodeInnerIndirectBlock = getIndirectBlock(0);
    if (offset < inodeInnerIndirectBlock->getMaxSize())
        return offset;
    offset -= inodeInnerIndirectBlock->getMaxSize();
    return offset % IndirectBlock::getMaxSize(partition);
}

BlockIdCache::BlockIdCache(FSPartition *partition, Inode *inode)
{
    this->partition = partition;
    this->inode = inode;
    cache.push_back(&inode->getDataBlockCollection());
}

BlockIdCache::~BlockIdCache()
{
    // i = 1, bo nie usuwam wewnetrznego IndirectBlocku w i-wezle,
    // robi to destruktor i-wezla
    for (unsigned i = 1; i < cache.size(); i++)
        delete cache[i];
}

BlockId BlockIdCache::getBlockId(BlockCount offset)
{
    assert(offset < inode->getSizeInBlocks());
    auto index = calculateIndex(offset);
    IIndirectBlock *indirectBlock = getIndirectBlock(index);
    Log::stream << "getBlockId offset: " << offset << " indirectBlock index: " << index << std::endl;
    return indirectBlock->getBlockId(calculatePosition(offset));
}

void BlockIdCache::initializeNewIndirectBlock(BlockId blockId, IIndirectBlock *lastIndirectBlock)
{
    BlockId newIndirectBlockId = partition->getFreeBlock();
    Log::stream << "New indirect block: " << newIndirectBlockId << std::endl;
    lastIndirectBlock->setNextIndirectBlockId(newIndirectBlockId);
    lastIndirectBlock = getIndirectBlock(calculateIndex(inode->getSizeInBlocks()));
    lastIndirectBlock->setBlockId(0, blockId);
}

void BlockIdCache::addBlock(BlockId blockId)
{
    IIndirectBlock *lastIndirectBlock;
    int lastBlockIdPosition;
    if (inode->getSizeInBlocks() > 0) {
        lastIndirectBlock = getIndirectBlock(calculateIndex(inode->getSizeInBlocks()-1));
        lastBlockIdPosition = calculatePosition(inode->getSizeInBlocks()-1);
    }
    else {
        lastIndirectBlock = getIndirectBlock(0);
        lastBlockIdPosition = -1;
    }
    if (lastBlockIdPosition != -1 && (unsigned)lastBlockIdPosition >= lastIndirectBlock->getMaxSize()-1)
        initializeNewIndirectBlock(blockId, lastIndirectBlock);
    else
        lastIndirectBlock->setBlockId(lastBlockIdPosition + 1, blockId);
    inode->setSizeInBlocks(inode->getSizeInBlocks() + 1);
}

IIndirectBlock *BlockIdCache::getIndirectBlock(unsigned index)
{
    if (!isCached(index))
        cacheBlocksTo(index);
    return cache.at(index);
}


void FileWriteBuffer::onByteRecieved(BytePointer bytePtr, const char *positionInBuffer)
{
    *bytePtr = *positionInBuffer;
}

void FileWriteBuffer::onBlockManipulationCompleted(char *innerBuffer, BlockCount blockSequenceNumber)
{
    getFile()->writeBlock(blockSequenceNumber, innerBuffer);
}

uint64_t FileWriteBuffer::write(char *buffer, uint64_t length)
{
    return iterateStream(buffer, length);
}

uint64_t FileWriteBuffer::write(const char *buffer, uint64_t length, uint64_t offset)
{
//    assert(offset == getFile()->getTotalSizeInBytes());
    return iterateStream(buffer, length, offset);
}
