#ifndef INODETABLE_H
#define INODETABLE_H

#include <exception>
#include "inode.h"
#include "idisk.h"
#include "partitionheader.h"

class WrongInodeSizeException : public std::exception {};
class WrongInodeCountException : public std::exception {};

class InodeTable
{
private:
    Inode tmp;
    BlockSize blockSize;
    InodeSize inodeSize;
    InodeCount inodeCount;
    BlockCount size;
public:
    static const int INODE_TABLE_BEGIN_BLOCK = 1;
    InodeTable(InodeCount inodeCount, InodeSize inodeSize, BlockSize blockSize);
    InodeTable(IDisk &disk, PartitionHeader *header);
    void writeAllToDisk(IDisk &disk);
    Inode *getInode(InodeId inodeId);
    BlockCount getTableSize();
//    INode *getFreeInode();
    void calculateSize();
};

#endif // INODETABLE_H
