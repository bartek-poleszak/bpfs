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
    uint32_t blockNumber;
public:
    static const int SIZE = 1024;
    PartitionHeader();
    void writeToDisk(IDisk &disk);
    void writeUintToBuffer(uint32_t value, char *buffer, int position);
};

#endif // PARTITIONHEADER_H
