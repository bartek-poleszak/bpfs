#ifndef MAIN_H
#define MAIN_H

#include <deque>
#include "filedisk.h"
#include "idisk.h"
#include "partitionheader.h"
#include "rawdatautils.h"
#include "menuoptions.h"
#include "prompt.h"
#include "commandresolver.h"
#include "directory.h"

class ConsoleInterface {
private:
    deque<Directory *> &directories;
    Prompt prompt;
    CommandResolver commandResolver;
    FSPartition *partition = nullptr;
    void put();
    void list();
    string resolveFileName(string &fullPath);
    void printFileToStdOut();
public:
    ConsoleInterface(deque<Directory *> &directories);
    ~ConsoleInterface();
    void main(const char *diskPath);
    void initializeFilesystem(IDisk &disk);
    Directory *currentDirectory();
    void unknownCommand();
    void makeMatrix(const char *diskPath);
};

#endif // MAIN_H
