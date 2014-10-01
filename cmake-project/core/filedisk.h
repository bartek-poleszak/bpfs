#ifndef FILEDISK_H
#define FILEDISK_H

#include "idisk.h"
#include "blockDevice.h"

class FileDisk : public IDisk
{
private:
    BlockDevice partition;

public:
    FileDisk(char *filePath, int blockSize);
    ~FileDisk();
    void readBlock(int blockNumber, char *buffer);
    void writeBlock(int blockNumber, char *buffer);
    int getBlockSize();
};

#endif // FILEDISK_H
