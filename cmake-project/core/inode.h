#ifndef INODE_H
#define INODE_H

#include <exception>
#include "permissions.h"
#include "bpfstypes.h"

class WrongINodeSizeException : public std::exception {};
class MaximumFileSizeAchievedException : public std::exception {};

class Inode
{
private:
    unsigned size;
    static const unsigned BLOCK_ADRESS_SIZE = 8;

    static const unsigned TYPE_AND_PERMISSIONS_OFFSET = 0; //uint16_t
    static const unsigned HARD_LINK_COUNT_OFFSET = 2; //uint8_t
    static const unsigned BLOCK_COUNT_OFFSET = 4; //uint64_t
    static const unsigned LAST_BLOCK_BYTE_COUNT_OFFSET = 12; //uint32_t
    //...
    static const unsigned FIRST_DATA_BLOCK_OFFSET = 24; //uint64_t

    Permissions *permissions;
    uint8_t hardLinkCount;
    BlockCount sizeInBlocks;
    BlockSize lastBlockByteCount;

    BlockId *dataBlocks;
    unsigned dataBlocksPerNode;
    InodeId index;

    unsigned getDataBlockOffset(unsigned blockNumber);
public:
    static const InodeSize MIN_SIZE = 32;
    Inode(InodeSize size);
    ~Inode();
    void readFromBuffer(char *buffer, unsigned position);
    void writeToBuffer(char *buffer, unsigned position);

    void addDataBlock(BlockId blockNumber);
    BlockId getDataBlockNumber(BlockCount sequenceNumber);
    BlockSize getLastBlockByteCount() const;
    void setLastBlockByteCount(const BlockSize &value);
    BlockCount getSizeInBlocks();
    InodeId getIndex();
    void clearForDebug();
};

#endif // INODE_H
