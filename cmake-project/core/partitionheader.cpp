#include "partitionheader.h"

std::string const PartitionHeader::IDENTIFIER = "BPFS";


uint32_t PartitionHeader::getBlockSize() const
{
    return blockSize;
}

PartitionHeader::PartitionHeader()
{
    this->blockNumber = 0;
    this->blockSize = 0;
}


void PartitionHeader::writeUintToBuffer(unsigned long long value, char *buffer, int position, int bytes)
{
    for (int i = 0; i < bytes; ++i) {
        char val = value % 256;
        buffer[position + i] = val;
//        buffer[position + i] = bytes;
        value = value >> 8;
    }
}

#include <iostream>
using namespace std;
void PartitionHeader::writeToDisk(IDisk &disk)
{
    char buffer[disk.getBlockSize()];

    int i;
    for (i = 0; i < IDENTIFIER.size(); ++i) {
        cout << "dupa " << IDENTIFIER.at(i) << endl;
        buffer[i] = IDENTIFIER.at(i);
    }

    blockSize = disk.getBlockSize();
    blockNumber = disk.getBlockNumber();

    writeUintToBuffer(blockSize, buffer, i, sizeof(blockSize));
    i += sizeof(blockSize);
    writeUintToBuffer(blockNumber, buffer, i, sizeof(blockNumber));
    i += sizeof(blockNumber);

    //fill rest of array with zeroes
    for ( ; i < disk.getBlockSize(); ++i) {
        buffer[i] = 0;
    }

    for (int i = 0; i < 50; ++i) {
        std::cout << i << ":" << (int)((unsigned char)buffer[i]) << std::endl;
    }
    disk.writeBlock(BLOCK, buffer);
}

uint32_t PartitionHeader::getBlockNumber() const
{
    return blockNumber;
}
