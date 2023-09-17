// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "sharedMemADT.h"

sharedMem createSharedMem(char * name, int size){
    sharedMem shm = malloc(sizeof(struct shm));
    if (shm==NULL){
        PERROR_EXIT("Malloc");
    }
    shm->size = size;
    shm->readOffset = 0;
    shm->writeOffset = 0;
    if ((shm->fd = shm_open(name, O_CREAT | O_RDWR,  S_IRUSR | S_IWUSR))==-1){
        PERROR_EXIT("Shm_open");
    }
    if (ftruncate(shm->fd, shm->size)==-1){
        PERROR_EXIT("Ftruncate");
    }
    if ((shm->startAddress = mmap(NULL, shm->size, PROT_WRITE | PROT_READ, MAP_SHARED, shm->fd, 0))==MAP_FAILED){
        PERROR_EXIT("Mmap");
    }
    if ((shm->sem = sem_open(name, O_CREAT, S_IRUSR|S_IWUSR, 1))==SEM_FAILED){
        PERROR_EXIT("Sem_open");
    }
    return shm;
}


int readSharedMem(sharedMem shm, char * ptr){
    strcpy(ptr, shm->startAddress + shm->readOffset );
    int readCount = strlen(ptr);
    shm->readOffset += readCount;
    return readCount;
}

void writeSharedMem(sharedMem shm, char * string){
    int n = strlen(string);
    memcpy(shm->startAddress+shm->writeOffset, string, n+1);
    shm->writeOffset += n;
}

void closeSharedMem(sharedMem shm) {
    if (munmap(shm->startAddress, shm->size) == -1) {
        PERROR_EXIT("Munmap");
    }
    close(shm->fd);
    sem_close(shm->sem);
    free(shm);
}

void unlinkSharedMem(char name[]){
    sem_unlink(name);
    shm_unlink(name);
}

void semaphoreUp(sharedMem shm){
    if (sem_post(shm->sem)==-1){
        PERROR_EXIT("Sem_post");
    }
}

void semaphoreDown(sharedMem shm){
    if (sem_wait(shm->sem)==-1){
        PERROR_EXIT("Sem_wait");
    }
}





