#include "simplefreeblockmanager.h"

SimpleFreeBlockManager::SimpleFreeBlockManager(PartitionHeader *header) {
    this->header = header;
    this->nextFreeBlock = 300;
}

uint64_t SimpleFreeBlockManager::getFreeBlock()
{
    return nextFreeBlock++;
}

uint64_t SimpleFreeBlockManager::freeBlockCount()
{
    return header->getBlockNumber() - nextFreeBlock;
}

bool SimpleFreeBlockManager::isFull()
{
    if (getFreeBlock() == 0)
        return true;
    return false;
}
