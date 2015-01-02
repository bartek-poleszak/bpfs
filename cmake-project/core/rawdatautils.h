#ifndef RAWDATAUTILS_H
#define RAWDATAUTILS_H

class RawDataUtils
{
public:
    RawDataUtils();
    static void writeUintToBuffer(unsigned long long value, char *buffer, int offset, int bytes);
    static unsigned long long readUintFromBuffer(char *buffer, int offset, int bytes);
};

#endif // RAWDATAUTILS_H
