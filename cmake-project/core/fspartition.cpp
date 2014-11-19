#include "fspartition.h"
#include "simplefreeblockmanager.h"
#include "partitionheader.h"

FSPartition::FSPartition(IDisk *disk)
{
    this->disk = disk;
    this->header = new PartitionHeader(*disk);
    this->freeBlockManager = new SimpleFreeBlockManager(header);
}

FSPartition::~FSPartition()
{
    delete freeBlockManager;
    delete header;
}

PartitionHeader *FSPartition::getHeader() const
{
    return header;
}

