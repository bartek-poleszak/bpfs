#ifndef FSPARTITION_H
#define FSPARTITION_H

#include <string>
#include "idisk.h"
#include "ifreeblockmanager.h"
#include "inodetable.h"

class PartitionHeader;

class FSPartition
{
private:
    IDisk *disk;
    PartitionHeader *header;
    IFreeBlockManager *freeBlockManager;
    InodeTable *inodeTable;
public:
    static const int INODE_TABLE_BLOCK = 1;

    FSPartition(IDisk *disk);
    ~FSPartition();
    PartitionHeader *getHeader() const;
    BlockSize getBlockSize();
    BlockId getFreeBlock();
    INode *getInode(InodeId inodeId);
    void writeDataBlock(BlockId blockNumber, char *buffer);
    void readDataBlock(BlockId blockNumber, char *buffer);
    BlockId getFirstDataBlock();
};

#endif // FSPARTITION_H
