#ifndef INODE_H
#define INODE_H

#include <exception>
#include "permissions.h"
#include "bpfstypes.h"
#include "iindirectblock.h"

class WrongINodeSizeException : public std::exception {};
class MaximumBlockCountInInodeException : public std::exception {};
class PopDataBlockOnEmptyFileException : public std::exception {};
class UnlinkOnFreeNodeException : public std::exception {};

class InternalDataBlockCollection : public IIndirectBlock
{
private:
    BlockId *dataBlocks;
    BlockId nextIndirectBlock = INVALID_BLOCK_ID;
    unsigned dataBlocksPerNode;
public:
    InternalDataBlockCollection(unsigned dataBlocksPerNode);
    ~InternalDataBlockCollection();
    BlockCount getMaxSize() override;
    BlockId getBlockId(BlockCount position) override;
    void setBlockId(BlockCount position, BlockId id) override;
    BlockId getNextIndirectBlockId() override;
    void setNextIndirectBlockId(BlockId id) override;
};

class Inode
{
private:
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
    InodeId id;

    InternalDataBlockCollection dataBlockCollection;
    unsigned getDataBlockOffset(unsigned blockNumber);
public:
    static const InodeId ROOT_DIRECTORY_ID = 0;
    static const InodeId BLOCK_MANAGER_FILE_ID = 1;
    static const InodeId INVALID_ID = ROOT_DIRECTORY_ID;
    static const InodeSize MIN_SIZE = 32;
    Inode(InodeSize size, FileType fileType, InodeId id);
    Inode(InodeSize size, char *buffer, unsigned indexInBlock, InodeId id);
    ~Inode();
    void readFromBuffer(char *buffer, unsigned inBufferOffset);
    void writeToBuffer(char *buffer, unsigned inBufferOffset);

    BlockSize getLastBlockByteCount() const;
    void setLastBlockByteCount(const BlockSize &value);
    BlockCount getSizeInBlocks();
    void setSizeInBlocks(BlockCount size);
    InodeId getId();
    Permissions *getPermissions();
//    void clearForDebug();
    void clear();
    void addLink();
    void removeLink();
    uint8_t getHardLinkCount();
    bool isFree();
    IIndirectBlock &getDataBlockCollection();
};

#endif // INODE_H
