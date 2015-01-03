#include "inode.h"
#include "rawdatautils.h"
#include "log.h"

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

void Inode::setSizeInBlocks(BlockCount size)
{
    this->sizeInBlocks = size;
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
    : dataBlockCollection( ((size - FIRST_DATA_BLOCK_OFFSET) / BLOCK_ADRESS_SIZE) - 1 )
{
    Log::stream << "Inode constructor, id: " << id << std::endl;
    if (size < MIN_SIZE || (size % BLOCK_ADRESS_SIZE) != 0) {
        throw WrongINodeSizeException();
    }
    this->id = id;
    permissions = new Permissions(fileType, 7, 7, 7);
    clear();
}

Inode::Inode(InodeSize size, char *buffer, unsigned indexInBlock, InodeId id)
    : Inode(size, FileType::NONE, id)
{
    readFromBuffer(buffer, indexInBlock*size);
}

Inode::~Inode()
{
    delete permissions;
}

unsigned Inode::getDataBlockOffset(unsigned blockNumber)
{
    return FIRST_DATA_BLOCK_OFFSET + blockNumber*BLOCK_ADRESS_SIZE;
}

IIndirectBlock &Inode::getDataBlockCollection()
{
    return dataBlockCollection;
}

void Inode::readFromBuffer(char *buffer, unsigned inBufferOffset)
{
    RawPermissions rawPermissions =
            RawDataUtils::readUintFromBuffer(buffer, inBufferOffset + TYPE_AND_PERMISSIONS_OFFSET, Permissions::getRawDataSize());
    permissions = new Permissions(rawPermissions);

    hardLinkCount = RawDataUtils::readUintFromBuffer(buffer, inBufferOffset + HARD_LINK_COUNT_OFFSET, sizeof(hardLinkCount));
    sizeInBlocks = RawDataUtils::readUintFromBuffer(buffer, inBufferOffset + BLOCK_COUNT_OFFSET, sizeof(sizeInBlocks));
    lastBlockByteCount = RawDataUtils::readUintFromBuffer(buffer, inBufferOffset + LAST_BLOCK_BYTE_COUNT_OFFSET, sizeof(lastBlockByteCount));

    unsigned i;
    for (i = 0; i < dataBlockCollection.getMaxSize(); ++i) {
        unsigned offset = getDataBlockOffset(i);
        BlockId blockId = RawDataUtils::readUintFromBuffer(buffer, inBufferOffset + offset, BLOCK_ADRESS_SIZE);
        dataBlockCollection.setBlockId(i, blockId);
    }
    unsigned offset = getDataBlockOffset(i);
    BlockId blockId = RawDataUtils::readUintFromBuffer(buffer, inBufferOffset + offset, BLOCK_ADRESS_SIZE);
    dataBlockCollection.setNextIndirectBlockId(blockId);
}

void Inode::writeToBuffer(char *buffer, unsigned inBufferOffset)
{
    RawDataUtils::writeUintToBuffer(permissions->getRawData(), buffer, inBufferOffset + TYPE_AND_PERMISSIONS_OFFSET,
                                    Permissions::getRawDataSize());
    RawDataUtils::writeUintToBuffer(hardLinkCount, buffer, inBufferOffset + HARD_LINK_COUNT_OFFSET, sizeof(hardLinkCount));
    RawDataUtils::writeUintToBuffer(sizeInBlocks, buffer, inBufferOffset + BLOCK_COUNT_OFFSET, sizeof(sizeInBlocks));
    RawDataUtils::writeUintToBuffer(lastBlockByteCount, buffer, inBufferOffset + LAST_BLOCK_BYTE_COUNT_OFFSET, sizeof(lastBlockByteCount));

    unsigned i;
    for (i = 0; i < dataBlockCollection.getMaxSize(); ++i) {
        unsigned offset = getDataBlockOffset(i);
        RawDataUtils::writeUintToBuffer(dataBlockCollection.getBlockId(i), buffer, inBufferOffset + offset, BLOCK_ADRESS_SIZE);
    }
    unsigned offset = getDataBlockOffset(i);
    RawDataUtils::writeUintToBuffer(dataBlockCollection.getNextIndirectBlockId(), buffer, inBufferOffset + offset, BLOCK_ADRESS_SIZE);
}

InternalDataBlockCollection::InternalDataBlockCollection(unsigned dataBlocksPerNode)
{
    this->dataBlocksPerNode = dataBlocksPerNode;
    this->dataBlocks = new BlockId[dataBlocksPerNode];
}

InternalDataBlockCollection::~InternalDataBlockCollection()
{
    delete [] dataBlocks;
}

BlockCount InternalDataBlockCollection::getMaxSize()
{
    return dataBlocksPerNode;
}

BlockId InternalDataBlockCollection::getBlockId(BlockCount position)
{
    if (position >= dataBlocksPerNode)
        throw MaximumBlockCountInInodeException();
    Log::stream << "\tIn inode, position: " << position << " value: " << dataBlocks[position] << std::endl;
    return dataBlocks[position];
}

void InternalDataBlockCollection::setBlockId(BlockCount position, BlockId id)
{
    if (position >= dataBlocksPerNode)
        throw MaximumBlockCountInInodeException();
    dataBlocks[position] = id;
}

BlockId InternalDataBlockCollection::getNextIndirectBlockId()
{
    return nextIndirectBlock;
}

void InternalDataBlockCollection::setNextIndirectBlockId(BlockId id)
{
    nextIndirectBlock = id;
}
