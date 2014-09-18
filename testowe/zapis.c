#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int partition = open("/dev/sdb1", O_WRONLY | O_SYNC);
    if (partition < 0) {
        fprintf(stderr, "Nie udalo sie otworzyc pliku!\n");
        perror(NULL);
        exit(1);
    }
    
    lseek(partition, 1, SEEK_SET);
    char text[] = "iop";
    ssize_t bytes = write(partition, text, 3);
    if (bytes < 0) {
        fprintf(stderr, "Nie udalo sie zapisac!\n");
        perror(NULL);
        exit(2);
    }
    
    close(partition);
}
