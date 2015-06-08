#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "fspartition.h"

using namespace std;

class CommandResolver {
private:
    unsigned long long promptForUnsignedInt(string message);
    void error(std::string message);
public:
    void makeFilesystem(const char *diskPath);
    void makeFilesystem(vector<string> &paths);
    FSPartition *loadFilesystem(IDisk &disk);
    void ok();
};
