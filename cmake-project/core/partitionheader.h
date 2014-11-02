#ifndef PARTITIONHEADER_H
#define PARTITIONHEADER_H

#include "idisk.h"

#include <string>
#include <cstdint>

class PartitionHeader
{
private:
    static const std::string IDENTIFIER;
    uint32_t blockSize;
    uint64_t blockNumber;
public:
    static const int BLOCK = 0;
    PartitionHeader();
    PartitionHeader(IDisk &disk);
    void writeToDisk(IDisk &disk);
    void writeUintToBuffer(unsigned long long value, char *buffer, int position, int bytes);
    uint32_t getBlockSize() const;
    uint32_t getBlockNumber() const;
};

#endif // PARTITIONHEADER_H
