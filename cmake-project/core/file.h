#ifndef FILE_H
#define FILE_H

#include <inode.h>
#include <fstream>
#include <exception>
#include "fspartition.h"

class BlockIndexOutOfRangeException : public std::exception {};
class TooManyDataForBlockException : public std::exception {};
class NoneDataForBlockException : public std::exception {};

class File
{
private:
    Inode *inode;
    FSPartition *fsPartition;
    void rewriteBlock(BlockCount index, char *buffer);
    BlockSize appendLastBlock(char *buffer, uint64_t significantBytes);
public:
    File(Inode *inode, FSPartition *fsPartition);
    ~File();
    void readBlock(BlockCount index, char *buffer);
    void writeBlock(BlockCount index, char *buffer);
    void appendByBlock(char *buffer, BlockSize significantBytes);
    void append(char *buffer, uint64_t significantBytes);
    void write(std::ifstream &fileStream);
    void get(std::ofstream &fileStream);

};


#endif // FILE_H
