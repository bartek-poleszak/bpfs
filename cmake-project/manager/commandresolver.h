#pragma once

#include <string>
#include <iostream>
#include "fspartition.h"

using namespace std;

class CommandResolver {
private:
    unsigned long long promptForUnsignedInt(string message);
    void error(std::string message);
public:
    void makeFilesystem(IDisk &disk);
    FSPartition *loadFilesystem(IDisk &disk);
    void ok();
};
