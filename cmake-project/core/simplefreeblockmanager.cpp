#include "simplefreeblockmanager.h"

SimpleFreeBlockManager::SimpleFreeBlockManager(PartitionHeader *header) {
    this->header = header;
    this->nextFreeBlock = 50;
}

BlockId SimpleFreeBlockManager::getFreeBlock()
{
//    nextFreeBlock += 1;
    return nextFreeBlock++;
}

BlockCount SimpleFreeBlockManager::freeBlockCount()
{
    return header->getBlockCount() - nextFreeBlock;
}

bool SimpleFreeBlockManager::isFull()
{
    if (freeBlockCount() == 0)
        return true;
    return false;
}
