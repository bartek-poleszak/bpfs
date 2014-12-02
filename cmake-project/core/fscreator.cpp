#include "fscreator.h"
#include "partitionheader.h"

FSCreator::FSCreator()
{

}

void FSCreator::createFilesystem(IDisk &disk, BlockSize blockSize, InodeSize inodeSize, InodeCount inodeCount)
{
    PartitionHeader header;
    header.writeToDisk(disk, inodeSize, inodeCount);

    InodeTable table(disk, inodeCount, inodeSize, blockSize);
    table.writeAllToDisk();

}


