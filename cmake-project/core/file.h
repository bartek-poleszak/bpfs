#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <exception>
#include <string>
#include <vector>

#include "inode.h"
#include "fspartition.h"

class BlockIndexOutOfRangeException : public std::exception {};
class TooManyDataForBlockException : public std::exception {};
class NoneDataForBlockException : public std::exception {};
class FileCutException : public std::exception {};

class File;

class FileBuffer
{
private:
    static const BlockCount INVALID_POSITION = -1;
    File *file;
    BlockSize blockSize;
    int currentPositionInBuffer;
    BlockCount nextBlock;
//    BlockCount currentBlock;
    int64_t currentBlockSize;
    char *innerBuffer;
    BytePointer nextByte();
    void loadNextBlock();
    void loadBlock(BlockCount blockId);
protected:
    uint64_t iterateStream(const char *buffer, uint64_t length);
    uint64_t iterateStream(const char *buffer, uint64_t length, uint64_t offset);
    virtual void onByteRecieved(BytePointer bytePtr, const char *positionInBuffer) = 0;
    virtual void onBlockManipulationCompleted(char *innerBuffer, BlockCount blockSequenceNumber) = 0;
public:
    File *getFile();
    static const int END_OF_FILE = -1000;
    FileBuffer(File *file, BlockSize blockSize);
    virtual ~FileBuffer();
};

class FileReadBuffer : public FileBuffer
{
protected:
    void onByteRecieved(BytePointer bytePtr, const char *positionInBuffer) override;
    void onBlockManipulationCompleted(char *innerBuffer, BlockCount blockSequenceNumber) override {
        (void)innerBuffer;
        (void)blockSequenceNumber;
    }
public:
    FileReadBuffer(File *file, BlockSize blockSize) : FileBuffer(file, blockSize) {}
    ~FileReadBuffer() override {}
    uint64_t read(char *buffer, uint64_t length);
    uint64_t read(char *buffer, uint64_t length, uint64_t offset);
};

class FileWriteBuffer : public FileBuffer
{
protected:
    void onByteRecieved(BytePointer bytePtr, const char *positionInBuffer) override;
    void onBlockManipulationCompleted(char *innerBuffer, BlockCount blockSequenceNumber) override;
public:
    FileWriteBuffer(File *file, BlockSize blockSize) : FileBuffer(file, blockSize) {}
    ~FileWriteBuffer() override {}
    uint64_t write(char *buffer, uint64_t length);
    uint64_t write(const char *buffer, uint64_t length, uint64_t offset);
};

class BlockIdCache {
private:
    FSPartition *partition;
    Inode *inode;
    std::vector<IIndirectBlock *> cache;
    IIndirectBlock *getIndirectBlock(unsigned index);
    void cacheBlocksTo(unsigned index);
    bool isCached(unsigned index);
    unsigned calculateIndex(BlockCount offset);
    BlockCount calculatePosition(BlockCount offset);
public:
    BlockIdCache(FSPartition *partition, Inode *inode);
    ~BlockIdCache();
    BlockId getBlockId(BlockCount offset);
    void addBlock(BlockId blockId);
    void initializeNewIndirectBlock(BlockId blockId, IIndirectBlock *lastIndirectBlock);
    void appendIndirectBlock(int lastBlockIdPosition, BlockId blockId, IIndirectBlock *lastIndirectBlock);
};

class File
{
private:
    std::string name;
    Inode *inode;
    FSPartition *fsPartition;
    void rewriteBlock(BlockCount index, char *buffer);
    BlockSize appendLastBlock(const char *buffer, uint64_t significantBytes);
    FileReadBuffer *fileReadBuffer;
    FileWriteBuffer *fileWriteBuffer;
    BlockIdCache *blockIdCache;
    uint64_t cutFromLastBlock(uint64_t bytesToCut);
    void markBlocksAsFree();
public:
    File(std::string name, Inode *inode, FSPartition *fsPartition);
    File();
    ~File();
    uint64_t read(char *buffer, uint64_t length);
    uint64_t read(char *buffer, uint64_t length, uint64_t offset);
    BlockSize readBlock(BlockCount index, char *buffer);
    void writeBlock(BlockCount index, char *buffer);
    bool isDirectory();
    void appendByBlock(const char *buffer, BlockSize significantBytes);
    void append(const char *buffer, uint64_t significantBytes);
    void write(std::ifstream &fileStream);
    uint64_t write(const char *buffer, uint64_t length, uint64_t offset);
    void get(std::ofstream &fileStream);
    Inode *getInode();
    std::string &getName();
    uint64_t getTotalSizeInBytes();
    void truncate(uint64_t size);
    void appendWithZeroes(uint64_t size);
    void cutToSize(uint64_t size);
    BlockSize cutByBlock();
    void initializeBlockIdCacheIfNeeded();
    void unlink();
    uint64_t rewrite(const char *buffer, uint64_t length, uint64_t offset);
};


#endif // FILE_H
