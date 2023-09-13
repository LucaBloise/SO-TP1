#include "application.h"

int fds[FDS_ARRAY_SIZE];
int filesIndex = 1;
int savedFiles = 1;
int slavesInitialLoadout[SLAVES] = {[0 ... (SLAVES - 1)] = INITIAL_LOADOUT};

int main(int argc, char* argv[]){

    createSlaves(SLAVES);

    for(int i = 0; i < SLAVES; i++){
        sendFiles(INITIAL_LOADOUT, i, argv);
    }

    slaveManager(argc, argv);
    
    closeAll();

}


void createSlaves(int slavesAmount){
    char *args[] = {"./slave.c", NULL};
    for(int i = 0; i < slavesAmount; i++){
        createPipe(fds + (SLAVES - 1)*i);
        createPipe(fds + ((SLAVES - 1)*i + 2));
        if(fork() != 0){
            close(fds[(SLAVES - 1)*i + 1]);
            close(fds[(SLAVES - 1)*i + 3]);
        }
        else {
            dup2(fds[(SLAVES - 1)*i + 3], 0);
            dup2(fds[(SLAVES - 1)*i + 1], 1);
            closeAll();
            execve("./slave.c", args, NULL);
        }
    }
}

void createPipe(int* fds){
    if(pipe(fds) == -1){
        perror("pipe");
        exit(1);
    }
}

void sendFiles(int filesAmount, int slaveIndex, char* files[]){
    for(int i = 0; i < filesAmount && files[filesIndex] != NULL; i++){
        char* path = files[filesIndex++];
        write(fds[(SLAVES - 1)*slaveIndex + 2], path, strlen(path));
    }
}

void slaveManager(int argc, char* argv[]){
    fd_set rfds;
    char result[RESULT_SIZE];
    FILE* resultsFile;

    resultsFile = fopen("results.txt", "w");

    while(savedFiles < argc){
        FD_ZERO(&rfds);
        for(int i = 0; i < SLAVES; i++){
            FD_SET(fds[(SLAVES - 1)*i], &rfds);
        }
        select(fds[4*SLAVES - 1] + 1, &rfds, NULL, NULL, NULL);
        for(int i = 0; i < SLAVES; i++){
            if(FD_ISSET(fds[(SLAVES - 1)*i], &rfds)){
                if(slavesInitialLoadout[i] > 1){
                    slavesInitialLoadout[i]--;
                } 
                else {
                    sendFiles(1, i, argv);
                }
                read(fds[(SLAVES - 1)*i], result, RESULT_SIZE); //Por alguna razon no lee nada.
                fprintf(resultsFile, "%s", result);
                savedFiles++;
            }
        }
    }

    fclose(resultsFile);
}

void closeAll(){
    for(int i = 0; i < FDS_ARRAY_SIZE; i++){
        close(fds[i]);
    }
}