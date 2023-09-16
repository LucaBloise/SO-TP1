#include "view.h"
#include <sys/wait.h>
int main(int argc, char ** argv){
    /*if (setvbuf(stdout, NULL, _IONBF, 0)!=0){
        perror("Setvbuf");
        exit(EXIT_FAILURE);
    }
    if (setvbuf(stderr, NULL, _IONBF, 0)!=0){
        perror("Setvbuf");
        exit(EXIT_FAILURE);
    }*/
    //unlinkSharedMem(SHM_NAME);
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

    char ptr[fileCount*SIZE_PER_FILE];
    while (fileCount > 0){ //TODO ver el codigo de esto
        //semaphoreDown(shm);
        int readCount = readSharedMem(shm, ptr);
        ptr[readCount]=0;
        printf("%s", ptr);
       // semaphoreUp(shm);
        for(int i = 0; i<readCount; i++){
            if (ptr[i]=='\n'){
                fileCount--;
            }
        }
    }

    closeSharedMem(shm);

    return 0;
}