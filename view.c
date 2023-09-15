#include "view.h"

int main(int argc, char ** argv){
    int fileCount;
    if (argc == 1){
        char shmBuffer[BUFFER_SIZE];
        int readCount;
        if (( readCount = read(STDIN_FILENO, shmBuffer, BUFFER_SIZE))==-1){
            perror("Read");
            exit(EXIT_FAILURE);
        }
        shmBuffer[readCount] = 0;
        fileCount = atoi(shmBuffer);
    } else {
        fileCount = atoi(argv[1]);
    }
    sharedMem shm = createSharedMem(SHM_NAME, fileCount * SIZE_PER_FILE);

    char * ptr = NULL;
    while (fileCount > 0){
        semaphoreDown(shm);
        int readCount = readSharedMem(shm, ptr);
        printf("%s", ptr);
        semaphoreUp(shm);
        fileCount -= readCount/SIZE_PER_FILE;
    }

    closeSharedMem(shm);
    return 0;
}