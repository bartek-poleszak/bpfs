#include "diskmatrix.h"
#include <algorithm>

bool blockCountComparator(BlockDevice &a, BlockDevice &b) {
    return blockCount(&a) < blockCount(&b);
}

void DiskMatrix::registerDevices(std::vector<std::string> &paths, bool newKeys)
{
    for (std::string &path : paths) {
        BlockDevice device = openBlockDevice(path.c_str(), blockSize);
        blockDevices.push_back(device);
        if (newKeys)
            generateDeviceKey(device);
        readBlockDevice(&device, 0, encryptorBuffer);
        encryptor.addKey(encryptorBuffer);
    }
}

int DiskMatrix::getKeyId(int deviceId)
{
    return (deviceId + 1) % blockDevices.size();
}

DiskMatrix::DiskMatrix(std::vector<std::string> &paths, BlockSize blockSize, Encryptor &encryptor, bool newKeys)
    : encryptor(encryptor)
{
    this->blockSize = blockSize;
    encryptor.setBlockSize(blockSize);
    encryptorBuffer = new char[blockSize];
    registerDevices(paths, newKeys);
}

DiskMatrix::DiskMatrix(std::vector<std::string> &paths, BlockSize blockSize, Encryptor &encryptor)
    : DiskMatrix(paths, blockSize, encryptor, false)
{

}

DiskMatrix::~DiskMatrix()
{
    delete [] encryptorBuffer;
    for (auto &blockDevice : blockDevices)
        closeBlockDevice(&blockDevice);
}

void DiskMatrix::readBlock(BlockId blockNumber, char *buffer)
{
    int deviceId = blockNumber % blockDevices.size();
    //pierwszy blok urzadzenia zawiera klucz
    readBlockDevice(&blockDevices[deviceId], blockNumber / blockDevices.size() + 1, encryptorBuffer);
    encryptor.decryptBlock(encryptorBuffer, buffer, getKeyId(deviceId));
}

void DiskMatrix::writeBlock(BlockId blockNumber, const char *buffer)
{
    int deviceId = blockNumber % blockDevices.size();
    encryptor.encryptBlock(buffer, encryptorBuffer, getKeyId(deviceId));
    writeBlockDevice(&blockDevices[deviceId], blockNumber / blockDevices.size() + 1, encryptorBuffer);
}

BlockSize DiskMatrix::getBlockSize()
{
    return this->blockSize;
}

BlockCount DiskMatrix::getBlockCount()
{
    BlockDevice smallestDevice = *std::min_element(blockDevices.begin(), blockDevices.end(), blockCountComparator);
    return blockCount(&smallestDevice) * blockDevices.size() - blockDevices.size(); //pierwszy blok kazdego urzadzenia zawiera klucz
}

void DiskMatrix::generateDeviceKey(BlockDevice &device)
{
    encryptor.generateKeyBlock(encryptorBuffer);
    writeBlockDevice(&device, 0, encryptorBuffer);
}
