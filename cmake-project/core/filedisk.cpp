#include "filedisk.h"

FileDisk::FileDisk(char *filePath, int blockSize)
{
    partition = openBlockDevice(filePath, blockSize);
}

FileDisk::~FileDisk()
{
    closeBlockDevice(&partition);
}

void FileDisk::readBlock(int blockNumber, char *buffer)
{
    readBlockDevice(&partition, blockNumber, buffer);
}

void FileDisk::writeBlock(int blockNumber, char *buffer)
{
    writeBlockDevice(&partition, blockNumber, buffer);
}

int FileDisk::getBlockSize()
{
    return partition.blockSize;
}
