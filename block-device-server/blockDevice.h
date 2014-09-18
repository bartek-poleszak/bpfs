#ifndef BLOCKDEVICE_H
#define BLOCKDEVICE_H

typedef int fileDescriptor;
typedef struct {
    fileDescriptor descriptor;
    int blockSize;
} BlockDevice;

struct BlockDevice openBlockDevice(char *path, int blockSIze);
void readBlock(struct BlockDevice partition, int blockNumber, char *buffer);
void writeBlock(struct BlockDevice partition, int blockNumber, char *buffer);
int blockCount(struct BlockDevice partition);

#endif // BLOCKDEVICE_H
