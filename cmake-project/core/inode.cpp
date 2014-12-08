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

InodeId Inode::getId() {
    return id;
}

Permissions *Inode::getPermissions()
{
    return permissions;
}

void Inode::clear()
{
    sizeInBlocks = 0;
    hardLinkCount = 0;
    lastBlockByteCount = 0;
}

//void Inode::clearForDebug()
//{
//    sizeInBlocks = 0;
//    hardLinkCount = 0;
//    lastBlockByteCount = 0;
//}

void Inode::addLink()
{
    hardLinkCount++;
}

void Inode::removeLink()
{
    if (hardLinkCount == 0)
        throw UnlinkOnFreeNodeException();
    hardLinkCount--;
}

uint8_t Inode::getHardLinkCount()
{
    return hardLinkCount;
}

bool Inode::isFree()
{
    return hardLinkCount == 0;
}

Inode::Inode(InodeSize size, FileType fileType, InodeId id)
{

    if (size < MIN_SIZE || (size % BLOCK_ADRESS_SIZE) != 0) {
        throw WrongINodeSizeException();
    }
    this->id = id;
    permissions = new Permissions(fileType, 7, 7, 7);
    clear();
    dataBlocksPerNode = (size - FIRST_DATA_BLOCK_OFFSET) / BLOCK_ADRESS_SIZE;
    dataBlocks = new uint64_t[dataBlocksPerNode];
}

Inode::Inode(InodeSize size, char *buffer, unsigned indexInBlock, InodeId id)
    : Inode(size, FileType::NONE, id)
{
    readFromBuffer(buffer, indexInBlock*size);
}

Inode::~Inode()
{
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

void Inode::readFromBuffer(char *buffer, unsigned inBufferOffset)
{
    RawPermissions rawPermissions =
            RawDataUtils::readUintFromBuffer(buffer, inBufferOffset + TYPE_AND_PERMISSIONS_OFFSET, Permissions::getRawDataSize());
    permissions = new Permissions(rawPermissions);

    hardLinkCount = RawDataUtils::readUintFromBuffer(buffer, inBufferOffset + HARD_LINK_COUNT_OFFSET, sizeof(hardLinkCount));
    sizeInBlocks = RawDataUtils::readUintFromBuffer(buffer, inBufferOffset + BLOCK_COUNT_OFFSET, sizeof(sizeInBlocks));
    lastBlockByteCount = RawDataUtils::readUintFromBuffer(buffer, inBufferOffset + LAST_BLOCK_BYTE_COUNT_OFFSET, sizeof(lastBlockByteCount));

    for (unsigned i = 0; i < dataBlocksPerNode; ++i) {
        unsigned offset = getDataBlockOffset(i);
        dataBlocks[i] = RawDataUtils::readUintFromBuffer(buffer, inBufferOffset + offset, BLOCK_ADRESS_SIZE);
    }
}

void Inode::writeToBuffer(char *buffer, unsigned inBufferOffset)
{
    RawDataUtils::writeUintToBuffer(permissions->getRawData(), buffer, inBufferOffset + TYPE_AND_PERMISSIONS_OFFSET,
                                    Permissions::getRawDataSize());
    RawDataUtils::writeUintToBuffer(hardLinkCount, buffer, inBufferOffset + HARD_LINK_COUNT_OFFSET, sizeof(hardLinkCount));
    RawDataUtils::writeUintToBuffer(sizeInBlocks, buffer, inBufferOffset + BLOCK_COUNT_OFFSET, sizeof(sizeInBlocks));
    RawDataUtils::writeUintToBuffer(lastBlockByteCount, buffer, inBufferOffset + LAST_BLOCK_BYTE_COUNT_OFFSET, sizeof(lastBlockByteCount));

    for (unsigned i = 0; i < dataBlocksPerNode; ++i) {
        unsigned offset = getDataBlockOffset(i);
        RawDataUtils::writeUintToBuffer(dataBlocks[i], buffer, inBufferOffset + offset, BLOCK_ADRESS_SIZE);
    }
}
