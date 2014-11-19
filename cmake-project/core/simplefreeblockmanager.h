#ifndef SIMPLEFREEBLOCKMANAGER_H
#define SIMPLEFREEBLOCKMANAGER_H

#include "ifreeblockmanager.h"
#include "partitionheader.h"

class SimpleFreeBlockManager : public IFreeBlockManager {
private:
    PartitionHeader *header;
    uint64_t nextFreeBlock;
public:
    SimpleFreeBlockManager(PartitionHeader *header);
    uint64_t getFreeBlock();
    uint64_t freeBlockCount();
    bool isFull();
};

#endif // SIMPLEFREEBLOCKMANAGER_H
