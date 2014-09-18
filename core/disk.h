#ifndef DISK_H
#define DISK_H

class Disk
{
public:
    virtual ~Disk();
    virtual int readBlock(char *buffer) = 0;
    virtual int writeBlock(char *buffer) = 0;
};

#endif // DISK_H
