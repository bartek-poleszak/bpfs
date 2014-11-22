#include "partitionheader.h"
#include "rawdatautils.h"

std::string const PartitionHeader::IDENTIFIER = "BPFS";


BlockSize PartitionHeader::getBlockSize() const
{
    return blockSize;
}


InodeSize PartitionHeader::getINodeSize() const
{
    return iNodeSize;
}

PartitionHeader::PartitionHeader()
{
    this->blockCount = 0;
    this->blockSize = 0;
    this->iNodeSize = 0;
}

PartitionHeader::PartitionHeader(IDisk &disk)
{
    char buffer[disk.getBlockSize()];
    disk.readBlock(HEADER_BLOCK, buffer);
    blockCount = RawDataUtils::readUintFromBuffer(buffer, BLOCK_NUMBER_OFFSET, sizeof(blockCount));
    blockSize = RawDataUtils::readUintFromBuffer(buffer, BLOCK_SIZE_OFFSET, sizeof(blockSize));
    iNodeSize = RawDataUtils::readUintFromBuffer(buffer, INODE_SIZE_OFFSET, sizeof(iNodeSize));
}

//#include <iostream>
//using namespace std;
void PartitionHeader::writeToDisk(IDisk &disk, InodeSize iNodeSize)
{
    this->iNodeSize = iNodeSize;
    char buffer[disk.getBlockSize()];

    for (unsigned i = IDENTIFIER_OFFSET; i < IDENTIFIER.size(); ++i) {
        buffer[i] = IDENTIFIER.at(i);
    }

    blockSize = disk.getBlockSize();
    blockCount = disk.getBlockNumber();

    RawDataUtils::writeUintToBuffer(blockSize, buffer, BLOCK_SIZE_OFFSET, sizeof(blockSize));
    RawDataUtils::writeUintToBuffer(blockCount, buffer, BLOCK_NUMBER_OFFSET, sizeof(blockCount));
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

BlockCount PartitionHeader::getBlockCount() const
{
    return blockCount;
}
