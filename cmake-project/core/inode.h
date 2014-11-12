#ifndef INODE_H
#define INODE_H

#include <exception>
#include <cstdint>
#include "permissions.h"

class WrongINodeSizeException : public std::exception {};

class INode
{
private:
    unsigned size;
    static const unsigned BLOCK_ADRESS_SIZE = 8;

    static const unsigned TYPE_AND_PERMISSIONS_OFFSET = 0; //uint16_t
    static const unsigned HARD_LINK_COUNT_OFFSET = 2; //uint8_t
    //...
    static const unsigned FIRST_DATA_BLOCK_OFFSET = 24; //uint64_t

    unsigned dataBlockCount;
    Permissions *permissions;
    uint8_t hardLinkCount;

    uint64_t *dataBlocks;
public:
    static const unsigned MIN_SIZE = 32;
    INode(unsigned size);
    ~INode();
    void readFromBuffer(char *buffer, unsigned position);
    void writeToBuffer(char *buffer, unsigned position);
    unsigned getDataBlockOffset(unsigned blockNumber);
};

#endif // INODE_H
