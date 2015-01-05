#ifndef INDIRECTBLOCK_H
#define INDIRECTBLOCK_H

#include <exception>
#include "iindirectblock.h"
#include "fspartition.h"

class PositionBiggerThanMaxSizeException : public std::exception {};

class IndirectBlock : public IIndirectBlock
{
private:
    BlockId blockId;
    FSPartition *partition;
    char *innerBuffer;
    void setBlockIdWithoutException(BlockId id, BlockCount position);
    unsigned long long getBlockIdWithoutException(BlockCount position);
public:
    IndirectBlock(FSPartition *partition, BlockId blockId);
    ~IndirectBlock() override;
    BlockCount getMaxSize() override;
    BlockId getBlockId(BlockCount position) override;
    void setBlockId(BlockCount position, BlockId id) override;
    BlockId getNextIndirectBlockId() override;
    void setNextIndirectBlockId(BlockId id) override;
    static BlockCount getMaxSize(FSPartition *partition);

    void flushImpl();
};

#endif // INDIRECTBLOCK_H
