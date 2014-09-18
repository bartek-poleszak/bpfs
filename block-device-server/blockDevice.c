#include "blockDevice.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void seekToBlock(struct BlockDevice *partition, int blockNumber)
{
    off_t desiredOffset = blockNumber * partition->blockSize;
    off_t offset = lseek(partition->descriptor, desiredOffset, SEEK_SET);
    if (offset < 0) {
        perror(0);
    }
}

///
/// \brief openBlockDevice
/// \param path
/// \return file descriptor if file is opened, terminates if error occured
///

struct BlockDevice openBlockDevice(char *path, int blockSize)
{
    struct BlockDevice result;
    result.descriptor = open(path, O_RDWR | O_SYNC);
    result.blockSize = blockSize;
    if (result < 0) {
        perror(NULL);
        exit(1);
    }
    return result;
}


void readBlock(BlockDevice partition, int blockNumber, char *buffer)
{

}


void writeBlock(BlockDevice partition, int blockNumber, char *buffer)
{

}


int blockCount(BlockDevice partition)
{

}
