#include "partitionheader.h"
#include "rawdatautils.h"

std::string const PartitionHeader::IDENTIFIER = "BPFS";


uint32_t PartitionHeader::getBlockSize() const
{
    return blockSize;
}


uint16_t PartitionHeader::getINodeSize() const
{
    return iNodeSize;
}

PartitionHeader::PartitionHeader()
{
    this->blockNumber = 0;
    this->blockSize = 0;
    this->iNodeSize = 0;
}

PartitionHeader::PartitionHeader(IDisk &disk)
{
    char buffer[disk.getBlockSize()];
    disk.readBlock(HEADER_BLOCK, buffer);
    blockNumber = RawDataUtils::readUintFromBuffer(buffer, BLOCK_NUMBER_OFFSET, sizeof(blockNumber));
    blockSize = RawDataUtils::readUintFromBuffer(buffer, BLOCK_SIZE_OFFSET, sizeof(blockSize));
    iNodeSize = RawDataUtils::readUintFromBuffer(buffer, INODE_SIZE_OFFSET, sizeof(iNodeSize));
}

//#include <iostream>
//using namespace std;
void PartitionHeader::writeToDisk(IDisk &disk, uint16_t iNodeSize)
{
    this->iNodeSize = iNodeSize;
    char buffer[disk.getBlockSize()];

    for (unsigned i = IDENTIFIER_OFFSET; i < IDENTIFIER.size(); ++i) {
        buffer[i] = IDENTIFIER.at(i);
    }

    blockSize = disk.getBlockSize();
    blockNumber = disk.getBlockNumber();

    RawDataUtils::writeUintToBuffer(blockSize, buffer, BLOCK_SIZE_OFFSET, sizeof(blockSize));
    RawDataUtils::writeUintToBuffer(blockNumber, buffer, BLOCK_NUMBER_OFFSET, sizeof(blockNumber));
    RawDataUtils::writeUintToBuffer(this->iNodeSize, buffer, INODE_SIZE_OFFSET, sizeof(this->iNodeSize));

    //fill rest of array with zeroes
    for (unsigned i = INODE_SIZE_OFFSET + sizeof(this->iNodeSize); i < disk.getBlockSize(); ++i) {
        buffer[i] = 0;
    }

//    for (int i = 0; i < 50; ++i) {
//        cout << i << ":" << (int)((unsigned char)buffer[i]) << endl;
//    }
    disk.writeBlock(HEADER_BLOCK, buffer);
}

uint64_t PartitionHeader::getBlockNumber() const
{
    return blockNumber;
}
