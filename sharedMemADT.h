#ifndef SHAREDMEMADT_H
#define SHAREDMEMADT_H

#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>

struct shm{
    int fd;
    int size;
    void * startAddress;
    sem_t * sem;
    int readOffset;
    int writeOffset;
};

typedef struct shm * sharedMem;

sharedMem createSharedMem(char * name, int size);

void closeSharedMem(sharedMem shm);
void unlinkSharedMem(char name[]);

int readSharedMem(sharedMem shm, char * buf);
void writeSharedMem(sharedMem shm, char * string, int n);

void semaphoreUp(sharedMem shm);
void semaphoreDown(sharedMem shm);
#endif