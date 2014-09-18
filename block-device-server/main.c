#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

typedef int fileDescriptor;

///
/// \brief main
/// \param argc
/// \param argv [1] - block device path
///             [2] - block size in bytes
/// \return
///

int main(int argc, char *argv[])
{
    fileDescriptor partition = openBlockDevice(argv[1]);
    int blockSize = atoi(argv[2]);
    close(partition);
    return 0;
}

///
/// \brief openBlockDevice
/// \param path
/// \return file descriptor if file is opened, terminates if error occured
///

fileDescriptor openBlockDevice(char *path) {
    fileDescriptor result = open(path, O_RDWR);
    if (result < 0) {
        perror(NULL);
        exit(0);
    }
    return result;
}

