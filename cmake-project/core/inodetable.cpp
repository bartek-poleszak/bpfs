#include "inodetable.h"

void InodeTable::calculateSize()
{
    this->size = (inodeCount * inodeSize) / blockSize;
}

InodeTable::InodeTable(InodeCount inodeCount, InodeSize inodeSize, BlockSize blockSize)
    : tmp(inodeSize)
{
    if (blockSize % inodeSize != 0) {
        throw WrongInodeSizeException();
    }
    if ((inodeSize * inodeCount) % blockSize != 0) {
        throw WrongInodeCountException();
    }
    this->inodeCount = inodeCount;
    this->inodeSize = inodeSize;
    this->blockSize = blockSize;
    calculateSize();
}

InodeTable::InodeTable(IDisk &disk, PartitionHeader *header)
    : tmp(inodeSize)
{
    char buffer[blockSize];
    disk.readBlock(INODE_TABLE_BEGIN_BLOCK, buffer);
    tmp.readFromBuffer(buffer, 0);

    this->inodeCount = header->getInodeCount();
    this->inodeSize = header->getInodeSize();
    this->blockSize = header->getBlockSize();
    calculateSize();
}

void InodeTable::writeAllToDisk(IDisk &disk)
{
    char buffer[blockSize];
    tmp.writeToBuffer(buffer, 0);
    disk.writeBlock(INODE_TABLE_BEGIN_BLOCK, buffer);
}

INode *InodeTable::getInode(InodeId inodeId)
{
    return &tmp;
}

BlockCount InodeTable::getTableSize()
{
    return size;
}
