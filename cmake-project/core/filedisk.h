#ifndef FILEDISK_H
#define FILEDISK_H

#include "idisk.h"
#include "blockDevice.h"

class FileDisk : public IDisk
{
private:
    BlockDevice partition;

public:
    FileDisk(const char *filePath, int blockSize);
    ~FileDisk();
    void readBlock(int blockNumber, char *buffer);
    void writeBlock(int blockNumber, char *buffer);
    int getBlockSize();
    unsigned long long getBlockNumber();
};

#endif // FILEDISK_H
