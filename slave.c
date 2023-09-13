#include "slave.h"

int main(){
    FILE* pipeRead;
    char path[PATH_SIZE];
    char command[2*PATH_SIZE];
    char md5[MD5_SIZE];
    char result[RESULT_SIZE];
    
    while(read(0, path, PATH_SIZE) > 0){
        sprintf(command, "md5sum %s", path);
        pipeRead = popen(command, "r");
        fread(md5, MD5_SIZE, 1, pipeRead);
        sprintf(result, "%s - %s - %d\n", path, md5, getpid());
        write(1, result, strlen(result));
        pclose(pipeRead);
    }
}