#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include "commons.h"
#include "sharedMemADT.h"

#define SLAVE_TO_FILE_RATIO 10
#define INITIAL_FILES_PER_SLAVE_RATIO 3


typedef struct{
    int talkPipe[2];
    int hearPipe[2];
    int pendingFileCount;
}slaveInfo;

void startSlaves(slaveInfo slavesInfo[], int slaveCount);
void sendFile(slaveInfo * slave, char * paths[], int *pathOffset);
void initialFileSend(slaveInfo slavesInfo[], int slaveCount, char * paths[], int * pathOffset);
void slaveManager(slaveInfo slavesInfo[], int slaveCount,char * paths[], int fileCount, FILE * results, sharedMem shm);
void save(char fileInfo[],FILE * results, sharedMem shm);
void closeAll(slaveInfo slavesInfo[], int slaveCount, FILE * results, sharedMem shm);

#endif