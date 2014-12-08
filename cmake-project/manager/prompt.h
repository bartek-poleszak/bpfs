#ifndef PROMPT_H
#define PROMPT_H

#include <string>
#include <deque>
#include "directory.h"

using namespace std;

class Prompt
{
private:
    string devicePath;
    string renderCurrentDirectoryPath(deque<Directory *> directories);
public:
    Prompt();
    void setDevicePath(const char *path);
    string render(deque<Directory *> directories);
};

#endif // PROMPT_H
