#include "inode.h"
#include "rawdatautils.h"

INode::INode(unsigned size)
{
    permissions = nullptr;
    if (size < MIN_SIZE || (size % BLOCK_ADRESS_SIZE) != 0) {
        throw WrongINodeSizeException();
    }
    this->size = size;
    this->dataBlockCount = (size - FIRST_DATA_BLOCK_OFFSET) / BLOCK_ADRESS_SIZE;
    dataBlocks = new uint64_t[dataBlockCount];
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

void INode::readFromBuffer(char *buffer, unsigned position)
{
    uint16_t rawPermissions =
            RawDataUtils::readUintFromBuffer(buffer, position + TYPE_AND_PERMISSIONS_OFFSET, Permissions::getRawDataSize());
    permissions = new Permissions(rawPermissions);

    hardLinkCount = RawDataUtils::readUintFromBuffer(buffer, position + HARD_LINK_COUNT_OFFSET, sizeof(hardLinkCount));

    for (unsigned i = 0; i < dataBlockCount; ++i) {
        unsigned offset = getDataBlockOffset(i);
        dataBlocks[i] = RawDataUtils::readUintFromBuffer(buffer, position + offset, BLOCK_ADRESS_SIZE);
    }
}

void INode::writeToBuffer(char *buffer, unsigned position)
{
    RawDataUtils::writeUintToBuffer(permissions->getRawData(), buffer, position + TYPE_AND_PERMISSIONS_OFFSET,
                                    Permissions::getRawDataSize());
    RawDataUtils::writeUintToBuffer(hardLinkCount, buffer, position + HARD_LINK_COUNT_OFFSET, sizeof(hardLinkCount));

    for (unsigned i = 0; i < dataBlockCount; ++i) {
        unsigned offset = getDataBlockOffset(i);
        RawDataUtils::writeUintToBuffer(dataBlocks[i], buffer, position + offset, BLOCK_ADRESS_SIZE);
    }
}
