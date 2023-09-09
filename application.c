#include "application.h"

int fds[FDS_ARRAY_SIZE];

int main(int argc, char* argv[]){

    createSlaves(SLAVES);

}


void createSlaves(int slavesAmount){
    for(int i = 0; i < slavesAmount, i++){
        createPipe(fds + (SLAVES - 1)*i);
        createPipe(fds + ((SLAVES - 1)*i + 2));
        if(fork() != 0){
            close(fds[(SLAVES - 1)*i + 1]);
            close(fds[(SLAVES - 1)*i + 3])
        }
        else {
            slaveClose(i);
            execve("./slave.c", NULL, NULL);
        }
    }
}

void createPipe(*int fds){
    if(pipe(fds) == -1){
        perror("pipe");
        exit(1);
    }
}

void slaveClose(int slaveIndex){
    for(int i = 0; i < FDS_ARRAY_SIZE; i++){
        if(i != (SLAVES - 1)*slaveIndex + 1 && i != (SLAVES - 1)*slaveIndex + 3){
            close(fds[i]);
        }
    }
}