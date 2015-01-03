#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <exception>

using namespace std;

class WrongPathException : public exception {};

class Utils
{
public:
    Utils();
    static string getFileNameFromPath(string &path);
    static string getFileNameFromPath(const char *path);
};

#endif // UTILS_H
