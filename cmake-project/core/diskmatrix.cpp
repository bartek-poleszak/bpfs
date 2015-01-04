#include "diskmatrix.h"
#include <algorithm>

bool blockCountComparator(BlockDevice &a, BlockDevice &b) {
    return blockCount(&a) < blockCount(&b);
}

DiskMatrix::DiskMatrix(std::vector<std::string> &paths, BlockSize blockSize)
{
    this->blockSize = blockSize;
    for (std::string &path : paths)
        blockDevices.push_back(openBlockDevice(path.c_str(), blockSize));
}

DiskMatrix::~DiskMatrix()
{
    for (auto &blockDevice : blockDevices)
        closeBlockDevice(&blockDevice);
}

void DiskMatrix::readBlock(BlockId blockNumber, char *buffer)
{
    int deviceId = blockNumber % blockDevices.size();
    readBlockDevice(&blockDevices[deviceId], blockNumber / blockDevices.size(), buffer);
}

void DiskMatrix::writeBlock(BlockId blockNumber, const char *buffer)
{
    int deviceId = blockNumber % blockDevices.size();
    writeBlockDevice(&blockDevices[deviceId], blockNumber / blockDevices.size(), buffer);
}

BlockSize DiskMatrix::getBlockSize()
{
    return this->blockSize;
}

BlockCount DiskMatrix::getBlockCount()
{
    BlockDevice smallestDevice = *std::min_element(blockDevices.begin(), blockDevices.end(), blockCountComparator);
    return blockCount(&smallestDevice);
}
