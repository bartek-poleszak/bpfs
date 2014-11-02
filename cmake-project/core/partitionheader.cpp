#include "partitionheader.h"

std::string const PartitionHeader::IDENTIFIER = "BPFS";

PartitionHeader::PartitionHeader()
{
}


void PartitionHeader::writeUintToBuffer(uint32_t value, char *buffer, int position)
{

}

void PartitionHeader::writeToDisk(IDisk &disk)
{
    char buffer[SIZE] = {0};
    int i;
    for (i = 0; i < IDENTIFIER.size(); ++i) {
        buffer[i] = IDENTIFIER[i];
    }

    writeUintToBuffer(blockSize, buffer, i);
    i += 4;
    writeUintToBuffer(blockNumber, buffer, i);
    i += 4;


}
