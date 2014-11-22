#include "filedisk.h"

FileDisk::FileDisk(const char *filePath, BlockSize blockSize)
{
    partition = openBlockDevice(filePath, blockSize);
}

FileDisk::~FileDisk()
{
    closeBlockDevice(&partition);
}

void FileDisk::readBlock(BlockId blockNumber, char *buffer)
{
    readBlockDevice(&partition, blockNumber, buffer);
}

void FileDisk::writeBlock(BlockId blockNumber, char *buffer)
{
    writeBlockDevice(&partition, blockNumber, buffer);
}

BlockSize FileDisk::getBlockSize()
{
    return partition.blockSize;
}

BlockCount FileDisk::getBlockNumber()
{
    return blockCount(&partition);
}
