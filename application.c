// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "application.h"

int main(int argc, char *argv[]){

    unlinkSharedMem(SHM_NAME);

    if (argc < 2){
        fprintf(stderr, "Not enough arguments\n");
        exit(EXIT_FAILURE);
    }

    if (setvbuf(stdout, NULL, _IONBF, 0)!=0){
        PERROR_EXIT("Setvbuf");
    }

    FILE * results = fopen("results.txt", "w");
    int fileCount = argc-1;
    printf("%d", fileCount);
    sleep(2);
    sharedMem shm = createSharedMem(SHM_NAME, fileCount*SIZE_PER_FILE);

    int slaveCount = fileCount/SLAVE_TO_FILE_RATIO + 1;
    slaveInfo slavesInfo[slaveCount];
    slaveManager(slavesInfo, slaveCount, argv, fileCount, results, shm);

    closeAll(slavesInfo, slaveCount, results, shm);
    return 0;
}

void startSlaves(slaveInfo slavesInfo[], int slaveCount){
    char * argv[] = {"slave", NULL};

    for(int i = 0; i<slaveCount; i++){
        slavesInfo[i].pendingFileCount=0;
        if (pipe(slavesInfo[i].hearPipe)==-1 || pipe(slavesInfo[i].talkPipe)==-1){
            PERROR_EXIT("Pipe");
        }

        int pid = fork();
        if (pid<0){
            PERROR_EXIT("Fork");
        }
        if (pid==0){

            close(slavesInfo[i].hearPipe[PIPE_READ_END]);
            close(slavesInfo[i].talkPipe[PIPE_WRITE_END]);

            close(STDIN_FILENO);
            dup(slavesInfo[i].talkPipe[PIPE_READ_END]);
            close(slavesInfo[i].talkPipe[PIPE_READ_END]);

            close(STDOUT_FILENO);
            dup(slavesInfo[i].hearPipe[PIPE_WRITE_END]);
            close(slavesInfo[i].hearPipe[PIPE_WRITE_END]);

            execv("slave", argv);
            PERROR_EXIT("Execve");
        } else {
            close(slavesInfo[i].talkPipe[PIPE_READ_END]);
            close(slavesInfo[i].hearPipe[PIPE_WRITE_END]);
        }
    }
}

void sendFile(slaveInfo * slave, char * paths[], int *pathOffset){
    int len = strlen(paths[*pathOffset]);
    char toWrite[len+1];
    for(int i = 0; i<len; i++){
        toWrite[i] = paths[*pathOffset][i];
    }
    toWrite[len] = '\n';
    if ((write(slave->talkPipe[PIPE_WRITE_END], toWrite, len+1))==-1){
        PERROR_EXIT("Write");
    }
    (*pathOffset)++;
    slave->pendingFileCount++;
}

void initialFileSend(slaveInfo slavesInfo[], int slaveCount, char * paths[], int * pathOffset){
    int initialFileCount = slaveCount/INITIAL_FILES_PER_SLAVE_RATIO + 1;
    for (int i=0; i<slaveCount; i++){
        for (int j=0; j<initialFileCount; j++){
            sendFile(&slavesInfo[i], paths, pathOffset);

        }
    }
}

void slaveManager(slaveInfo slavesInfo[], int slaveCount, char * paths[], int fileCount,FILE * results, sharedMem shm){
    startSlaves(slavesInfo, slaveCount);
    int pathOffset = 1;
    initialFileSend(slavesInfo, slaveCount, paths, &pathOffset);
    fd_set rfds;
    int highestFd = -1;
    int availableCount;
    char readBuffer[PIPE_CAP+1];
    long readCount;
    int remainingFiles = fileCount;

    while(remainingFiles>0) {
        FD_ZERO(&rfds);
        for (int i = 0; i < slaveCount; i++) {
            FD_SET(slavesInfo[i].hearPipe[PIPE_READ_END], &rfds);
            if (slavesInfo[i].hearPipe[PIPE_READ_END] > highestFd){
                highestFd = slavesInfo[i].hearPipe[PIPE_READ_END];
            }
        }
        if ((availableCount = select(highestFd + 1, &rfds, NULL, NULL, NULL)) == -1){
            PERROR_EXIT("Select");
        }
        for(int i=0; i<slaveCount && availableCount>0; i++){

            if (FD_ISSET(slavesInfo[i].hearPipe[PIPE_READ_END],&rfds)){
                availableCount--;
                if ((readCount = read(slavesInfo[i].hearPipe[PIPE_READ_END], readBuffer, PIPE_CAP))==-1){
                    PERROR_EXIT("read");
                }
                readBuffer[readCount]=0;
                for(int j=0; j<readCount;j++){
                    if (readBuffer[j]=='\n') {
                        slavesInfo[i].pendingFileCount--;
                        remainingFiles--;
                        save(readBuffer, results, shm);
                    }
                }
                if (slavesInfo[i].pendingFileCount==0 && pathOffset<=fileCount){
                    sendFile(&slavesInfo[i], paths, &pathOffset);
                }

            }
        }
    }
}

void save(char fileInfo[], FILE * results,sharedMem shm){
    fprintf(results, "%s", fileInfo);
    semaphoreDown(shm);
    writeSharedMem(shm, fileInfo, strlen(fileInfo));
    semaphoreUp(shm);
}

void closeAll(slaveInfo slavesInfo[], int slaveCount, FILE * results, sharedMem shm){
    for(int i = 0; i< slaveCount; i++){
        close(slavesInfo[i].hearPipe[PIPE_READ_END]);
        close(slavesInfo[i].talkPipe[PIPE_WRITE_END]);
    }
    fclose(results);
    closeSharedMem(shm);
    unlinkSharedMem(SHM_NAME);
}

