#ifndef INODETABLE_H
#define INODETABLE_H

#include <exception>
#include <map>
#include "inode.h"
#include "idisk.h"
#include "partitionheader.h"

class WrongInodeSizeException : public std::exception {};
class WrongInodeCountException : public std::exception {};
class InodeTableFullException : public std::exception {};

class InodeTable
{
private:
    IDisk *disk;
    std::map<InodeId, Inode *> cachedInodes;
    BlockSize blockSize;
    InodeSize inodeSize;
    InodeCount inodeCount;
    BlockCount sizeInBytes;
    Inode *getInodeFromMemory(InodeId inodeId);
    unsigned nodesPerBlock;
    void writeNodeToDisk(Inode *inode);
public:
    static const int INODE_TABLE_BEGIN_BLOCK = 1;
    InodeTable(IDisk &disk, InodeCount inodeCount, InodeSize inodeSize, BlockSize blockSize);
    InodeTable(IDisk &disk, PartitionHeader *header);
    ~InodeTable();
    void writeCachedToDisk();
    Inode *getInode(InodeId inodeId);
    BlockCount getSizeInBytes();
    void calculateSizeAndNodesPerBlock();
    Inode *getFreeNode();
    void onPartitionCreation();
};

#endif // INODETABLE_H
