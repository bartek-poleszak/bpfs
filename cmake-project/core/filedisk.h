#ifndef FILEDISK_H
#define FILEDISK_H

#include "idisk.h"
#include "blockDevice.h"
#include "bpfstypes.h"

class FileDisk : public IDisk
{
private:
    BlockDevice partition;

public:
    FileDisk(const char *filePath, BlockSize blockSize);
    ~FileDisk();
    void readBlock(BlockId blockNumber, char *buffer);
    void writeBlock(BlockId blockNumber, char *buffer);
    BlockSize getBlockSize();
    BlockCount getBlockCount();
};


#endif // FILEDISK_H
