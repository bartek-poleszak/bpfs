#ifndef IDATABLOCKPOINTERCOLLECTION_H
#define IDATABLOCKPOINTERCOLLECTION_H

#include "bpfstypes.h"

class IIndirectBlock
{
public:
    virtual ~IIndirectBlock();
    virtual BlockCount getMaxSize() = 0;
    virtual BlockId getBlockId(BlockCount position) = 0;
    virtual void setBlockId(BlockCount position, BlockId id) = 0;
    virtual BlockId getNextIndirectBlockId() = 0;
    virtual void setNextIndirectBlockId(BlockId id) = 0;
};

#endif // IDATABLOCKPOINTERCOLLECTION_H
