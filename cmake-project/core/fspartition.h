#ifndef FSPARTITION_H
#define FSPARTITION_H

#include <string>
#include "idisk.h"
#include "ifreeblockmanager.h"
#include "inodetable.h"

class Directory;
class PartitionHeader;

class FSPartition
{
private:
    IDisk &disk;
    PartitionHeader *header;
    IFreeBlockManager *freeBlockManager;
    InodeTable *inodeTable;
    void flushFreeBlockManager();
public:
    static const int INODE_TABLE_BLOCK = 1;

    FSPartition(IDisk &disk);
    ~FSPartition();
    PartitionHeader *getHeader() const;
    BlockSize getBlockSize();
    BlockId getFreeBlock();
    Inode *getInode(InodeId inodeId);
    Inode *getFreeInode();
    void writeDataBlock(BlockId blockNumber, const char *buffer);
    void readDataBlock(BlockId blockNumber, char *buffer);
    void flushInodeTable();
    BlockId getFirstDataBlock();
    void onPartitionCreation();
    void markBlockAsFree(BlockId id);
};

#endif // FSPARTITION_H
