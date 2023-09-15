#include "slave.h"

int main(){
    char buffer[PIPE_CAP];
    int readCount;
    char * path;
    while((readCount = read(0, buffer, PIPE_CAP)) > 0){
        buffer[readCount]=0;
        path = strtok(buffer,"\n");
        while (path != NULL){
            printMD5(path);
            path = strtok(NULL, "\n");
        }

    }
    if (readCount == -1){
        perror("Read");
        exit(EXIT_FAILURE);
    }
}

void printMD5(const char path[]){
    char md5[MD5_LENGTH+1];
    char command[8 + MAX_PATH_LENGTH];
    sprintf(command ,"md5sum %256s" ,path); //TODO: acordarse de poner en el informe que el maximo path aceptable es de 256
    FILE * md5Pipe = popen(command, "r");
    if (md5Pipe==NULL){
        perror("Pipe");
        exit(EXIT_FAILURE);
    }
    fgets(md5, MD5_LENGTH+1, md5Pipe);
    pclose(md5Pipe);
    printf("PID: %d PATH: %s MD5: %s\n",getpid(), path, md5); //TODO: en el informe, decir que se asume que ningun path tiene el caracter '\n'
}
