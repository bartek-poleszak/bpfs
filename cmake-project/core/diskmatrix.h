#ifndef DISKMATRIX_H
#define DISKMATRIX_H

#include <vector>
#include <string>
#include "idisk.h"
#include "blockDevice.h"
#include "encryptor.h"

class DiskMatrix : public IDisk
{
private:
    BlockSize blockSize;
    std::vector<BlockDevice> blockDevices;
    Encryptor &encryptor;
    char *encryptorBuffer;
    void registerDevices(std::vector<std::string> &paths, bool newKeys);
    int getKeyId(int deviceId);
public:
    DiskMatrix(std::vector<std::string> &paths, BlockSize blockSize, Encryptor &encryptor, bool newKeys);
    DiskMatrix(std::vector<std::string> &paths, BlockSize blockSize, Encryptor &encryptor);
    ~DiskMatrix() override;
    void readBlock(BlockId blockNumber, char *buffer) override;
    void writeBlock(BlockId blockNumber, const char *buffer) override;
    BlockSize getBlockSize() override;
    BlockCount getBlockCount() override;
    void generateDeviceKey(BlockDevice &device);
};

#endif // DISKMATRIX_H
