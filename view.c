// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "view.h"

int main(int argc, char ** argv){

    int fileCount;
    if (argc == 1){
        char shmBuffer[BUFFER_SIZE+1];
        int readCount;
        if (( readCount = read(STDIN_FILENO, shmBuffer, BUFFER_SIZE))==-1){
            PERROR_EXIT("Read")
        }
        shmBuffer[readCount] = 0;
        fileCount = atoi(shmBuffer);
    } else {
        fileCount = atoi(argv[1]);
    }
    sharedMem shm = createSharedMem(SHM_NAME, fileCount * SIZE_PER_FILE);

    char ptr[SIZE_PER_FILE];
    while (fileCount > 0){
        semaphoreDown(shm);
        readSharedMem(shm, ptr, SIZE_PER_FILE);
        write(STDOUT_FILENO, ptr, SIZE_PER_FILE);
        fileCount--;
    }

    closeSharedMem(shm);
    return 0;
}