#include "filedisk.h"
#include "log.h"

FileDisk::FileDisk(const char *filePath, BlockSize blockSize)
{
    partition = openBlockDevice(filePath, blockSize);
}

FileDisk::~FileDisk()
{
    Log::stream << "FileDisk destructor" << std::endl;
    closeBlockDevice(&partition);
}

void FileDisk::readBlock(BlockId blockNumber, char *buffer)
{
    readBlockDevice(&partition, blockNumber, buffer);
}

void FileDisk::writeBlock(BlockId blockNumber, const char *buffer)
{
    writeBlockDevice(&partition, blockNumber, buffer);
}

BlockSize FileDisk::getBlockSize()
{
    return partition.blockSize;
}

BlockCount FileDisk::getBlockCount()
{
    return blockCount(&partition);
}
