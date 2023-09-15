#include "sharedMemADT.h"

sharedMem createSharedMem(char * name, int size){
    sharedMem shm = malloc(sizeof(struct shm));
    shm->size = size;
    shm->readOffset = 0;
    shm->writeOffset = 0;
    if ((shm->fd = shm_open(name, O_CREAT | O_RDWR,  S_IRUSR | S_IWUSR))==-1){
        perror("Shm_open");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(shm->fd, shm->size)==-1){
        perror("Ftruncate");
        exit(EXIT_FAILURE);
    }
    if ((shm->startAddress = mmap(NULL, shm->size, PROT_WRITE | PROT_READ, MAP_SHARED, shm->fd, 0))==MAP_FAILED){
        perror("Mmap");
        exit(EXIT_FAILURE);
    }
    if ((shm->sem = sem_open(name, O_CREAT, S_IRUSR|S_IWUSR, 0))==SEM_FAILED){
        perror("Sem_open");
        exit(EXIT_FAILURE);
    }
    return shm;
}


int readSharedMem(sharedMem shm, char * ptr){
    ptr = shm->startAddress + shm->readOffset;
    int readCount = strlen(ptr);
    shm->readOffset += readCount;
    return readCount;
}

void writeSharedMem(sharedMem shm, char * string, int n){
    memcpy(shm->startAddress + shm->writeOffset, string, n);
    shm->writeOffset += n;
}

void closeSharedMem(sharedMem shm) {
    if (munmap(shm->startAddress, shm->size) == -1) {
        perror("Munmap");
        exit(EXIT_FAILURE);
    }
    close(shm->fd);
    sem_close(shm->sem);
    free(shm);
}

void unlinkSharedMem(char name[]){
    if (sem_unlink(name)==-1){
        perror("Sem_unlink");
        exit(EXIT_FAILURE);
    }
    if (shm_unlink(name)==-1){
        perror("Shm_unlink");
        exit(EXIT_FAILURE);
    }
}

void semaphoreUp(sharedMem shm){
    if (sem_post(shm->sem)==-1){
        perror("Sem_post");
        exit(EXIT_FAILURE);
    }
}

void semaphoreDown(sharedMem shm){
    if (sem_wait(shm->sem)==-1){
        perror("Sem_wait");
        exit(EXIT_FAILURE);
    }
}





