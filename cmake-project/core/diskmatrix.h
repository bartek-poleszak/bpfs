#ifndef DISKMATRIX_H
#define DISKMATRIX_H

#include <vector>
#include <string>
#include "idisk.h"
#include "blockDevice.h"

class DiskMatrix : public IDisk
{
private:
    BlockSize blockSize;
    std::vector<BlockDevice> blockDevices;
public:
    DiskMatrix(std::vector<std::string> &paths, BlockSize blockSize);
    ~DiskMatrix() override;
    void readBlock(BlockId blockNumber, char *buffer) override;
    void writeBlock(BlockId blockNumber, const char *buffer) override;
    BlockSize getBlockSize() override;
    BlockCount getBlockCount() override;
};

#endif // DISKMATRIX_H
