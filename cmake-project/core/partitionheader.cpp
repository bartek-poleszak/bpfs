#include "partitionheader.h"
#include "rawdatautils.h"

std::string const PartitionHeader::IDENTIFIER = "BPFS";


BlockSize PartitionHeader::getBlockSize() const
{
    return blockSize;
}


InodeSize PartitionHeader::getInodeSize() const
{
    return inodeSize;
}

InodeCount PartitionHeader::getInodeCount() const
{
    return inodeCount;
}

void PartitionHeader::validateFilesystem(char *headerBuffer)
{
    std::string fromBuffer;
    for (unsigned i = 0; i < IDENTIFIER.size(); ++i) {
        fromBuffer.push_back(headerBuffer[i]);
    }
    if (fromBuffer != IDENTIFIER)
        throw InvalidFilesystemIdenifierException();
}

PartitionHeader::PartitionHeader()
{
    this->blockCount = 0;
    this->blockSize = 0;
    this->inodeSize = 0;
    this->inodeCount = 0;
}

PartitionHeader::PartitionHeader(IDisk &disk)
{
    char buffer[disk.getBlockSize()];
    disk.readBlock(HEADER_BLOCK, buffer);
    validateFilesystem(buffer);
    blockCount = RawDataUtils::readUintFromBuffer(buffer, BLOCK_NUMBER_OFFSET, sizeof(blockCount));
    blockSize = RawDataUtils::readUintFromBuffer(buffer, BLOCK_SIZE_OFFSET, sizeof(blockSize));
    inodeSize = RawDataUtils::readUintFromBuffer(buffer, INODE_SIZE_OFFSET, sizeof(inodeSize));
    inodeCount = RawDataUtils::readUintFromBuffer(buffer, INODE_COUNT_OFFSET, sizeof(inodeCount));
}

//#include <iostream>
//using namespace std;
void PartitionHeader::writeToDisk(IDisk &disk, InodeSize inodeSize, InodeCount inodeCount)
{
    this->inodeSize = inodeSize;
    this->inodeCount = inodeCount;
    char buffer[disk.getBlockSize()];

    for (unsigned i = IDENTIFIER_OFFSET; i < IDENTIFIER.size(); ++i) {
        buffer[i] = IDENTIFIER.at(i);
    }

    blockSize = disk.getBlockSize();
    blockCount = disk.getBlockCount();

    RawDataUtils::writeUintToBuffer(blockSize, buffer, BLOCK_SIZE_OFFSET, sizeof(blockSize));
    RawDataUtils::writeUintToBuffer(blockCount, buffer, BLOCK_NUMBER_OFFSET, sizeof(blockCount));
    RawDataUtils::writeUintToBuffer(this->inodeSize, buffer, INODE_SIZE_OFFSET, sizeof(this->inodeSize));
    RawDataUtils::writeUintToBuffer(this->inodeCount, buffer, INODE_COUNT_OFFSET, sizeof(this->inodeCount));

    //fill rest of array with zeroes
    for (unsigned i = INODE_COUNT_OFFSET + sizeof(this->inodeCount); i < disk.getBlockSize(); ++i) {
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
