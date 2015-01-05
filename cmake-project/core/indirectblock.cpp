#include "indirectblock.h"
#include "rawdatautils.h"
#include "log.h"

IndirectBlock::IndirectBlock(FSPartition *partition, BlockId blockId)
{
    this->blockId = blockId;
    this->partition = partition;
    this->innerBuffer = new char[partition->getBlockSize()];
    partition->readDataBlock(blockId, innerBuffer);
}

void IndirectBlock::flushImpl()
{
    partition->writeDataBlock(blockId, innerBuffer);
    Log::stream << "IndirectBlock flush, writing block " << blockId << std::endl;
}

IndirectBlock::~IndirectBlock()
{
    flushImpl();
    delete [] innerBuffer;
}

BlockCount IndirectBlock::getMaxSize()
{
    return getMaxSize(partition);
}

unsigned long long IndirectBlock::getBlockIdWithoutException(BlockCount position)
{
    auto result = RawDataUtils::readUintFromBuffer(innerBuffer, position * sizeof(BlockId), sizeof(BlockId));
    Log::stream << "\tIndirectBlock " << blockId << ", position: " << position << " value: " << result << std::endl;
    return result;
}

BlockId IndirectBlock::getBlockId(BlockCount position)
{
    if (position >= getMaxSize())
        throw PositionBiggerThanMaxSizeException();
    return getBlockIdWithoutException(position);
}

void IndirectBlock::setBlockIdWithoutException(BlockId id, BlockCount position)
{
    Log::stream << "Setting block id: " << id << " on position " << position << "in indirect block no " << blockId << std::endl;
    RawDataUtils::writeUintToBuffer(id, innerBuffer, position * sizeof(BlockId), sizeof(BlockId));
}

void IndirectBlock::setBlockId(BlockCount position, BlockId id)
{
    if (position >= getMaxSize())
        throw PositionBiggerThanMaxSizeException();
    setBlockIdWithoutException(id, position);
}

BlockId IndirectBlock::getNextIndirectBlockId()
{
    return getBlockIdWithoutException(getMaxSize());
}

void IndirectBlock::setNextIndirectBlockId(BlockId id)
{
    setBlockIdWithoutException(id, getMaxSize());
}

BlockCount IndirectBlock::getMaxSize(FSPartition *partition)
{
    return (partition->getBlockSize() / sizeof(BlockId)) - 1;
}
