#ifndef FSPARTITION_H
#define FSPARTITION_H

#include "idisk.h"

#include <string>

class FSPartition
{
private:
    IDisk *disk;
public:

    FSPartition(IDisk *disk);
};

#endif // FSPARTITION_H
