#include "commons.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void){
    int fifo = open(FIFO_NAME, O_RDONLY);
    char buf[SIZE_PER_FILE];
    while (read(fifo, buf, SIZE_PER_FILE)>0){
        write(STDOUT_FILENO, buf, SIZE_PER_FILE);
    }
    close(fifo);
    return 0;
}