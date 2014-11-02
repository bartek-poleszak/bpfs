#ifndef DISK_H
#define DISK_H

class IDisk
{
public:
    IDisk();
    virtual ~IDisk();
    virtual void readBlock(int blockNumber, char *buffer) = 0;
    virtual void writeBlock(int blockNumber, char *buffer) = 0;
    virtual int getBlockSize() = 0;
    virtual unsigned long long getBlockNumber() = 0;
};

#endif // DISK_H
