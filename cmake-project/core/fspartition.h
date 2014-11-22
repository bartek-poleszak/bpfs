#ifndef FSPARTITION_H
#define FSPARTITION_H

#include <string>
#include "idisk.h"
#include "ifreeblockmanager.h"

class PartitionHeader;

class FSPartition
{
private:
    IDisk *disk;
    PartitionHeader *header;
    IFreeBlockManager *freeBlockManager;
public:
    static const int INODE_TABLE_BLOCK = 1;

    FSPartition(IDisk *disk);
    ~FSPartition();
    PartitionHeader *getHeader() const;
    BlockSize getBlockSize();
    BlockId getFreeBlock();
    void writeDataBlock(BlockId blockNumber, char *buffer);
    void readDataBlock(BlockId blockNumber, char *buffer);
};

#endif // FSPARTITION_H
