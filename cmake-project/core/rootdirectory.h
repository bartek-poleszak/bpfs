#ifndef ROOTDIRECTORY_H
#define ROOTDIRECTORY_H

#include "directory.h"

class RootDirectory : public Directory
{
public:
    RootDirectory(File *file, FSPartition *partition);
    ~RootDirectory() override;
};

#endif // ROOTDIRECTORY_H
