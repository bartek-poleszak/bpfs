#ifndef FILE_H
#define FILE_H

#include <inode.h>
#include <fstream>
#include "fspartition.h"

class File
{
private:
    INode *inode;
    FSPartition *fsPartition;
public:
    File(INode *inode, FSPartition *fsPartition);
    ~File();

    void write(std::ifstream &fileStream);
    void get(std::ofstream &fileStream);
};

#endif // FILE_H
