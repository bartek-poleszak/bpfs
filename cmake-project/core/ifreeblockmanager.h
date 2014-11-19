#ifndef IFILEMANAGER_H
#define IFILEMANAGER_H

#include <cstdint>
#include <stdint.h>

class IFreeBlockManager {
public:
    virtual ~IFreeBlockManager();
    virtual uint64_t getFreeBlock() = 0;
    virtual uint64_t freeBlockCount() = 0;
    virtual bool isFull() = 0;
};

#endif // IFILEMANAGER_H
