#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "blockDevice.h"

///
/// \brief main
/// \param argc
/// \param argv [1] - block device path
///             [2] - block size in bytes
/// \return
///

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Too few arguments!");
        exit(1);
    }
    int blockSize = atoi(argv[2]);
    struct BlockDevice partition = openBlockDevice(argv[1]);
    close(partition.descriptor);
    return 0;
}

