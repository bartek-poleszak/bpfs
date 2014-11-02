#ifndef FSPARTITION_H
#define FSPARTITION_H

#include "idisk.h"

#include <string>

class FSPartition
{
private:
    IDisk *disk;
public:
    static const int INODE_TABLE_BLOCK = 1;

    FSPartition(IDisk *disk);
};

#endif // FSPARTITION_H
