#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int partition = open("/dev/sda2", O_RDONLY);
    if (partition < 0) {
        fprintf(stderr, "Nie udalo sie otworzyc pliku!\n");
        perror(NULL);
        exit(1);
    }
    
    char buff[100];
    ssize_t bytes = read(partition, buff, 100);
    if (bytes < 0) {
        fprintf(stderr, "Nie udalo sie odczytac!\n");
        perror(NULL);
        exit(2);
    }
    
    puts(buff);
    
    close(partition);
}
