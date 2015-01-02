#include "rawdatautils.h"

RawDataUtils::RawDataUtils()
{

}

//#include <iostream>
//using namespace std;

void RawDataUtils::writeUintToBuffer(unsigned long long value, char *buffer, int offset, int bytes)
{
    for (int i = 0; i < bytes; ++i) {
        char val = value % 256;
        buffer[offset + i] = val;
        value = value >> 8;
    }
}

unsigned long long RawDataUtils::readUintFromBuffer(char *buffer, int offset, int bytes)
{
    unsigned long long result = 0;
    unsigned long long tmp;
    for (int i = 0; i < bytes; i++) {
        tmp = (unsigned char)buffer[offset + i];
        tmp = tmp << 8 * i;
        result += tmp;
    }

    return result;
}
