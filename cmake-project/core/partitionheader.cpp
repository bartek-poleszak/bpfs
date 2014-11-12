#include "partitionheader.h"
#include "rawdatautils.h"

std::string const PartitionHeader::IDENTIFIER = "BPFS";


uint32_t PartitionHeader::getBlockSize() const
{
    return blockSize;
}


uint8_t PartitionHeader::getINodeSize() const
{
    return iNodeSize;
}

PartitionHeader::PartitionHeader()
{
    this->blockNumber = 0;
    this->blockSize = 0;
    this->iNodeSize = 0;
}

using namespace std;
void PartitionHeader::writeToDisk(IDisk &disk, uint8_t iNodeSize)
{
    this->iNodeSize = iNodeSize;
    char buffer[disk.getBlockSize()];

    unsigned i;
    for (i = 0; i < IDENTIFIER.size(); ++i) {
        buffer[i] = IDENTIFIER.at(i);
    }

    blockSize = disk.getBlockSize();
    blockNumber = disk.getBlockNumber();

    RawDataUtils::writeUintToBuffer(blockSize, buffer, i, sizeof(blockSize));
    i += sizeof(blockSize);
    RawDataUtils::writeUintToBuffer(blockNumber, buffer, i, sizeof(blockNumber));
    i += sizeof(blockNumber);
    RawDataUtils::writeUintToBuffer(this->iNodeSize, buffer, i, sizeof(this->iNodeSize));
    i += sizeof(this->iNodeSize);

    //fill rest of array with zeroes
    for ( ; i < disk.getBlockSize(); ++i) {
        buffer[i] = 0;
    }

//    for (int i = 0; i < 50; ++i) {
//        std::cout << i << ":" << (int)((unsigned char)buffer[i]) << std::endl;
//    }
    disk.writeBlock(BLOCK, buffer);
}

uint64_t PartitionHeader::getBlockNumber() const
{
    return blockNumber;
}
