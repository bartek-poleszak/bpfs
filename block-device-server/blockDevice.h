#ifndef BLOCKDEVICE_H
#define BLOCKDEVICE_H

typedef int fileDescriptor;
typedef struct {
    fileDescriptor descriptor;
    int blockSize;
} BlockDevice;

BlockDevice openBlockDevice(char *path, int blockSIze);
void readBlock(BlockDevice partition, int blockNumber, char *buffer);
void writeBlock(BlockDevice partition, int blockNumber, char *buffer);
unsigned blockCount(BlockDevice partition);

#endif // BLOCKDEVICE_H
