#include "fspartition.h"
#include "simplefreeblockmanager.h"
#include "partitionheader.h"

FSPartition::FSPartition(IDisk *disk)
{
    this->disk = disk;
    this->header = new PartitionHeader(*disk);
    this->freeBlockManager = new SimpleFreeBlockManager(header);
    this->inodeTable = new InodeTable(*disk, header);
}

FSPartition::~FSPartition()
{
    delete freeBlockManager;
    delete header;
    delete inodeTable;
}

PartitionHeader *FSPartition::getHeader() const
{
    return header;
}

BlockSize FSPartition::getBlockSize()
{
    return header->getBlockSize();
}

BlockId FSPartition::getFreeBlock()
{
    return freeBlockManager->getFreeBlock();
}

INode *FSPartition::getInode(InodeId inodeId)
{
    return inodeTable->getInode(inodeId);
}

void FSPartition::writeDataBlock(BlockId blockNumber, char *buffer)
{
    disk->writeBlock(blockNumber, buffer);
}

void FSPartition::readDataBlock(BlockId blockNumber, char *buffer)
{
    disk->readBlock(blockNumber, buffer);
}

BlockId FSPartition::getFirstDataBlock()
{
    return INODE_TABLE_BLOCK + inodeTable->getTableSize();
}
