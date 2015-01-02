#include "simplefreeblockmanager.h"
#include "rawdatautils.h"
#include "log.h"

void SimpleFreeBlockManager::flush()
{
    RawDataUtils::writeUintToBuffer(nextFreeBlock, rawData, 0, sizeof(nextFreeBlock));
    partition->writeDataBlock(FIRST_FREE_BLOCK, rawData);
}

void SimpleFreeBlockManager::initialize()
{
    nextFreeBlock = FIRST_FREE_BLOCK;
    nextFreeBlock++;
    flush();
}

void SimpleFreeBlockManager::markBlockAsFree(BlockId id)
{
    Log::stream << "Marking blocks as free unimplemented" << std::endl;
}

SimpleFreeBlockManager::SimpleFreeBlockManager(FSPartition *partition)
{
    this->partition = partition;
    rawData = new char[partition->getBlockSize()];
    partition->readDataBlock(FIRST_FREE_BLOCK, rawData);
    nextFreeBlock = RawDataUtils::readUintFromBuffer(rawData, 0, sizeof(nextFreeBlock));
}

SimpleFreeBlockManager::~SimpleFreeBlockManager()
{
    delete [] rawData;
}

BlockId SimpleFreeBlockManager::getFreeBlock()
{
//    nextFreeBlock += 1;
    return nextFreeBlock++;
}

BlockCount SimpleFreeBlockManager::freeBlockCount()
{
    return partition->getHeader()->getBlockCount() - nextFreeBlock;
}

bool SimpleFreeBlockManager::isFull()
{
    if (freeBlockCount() == 0)
        return true;
    return false;
}
