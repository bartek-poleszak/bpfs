#include "inodetable.h"

void InodeTable::calculateSizeAndNodesPerBlock()
{
    this->size = (inodeCount * inodeSize) / blockSize;
    this->nodesPerBlock = blockSize / inodeSize;
}

InodeTable::InodeTable(IDisk &disk, InodeCount inodeCount, InodeSize inodeSize, BlockSize blockSize)
{
    if (blockSize % inodeSize != 0) {
        throw WrongInodeSizeException();
    }
    if ((inodeSize * inodeCount) % blockSize != 0) {
        throw WrongInodeCountException();
    }
    this->disk = &disk;

    this->inodeCount = inodeCount;
    this->inodeSize = inodeSize;
    this->blockSize = blockSize;
    calculateSizeAndNodesPerBlock();
}

InodeTable::InodeTable(IDisk &disk, PartitionHeader *header)
{
    this->disk = &disk;

    this->inodeCount = header->getInodeCount();
    this->inodeSize = header->getInodeSize();
    this->blockSize = header->getBlockSize();

    char buffer[blockSize];
    disk.readBlock(INODE_TABLE_BEGIN_BLOCK, buffer);
    calculateSizeAndNodesPerBlock();
}

InodeTable::~InodeTable()
{
    for (auto &idInodePair : cachedInodes)
        delete idInodePair.second;
}

void InodeTable::writeNodeToDisk(Inode *inode) {
    char buffer[blockSize];
    unsigned whichBlock = INODE_TABLE_BEGIN_BLOCK + (inode->getId() / nodesPerBlock);
    unsigned indexInBlock = inode->getId() % nodesPerBlock;
    disk->readBlock(whichBlock, buffer);
    inode->writeToBuffer(buffer, indexInBlock*inodeSize);
    disk->writeBlock(whichBlock, buffer);
}

void InodeTable::writeAllToDisk()
{
    for (auto &idInodePair : cachedInodes)
        writeNodeToDisk(idInodePair.second);
}

Inode *InodeTable::getInodeFromMemory(InodeId inodeId)
{
    unsigned whichBlock = INODE_TABLE_BEGIN_BLOCK + (inodeId / nodesPerBlock);
    unsigned indexInBlock = inodeId % nodesPerBlock;
    char buffer[blockSize];
    disk->readBlock(whichBlock, buffer);
    return new Inode(inodeSize, buffer, indexInBlock, inodeId);
}

Inode *InodeTable::getInode(InodeId inodeId)
{
    if (cachedInodes[inodeId] == nullptr)
        cachedInodes[inodeId] = getInodeFromMemory(inodeId);
    return cachedInodes[inodeId];
}

BlockCount InodeTable::getTableSize()
{
    return size;
}


