#ifndef LISTFREEBLOCKMANAGER_H
#define LISTFREEBLOCKMANAGER_H

#include "ifreeblockmanager.h"
#include "fspartition.h"

class ListFreeBlockManager : public IFreeBlockManager
{
private:
    class ControlBlock {
    private:
        BlockId blockId;
    public:
        BlockCount freeBlockCount;
        BlockId currentIndirectBlockId;
        BlockCount nextFreeBlockPosition;
        ControlBlock(FSPartition *partition);
        void flush(FSPartition *partition);
    };

    FSPartition *partition;
    ControlBlock controlBlock;
    IIndirectBlock *currentIndirectBlock = nullptr;

    BlockCount calculateAvailableBlocks(BlockId firstAvailableBlock);
    BlockCount calculateListBlockCount();
    void calculateInitialFreeBlockPosition(BlockCount blocksToStore);
    void createList(BlockId inclusiveFrom);
    void initializeControlBlock(BlockId firstAvailableBlock);
    void reloadCurrentIndirectBlock();
public:
    ListFreeBlockManager(FSPartition *partition);
    ~ListFreeBlockManager() override;
    BlockId getFreeBlock() override;
    BlockCount freeBlockCount() override;
    bool isFull() override;
    void flush() override;
    void onPartitionCreation() override;
    void markBlockAsFree(BlockId id) override;
};

#endif // LISTFREEBLOCKMANAGER_H
