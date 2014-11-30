#ifndef IFILEMANAGER_H
#define IFILEMANAGER_H

#include "bpfstypes.h"


class IFreeBlockManager {
public:
    virtual ~IFreeBlockManager();
    virtual BlockId getFreeBlock() = 0;
    virtual BlockCount freeBlockCount() = 0;
    virtual bool isFull() = 0;
};

#endif // IFILEMANAGER_H
