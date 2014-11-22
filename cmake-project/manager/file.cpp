#include "file.h"

File::File(INode *inode, FSPartition *fsPartition)
{
    this->inode = inode;
    this->fsPartition = fsPartition;
}

File::~File()
{

}

#include <iostream>
using namespace std;
void File::write(std::ifstream &fileStream)
{
    BlockSize bufferSize = fsPartition->getBlockSize();
    char buffer[bufferSize];

    BlockId blockId;
    fileStream.read(buffer, bufferSize);
    while (fileStream.gcount() == bufferSize) {
        blockId = fsPartition->getFreeBlock();
        fsPartition->writeDataBlock(blockId, buffer);
        inode->addDataBlock(blockId);
        fileStream.read(buffer, bufferSize);
    }
    blockId = fsPartition->getFreeBlock();
    fsPartition->writeDataBlock(blockId, buffer);
    inode->addDataBlock(blockId);
    inode->setLastBlockByteCount(fileStream.gcount());
}

void File::get(std::ofstream &fileStream)
{
    BlockSize bufferSize = fsPartition->getBlockSize();
    char buffer[bufferSize];

    BlockId blockId;
    for (BlockCount i = 0; i < inode->getBlockCount() - 1; ++i) {
        blockId = inode->getDataBlockNumber(i);
        fsPartition->readDataBlock(blockId, buffer);
        fileStream.write(buffer, bufferSize);
    }
    blockId = inode->getDataBlockNumber(inode->getBlockCount() - 1);
    fsPartition->readDataBlock(blockId, buffer);
    fileStream.write(buffer, inode->getLastBlockByteCount());

}
