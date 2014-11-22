#ifndef SIMPLEFREEBLOCKMANAGER_H
#define SIMPLEFREEBLOCKMANAGER_H

#include "ifreeblockmanager.h"
#include "partitionheader.h"

class SimpleFreeBlockManager : public IFreeBlockManager {
private:
    PartitionHeader *header;
    BlockId nextFreeBlock;
public:
    SimpleFreeBlockManager(PartitionHeader *header);
    BlockId getFreeBlock();
    BlockCount freeBlockCount();
    bool isFull();
};

#endif // SIMPLEFREEBLOCKMANAGER_H
