#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>
#include <map>
#include <exception>
#include "file.h"

using namespace std;

class FileDosentExistException : public exception {};

class Directory
{
private:
    FSPartition *partition;
    File *file;
    map<string, InodeId> entries;
    void flush();
    Inode *getInode(string fileName);
public:
    Directory(File *file, FSPartition *partition);
    void link(string fileName, Inode *inode);
    void unlink(string fileName);
    File getFile(string fileName);
};

#endif // DIRECTORY_H
