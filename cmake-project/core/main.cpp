#include <iostream>
#include "filedisk.h"
#include "idisk.h"
#include "partitionheader.h"

using namespace std;

int main()
{
    FileDisk disk("/dev/sda2", 1024);
    PartitionHeader header;
    header.writeToDisk(disk);
    return 0;
}

