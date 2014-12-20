#ifndef FILEDISK_H
#define FILEDISK_H

#include "idisk.h"
#include "blockDevice.h"
#include "bpfstypes.h"

class FileDisk : public IDisk
{
private:
    BlockDevice partition;

public:
    FileDisk(const char *filePath, BlockSize blockSize);
    ~FileDisk() override;
    void readBlock(BlockId blockNumber, char *buffer) override;
    void writeBlock(BlockId blockNumber, const char *buffer) override;
    BlockSize getBlockSize() override;
    BlockCount getBlockCount() override;
};


#endif // FILEDISK_H
