#include "fspartition.h"
#include "partitionheader.h"
#include "log.h"
#include "listfreeblockmanager.h"

FSPartition::FSPartition(IDisk &disk)
    : disk(disk)
{
    this->header = new PartitionHeader(disk);
    this->inodeTable = new InodeTable(disk, header);
//    this->freeBlockManager = new SimpleFreeBlockManager(this);
    this->freeBlockManager = new ListFreeBlockManager(this);
}

FSPartition::~FSPartition()
{
    flushInodeTable();
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
    auto result = freeBlockManager->getFreeBlock();
    freeBlockManager->flush();
    Log::stream << "Free block recieved: " << result << std::endl;
    return result;
}

Inode *FSPartition::getInode(InodeId inodeId)
{
    return inodeTable->getInode(inodeId);
}

Inode *FSPartition::getFreeInode()
{
    return inodeTable->getFreeNode();
}

void FSPartition::writeDataBlock(BlockId blockNumber, const char *buffer)
{
    disk.writeBlock(blockNumber, buffer);
}

void FSPartition::readDataBlock(BlockId blockNumber, char *buffer)
{
    disk.readBlock(blockNumber, buffer);
}

void FSPartition::flushInodeTable()
{
    inodeTable->writeCachedToDisk();
}

BlockId FSPartition::getFirstDataBlock()
{
    return INODE_TABLE_BLOCK + inodeTable->getSizeInBytes();
}

void FSPartition::onPartitionCreation()
{
    freeBlockManager->onPartitionCreation();
}

void FSPartition::markBlockAsFree(BlockId id)
{
    freeBlockManager->markBlockAsFree(id);
}

void FSPartition::flushFreeBlockManager() {
    freeBlockManager->flush();
}
