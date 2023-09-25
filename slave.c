// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "slave.h"

int main(){
    if (setvbuf(stdout, NULL, _IONBF, 0)!=0){
        PERROR_EXIT("Setvbuf")
    }

    int namedPipeFd = open(FIFO_NAME, O_WRONLY);

    char buffer[PIPE_CAP+1];
    int readCount;
    char path[MAX_PATH_LENGTH];
    while((readCount = read(0, buffer, PIPE_CAP)) > 0){
        buffer[readCount]=0;
        int j=0;
        for(int i = 0; buffer[i] && j<MAX_PATH_LENGTH;i++){
            if (buffer[i]=='\n'){
                path[j]=0;
                printMD5(path, namedPipeFd);
                j=0;
            } else{
                path[j++]=buffer[i];
            }
        }
        if(j==MAX_PATH_LENGTH){
            fprintf(stderr, "Unsoported file lenght\n");
            exit(EXIT_FAILURE);
        }

    }
    close(namedPipeFd);
    if (readCount == -1){
        PERROR_EXIT("Read")
    }

    return 0;
}

void printMD5(char path[], int fd){
    int pipefd[2];
    if (pipe(pipefd)==-1){
        PERROR_EXIT("Pipe")
    }
    int pid = fork();
    if (pid ==-1){
        PERROR_EXIT("Fork")
    }
    if (pid==0){
        close(pipefd[PIPE_READ_END]);
        close(STDOUT_FILENO);
        dup(pipefd[PIPE_WRITE_END]);
        close(pipefd[PIPE_WRITE_END]);
        char * argv[] = {"md5sum", path, NULL};
        execv(MD5SUM_PATH, argv);
        PERROR_EXIT("Execv")
    } else{
        close(pipefd[PIPE_WRITE_END]);
        wait(NULL);
        int myPid = getpid();
        while (digits(myPid) > MAX_PID_LENGTH){
            myPid /= 10;
        }
        char md5[MD5_LENGTH+1];
        read(pipefd[PIPE_READ_END],md5, MD5_LENGTH);
        close(pipefd[PIPE_READ_END]);
        md5[MD5_LENGTH]=0;
        printf("PID: %d PATH: %s MD5: %s\n", myPid, path, md5);
        write(fd, md5, MD5_LENGTH);
    }
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