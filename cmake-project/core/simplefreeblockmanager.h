#ifndef SIMPLEFREEBLOCKMANAGER_H
#define SIMPLEFREEBLOCKMANAGER_H

#include "ifreeblockmanager.h"
#include "fspartition.h"

class SimpleFreeBlockManager : public IFreeBlockManager {
private:
    FSPartition *partition;
    BlockId nextFreeBlock;
    char *rawData;
    static const unsigned FIRST_FREE_BLOCK = 50;
public:
    SimpleFreeBlockManager(FSPartition *partition);
    ~SimpleFreeBlockManager() override;
    BlockId getFreeBlock() override;
    BlockCount freeBlockCount() override;
    bool isFull() override;
    void flush() override;
    void initialize() override;
    void markBlockAsFree(BlockId id) override;
    inline void flushImpl();
};

#endif // SIMPLEFREEBLOCKMANAGER_H
