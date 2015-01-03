#include "utils.h"

Utils::Utils()
{
}

string Utils::getFileNameFromPath(const char *path) {
    string stringPath(path);
    return getFileNameFromPath(stringPath);
}

string Utils::getFileNameFromPath(string &path) {
    auto slashPosition = path.find_last_of('/');
    if (slashPosition == string::npos)
        throw WrongPathException();
    return path.substr(slashPosition + 1);
}
