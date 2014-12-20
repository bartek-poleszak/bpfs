#ifndef LOG_H
#define LOG_H

#include <iostream>

class Log
{
private:
    Log();
public:
    static std::ostream &stream;
};

#endif // LOG_H
