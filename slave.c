// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "slave.h"

int main(){
    if (setvbuf(stdout, NULL, _IONBF, 0)!=0){
        PERROR_EXIT("Setvbuf");
    }

    char buffer[PIPE_CAP+1];
    int readCount;
    char path[MAX_PATH_LENGTH];
    while((readCount = read(0, buffer, PIPE_CAP)) > 0){
        buffer[readCount]=0;

        int j=0;
        for(int i = 0; buffer[i] && j<MAX_PATH_LENGTH; i++){
            if (buffer[i]=='\n'){
                path[j]=0;
                printMD5(path);
                j=0;
            } else {
                if (buffer[i] == ' ') {
                    path[j++] = '\\';
                    if (j==MAX_PATH_LENGTH){
                        break;
                    }
                }

                path[j++] = buffer[i];
            }
        }
        if(j==MAX_PATH_LENGTH){
            fprintf(stderr, "Unsoported file lenght\n");
            exit(EXIT_FAILURE);
        }

    }
    if (readCount == -1){
        PERROR_EXIT("Read");
    }

    return 0;
}

void printMD5(const char path[]){
    char md5[MD5_LENGTH+1];
    char command[strlen(CMD_NAME) + MAX_PATH_LENGTH + 1];
    sprintf(command ,"md5sum %s" ,path);
    FILE * md5Pipe = popen(command, "r");
    if (md5Pipe==NULL){
        PERROR_EXIT("Pipe");
    }
    fgets(md5, MD5_LENGTH+1, md5Pipe);
    pclose(md5Pipe);
    int pid = getpid();
    while (digits(pid) > MAX_PID_LENGTH){
        pid /= 10;
    }
    printf("PID: %d PATH: %s MD5: %s\n",pid, path, md5);
}

int digits(int n){
    if (n==0){
        return 1;
    }
    int count = 0;
    while (n != 0) {
        n /= 10;
        ++count;
    }
    return count;
}