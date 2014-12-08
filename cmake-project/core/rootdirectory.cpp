#include "rootdirectory.h"

RootDirectory::RootDirectory(File *file, FSPartition *partition)
    : Directory(file, partition)
{
}

RootDirectory::~RootDirectory()
{
    delete file;
}
