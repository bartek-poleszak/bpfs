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

class File;

class FileReadBuffer
{
private:
    File *file;
    BlockSize blockSize;
    int currentPositionInBuffer;
    BlockCount currentBlock;
    int64_t currentBlockSize;
    char *innerBuffer;
    int nextByte();
    void loadNextBlock();
public:
    static const int END_OF_FILE = -1000;
    FileReadBuffer(File *file, BlockSize blockSize);
    ~FileReadBuffer();
    uint64_t read(char *innerBuffer, uint64_t length);
};


class File
{
private:
    std::string name;
    Inode *inode;
    FSPartition *fsPartition;
    void rewriteBlock(BlockCount index, char *buffer);
    BlockSize appendLastBlock(char *buffer, uint64_t significantBytes);
    FileReadBuffer *fileReadBuffer;
public:
    File(std::string name, Inode *inode, FSPartition *fsPartition);
    ~File();
    uint64_t read(char *buffer, uint64_t length);
    BlockSize readBlock(BlockCount index, char *buffer);
    void writeBlock(BlockCount index, char *buffer);
    bool isDirectory();
    void appendByBlock(char *buffer, BlockSize significantBytes);
    void append(char *buffer, uint64_t significantBytes);
    void write(std::ifstream &fileStream);
    void get(std::ofstream &fileStream);
    Inode *getInode();
    std::string &getName();
};


#endif // FILE_H
