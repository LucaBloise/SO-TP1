#ifndef APPLICATION_H
#define APPLICATION_H

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define SLAVES = 5;
#define INITIAL_LOADOUT = 5;
#define FDS_ARRAY_SIZE = 4*SLAVES;

void createSlaves(int slavesAmount);

void createPipe(*int fds);

void slaveClose(int slaveIndex);

void sendFiles(int filesAmount, int slaveIndex, char* files[]);

#endif