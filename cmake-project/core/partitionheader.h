#ifndef PARTITIONHEADER_H
#define PARTITIONHEADER_H

#include "idisk.h"

#include <string>
#include <cstdint>

class PartitionHeader
{
private:
    static const std::string IDENTIFIER;
    static const int IDENTIFIER_OFFSET = 0;
    static const int BLOCK_SIZE_OFFSET = 4;
    static const int BLOCK_NUMBER_OFFSET = 8;
    static const int INODE_SIZE_OFFSET = 16;

    uint32_t blockSize;
    uint64_t blockNumber;
    uint16_t iNodeSize;
public:
    static const int HEADER_BLOCK = 0;
    PartitionHeader();
    PartitionHeader(IDisk &disk);
    void writeToDisk(IDisk &disk, uint16_t iNodeSize);
    uint32_t getBlockSize() const;
    uint64_t getBlockNumber() const;
    uint16_t getINodeSize() const;
};

#endif // PARTITIONHEADER_H
