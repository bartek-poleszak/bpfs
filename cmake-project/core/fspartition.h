#ifndef FSPARTITION_H
#define FSPARTITION_H

#include <string>
#include "idisk.h"
#include "partitionheader.h"

class FSPartition
{
private:
    IDisk *disk;
    PartitionHeader header;
public:
    static const int INODE_TABLE_BLOCK = 1;

    FSPartition(IDisk *disk);
};

#endif // FSPARTITION_H
