#include "inode.h"
#include "rawdatautils.h"


BlockSize INode::getLastBlockByteCount() const
{
    return lastBlockByteCount;
}

void INode::setLastBlockByteCount(const BlockSize &value)
{
    lastBlockByteCount = value;
}

BlockCount INode::getBlockCount()
{
    return blockCount;
}

INode::INode(InodeSize size)
{
    permissions = nullptr;
    blockCount = 0;
    hardLinkCount = 0;
    lastBlockByteCount = 0;
    if (size < MIN_SIZE || (size % BLOCK_ADRESS_SIZE) != 0) {
        throw WrongINodeSizeException();
    }
    this->size = size;
    dataBlocksInNode = (size - FIRST_DATA_BLOCK_OFFSET) / BLOCK_ADRESS_SIZE;
    dataBlocks = new uint64_t[dataBlocksInNode];
}

INode::~INode()
{
    if (permissions != nullptr)
        delete permissions;
    delete [] dataBlocks;
}

unsigned INode::getDataBlockOffset(unsigned blockNumber)
{
    return FIRST_DATA_BLOCK_OFFSET + blockNumber*BLOCK_ADRESS_SIZE;
}

void INode::addDataBlock(BlockId blockNumber)
{
    if (blockCount == dataBlocksInNode) {
        throw MaximumFileSizeAchievedException();
    }
    dataBlocks[blockCount] = blockNumber;
    blockCount++;
}

BlockId INode::getDataBlockNumber(BlockCount sequenceNumber)
{
    return dataBlocks[sequenceNumber];
}

void INode::readFromBuffer(char *buffer, unsigned position)
{
    RawPermissions rawPermissions =
            RawDataUtils::readUintFromBuffer(buffer, position + TYPE_AND_PERMISSIONS_OFFSET, Permissions::getRawDataSize());
    permissions = new Permissions(rawPermissions);

    hardLinkCount = RawDataUtils::readUintFromBuffer(buffer, position + HARD_LINK_COUNT_OFFSET, sizeof(hardLinkCount));
    blockCount = RawDataUtils::readUintFromBuffer(buffer, position + BLOCK_COUNT_OFFSET, sizeof(blockCount));
    lastBlockByteCount = RawDataUtils::readUintFromBuffer(buffer, position + LAST_BLOCK_BYTE_COUNT_OFFSET, sizeof(lastBlockByteCount));

    for (unsigned i = 0; i < dataBlocksInNode; ++i) {
        unsigned offset = getDataBlockOffset(i);
        dataBlocks[i] = RawDataUtils::readUintFromBuffer(buffer, position + offset, BLOCK_ADRESS_SIZE);
    }
}

void INode::writeToBuffer(char *buffer, unsigned position)
{
    RawDataUtils::writeUintToBuffer(permissions->getRawData(), buffer, position + TYPE_AND_PERMISSIONS_OFFSET,
                                    Permissions::getRawDataSize());
    RawDataUtils::writeUintToBuffer(hardLinkCount, buffer, position + HARD_LINK_COUNT_OFFSET, sizeof(hardLinkCount));
    RawDataUtils::writeUintToBuffer(blockCount, buffer, position + BLOCK_COUNT_OFFSET, sizeof(blockCount));
    RawDataUtils::writeUintToBuffer(lastBlockByteCount, buffer, position + LAST_BLOCK_BYTE_COUNT_OFFSET, sizeof(lastBlockByteCount));

    for (unsigned i = 0; i < dataBlocksInNode; ++i) {
        unsigned offset = getDataBlockOffset(i);
        RawDataUtils::writeUintToBuffer(dataBlocks[i], buffer, position + offset, BLOCK_ADRESS_SIZE);
    }
}
