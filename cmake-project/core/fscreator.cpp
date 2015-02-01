#include "fscreator.h"
#include "partitionheader.h"
#include "file.h"
#include "rootdirectory.h"
#include "log.h"

FSCreator::FSCreator()
{

}

void FSCreator::createFilesystem(IDisk &disk, BlockSize blockSize, InodeSize inodeSize, InodeCount inodeCount)
{
    PartitionHeader header;
    header.writeToDisk(disk, inodeSize, inodeCount);

    InodeTable table(disk, inodeCount, inodeSize, blockSize);

    FSPartition partition(disk, true);
    partition.onPartitionCreation();
    File rootDirectory("", table.getInode(Inode::ROOT_DIRECTORY_ID), &partition);
    rootDirectory.getInode()->addLink();
    table.writeCachedToDisk();
}


