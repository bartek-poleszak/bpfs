#ifndef DISK_H
#define DISK_H

#include "bpfstypes.h"

class IDisk
{
public:
    IDisk();
    virtual ~IDisk();
    virtual void readBlock(BlockId blockNumber, char *buffer) = 0;
    virtual void writeBlock(BlockId blockNumber, char *buffer) = 0;
    virtual BlockSize getBlockSize() = 0;
    virtual BlockCount getBlockCount() = 0;
};

#endif // DISK_H
