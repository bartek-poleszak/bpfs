#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>
#include <map>
#include <exception>
#include <vector>
#include "file.h"

using namespace std;

class FileDosentExistException : public exception {};
class FileNameTooLongException : public exception {};
class ReadingDirectoryException : public exception {};

class Directory
{
private:
    FSPartition *partition;
    map<string, InodeId> entries;
//    void flush();
    Inode *getInode(const string &fileName);
    unsigned createRawEntry(char *buffer, string &fileName);
    static const unsigned ENTRY_LENGTH_OFFSET = 0;
    static const unsigned INODE_ID_OFFSET = 1;
    static const unsigned FILE_NAME_OFFSET = 5;
    static const unsigned MAX_ENTRY_SIZE = 256;
protected:
    File *file;
public:
    static const unsigned MAX_FILE_NAME_LENGTH = 251;
    static Directory *rootOf(FSPartition *partition);
    Directory(File *file, FSPartition *partition);
    virtual ~Directory();
    void link(string &fileName, Inode *inode);
    void link(File &file);
    void unlink(string &fileName);
    File getFile(string &fileName);
    vector<File> getFileList();
    unsigned getFileCount();
    void readEntryFromBuffer(uint8_t entryLength, File *file, char buffer);
    void readEntryFromBuffer(char *buffer, uint8_t stringLength);
};

#endif // DIRECTORY_H
