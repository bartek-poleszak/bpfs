#ifndef BLOCKDEVICE_H
#define BLOCKDEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int fileDescriptor;
typedef struct {
    fileDescriptor descriptor;
    int blockSize;
} BlockDevice;

BlockDevice openBlockDevice(const char *path, int blockSIze);
void closeBlockDevice(BlockDevice *partition);
void readBlockDevice(BlockDevice *partition, int blockNumber, char *buffer);
void writeBlockDevice(BlockDevice *partition, int blockNumber, char *buffer);
unsigned long long blockCount(BlockDevice *partition);

#ifdef __cplusplus
}
#endif

#endif // BLOCKDEVICE_H
