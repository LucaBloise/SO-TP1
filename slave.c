#include "slave.h"

int main(){
    FILE* pipeRead;
    char* path;
    char* command;
    char* md5;
    char* result;
    while(read(0, path, PATH_SIZE) > 0){
        sprintf(command, "md5sum %s", path);
        pipeRead = popen(command, "r");
        fread(md5, MD5_SIZE, 1, pipeRead);
        sprintf(result, "%s - %s - %d", path, md5, getpid());
        write(1, result, RESULT_SIZE);
        pclose(pipeRead);
    }
}