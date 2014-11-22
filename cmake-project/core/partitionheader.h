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

    BlockSize blockSize;
    BlockCount blockCount;
    InodeSize iNodeSize;
public:
    static const int HEADER_BLOCK = 0;
    PartitionHeader();
    PartitionHeader(IDisk &disk);
    void writeToDisk(IDisk &disk, InodeSize iNodeSize);
    BlockSize getBlockSize() const;
    BlockCount getBlockCount() const;
    InodeSize getINodeSize() const;
};

#endif // PARTITIONHEADER_H
