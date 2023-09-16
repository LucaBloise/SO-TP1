#include "slave.h"

int main(){

    if (setvbuf(stdout, NULL, _IONBF, 0)!=0){
        perror("Setvbuf");
        exit(EXIT_FAILURE);
    }

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
    return 0;
}

void printMD5(const char path[]){
    char md5[MD5_LENGTH+1];
    char command[8 + MAX_PATH_LENGTH];
    sprintf(command ,"md5sum %s" ,path);
    FILE * md5Pipe = popen(command, "r");
    if (md5Pipe==NULL){
        perror("Pipe");
        exit(EXIT_FAILURE);
    }
    fgets(md5, MD5_LENGTH+1, md5Pipe);
    pclose(md5Pipe);
    printf("PID: %d PATH: %s MD5: %s\n",getpid(), path, md5);
}
