#include "commons.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void){
    int fifo = open(FIFO_NAME, O_RDONLY);
    char buf[MD5_LENGTH];
    while (read(fifo, buf, MD5_LENGTH)>0){
        write(STDOUT_FILENO, buf, MD5_LENGTH);
    }
    close(fifo);
    return 0;
}