#include "inode.h"
#include "rawdatautils.h"

BlockSize Inode::getLastBlockByteCount() const
{
    return lastBlockByteCount;
}

void Inode::setLastBlockByteCount(const BlockSize &value)
{
    lastBlockByteCount = value;
}

BlockCount Inode::getSizeInBlocks()
{
    return sizeInBlocks;
}

InodeId Inode::getIndex() {
    return index;
}

void Inode::clearForDebug()
{
    sizeInBlocks = 0;
    hardLinkCount = 0;
    lastBlockByteCount = 0;
}

Inode::Inode(InodeSize size)
{

    if (size < MIN_SIZE || (size % BLOCK_ADRESS_SIZE) != 0) {
        throw WrongINodeSizeException();
    }
    this->size = size;
    permissions = nullptr;
    clearForDebug();
    dataBlocksPerNode = (size - FIRST_DATA_BLOCK_OFFSET) / BLOCK_ADRESS_SIZE;
    dataBlocks = new uint64_t[dataBlocksPerNode];
}

Inode::~Inode()
{
    if (permissions != nullptr)
        delete permissions;
    delete [] dataBlocks;
}

unsigned Inode::getDataBlockOffset(unsigned blockNumber)
{
    return FIRST_DATA_BLOCK_OFFSET + blockNumber*BLOCK_ADRESS_SIZE;
}

void Inode::addDataBlock(BlockId blockNumber)
{
    if (sizeInBlocks == dataBlocksPerNode) {
        throw MaximumFileSizeAchievedException();
    }
    dataBlocks[sizeInBlocks] = blockNumber;
    sizeInBlocks++;
}

BlockId Inode::getDataBlockNumber(BlockCount sequenceNumber)
{
    return dataBlocks[sequenceNumber];
}

void Inode::readFromBuffer(char *buffer, unsigned position)
{
    RawPermissions rawPermissions =
            RawDataUtils::readUintFromBuffer(buffer, position + TYPE_AND_PERMISSIONS_OFFSET, Permissions::getRawDataSize());
    permissions = new Permissions(rawPermissions);

    hardLinkCount = RawDataUtils::readUintFromBuffer(buffer, position + HARD_LINK_COUNT_OFFSET, sizeof(hardLinkCount));
    sizeInBlocks = RawDataUtils::readUintFromBuffer(buffer, position + BLOCK_COUNT_OFFSET, sizeof(sizeInBlocks));
    lastBlockByteCount = RawDataUtils::readUintFromBuffer(buffer, position + LAST_BLOCK_BYTE_COUNT_OFFSET, sizeof(lastBlockByteCount));

    for (unsigned i = 0; i < dataBlocksPerNode; ++i) {
        unsigned offset = getDataBlockOffset(i);
        dataBlocks[i] = RawDataUtils::readUintFromBuffer(buffer, position + offset, BLOCK_ADRESS_SIZE);
    }
}

void Inode::writeToBuffer(char *buffer, unsigned position)
{
    RawDataUtils::writeUintToBuffer(permissions->getRawData(), buffer, position + TYPE_AND_PERMISSIONS_OFFSET,
                                    Permissions::getRawDataSize());
    RawDataUtils::writeUintToBuffer(hardLinkCount, buffer, position + HARD_LINK_COUNT_OFFSET, sizeof(hardLinkCount));
    RawDataUtils::writeUintToBuffer(sizeInBlocks, buffer, position + BLOCK_COUNT_OFFSET, sizeof(sizeInBlocks));
    RawDataUtils::writeUintToBuffer(lastBlockByteCount, buffer, position + LAST_BLOCK_BYTE_COUNT_OFFSET, sizeof(lastBlockByteCount));

    for (unsigned i = 0; i < dataBlocksPerNode; ++i) {
        unsigned offset = getDataBlockOffset(i);
        RawDataUtils::writeUintToBuffer(dataBlocks[i], buffer, position + offset, BLOCK_ADRESS_SIZE);
    }
}
