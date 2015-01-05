#ifndef FSCREATOR_H
#define FSCREATOR_H

#include "fspartition.h"
#include "idisk.h"

class FSCreator
{
public:
    FSCreator();
    void createFilesystem(IDisk &partition, BlockSize blockSize, InodeSize inodeSize, InodeCount inodeCount);
    InodeTable * initializeInodeTable(BlockSize blockSize, InodeSize inodeSize, InodeCount inodeCount, IDisk &disk);
};

#endif // FSCREATOR_H
