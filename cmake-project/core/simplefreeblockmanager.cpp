#include "simplefreeblockmanager.h"

SimpleFreeBlockManager::SimpleFreeBlockManager(PartitionHeader *header) {
    this->header = header;
    this->nextFreeBlock = 300;
}

BlockId SimpleFreeBlockManager::getFreeBlock()
{
    nextFreeBlock += 5;
    return nextFreeBlock++;
}

BlockCount SimpleFreeBlockManager::freeBlockCount()
{
    return header->getBlockCount() - nextFreeBlock;
}

bool SimpleFreeBlockManager::isFull()
{
    if (getFreeBlock() == 0)
        return true;
    return false;
}
