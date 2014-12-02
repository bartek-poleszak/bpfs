#ifndef FILE_H
#define FILE_H

#include <inode.h>
#include <fstream>
#include <exception>
#include <string>
#include "fspartition.h"

class BlockIndexOutOfRangeException : public std::exception {};
class TooManyDataForBlockException : public std::exception {};
class NoneDataForBlockException : public std::exception {};

class File
{
private:
    std::string name;
    Inode *inode;
    FSPartition *fsPartition;
    void rewriteBlock(BlockCount index, char *buffer);
    BlockSize appendLastBlock(char *buffer, uint64_t significantBytes);
public:
    File(std::string name, Inode *inode, FSPartition *fsPartition);
    ~File();
    void read(char *buffer, uint64_t offset, uint64_t length);
    void readBlock(BlockCount index, char *buffer);
    void writeBlock(BlockCount index, char *buffer);
    bool isDirectory();
    void appendByBlock(char *buffer, BlockSize significantBytes);
    void append(char *buffer, uint64_t significantBytes);
    void write(std::ifstream &fileStream);
    void get(std::ofstream &fileStream);
};


#endif // FILE_H
