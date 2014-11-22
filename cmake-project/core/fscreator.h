#ifndef FSCREATOR_H
#define FSCREATOR_H

#include "fspartition.h"
#include "idisk.h"

class FSCreator
{
private:
    void createHeader(IDisk &partition);
    void createINodeTable(IDisk &partition);
public:
    FSCreator(BlockSize blockSize);
    FSPartition createFilesystem(IDisk &partition);
};

#endif // FSCREATOR_H
