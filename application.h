#ifndef APPLICATION_H
#define APPLICATION_H

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/select.h>

#define SLAVES = 5;
#define INITIAL_LOADOUT = 5;
#define FDS_ARRAY_SIZE = 4*SLAVES;
#define RESULT_SIZE = 100;

void createSlaves(int slavesAmount);

void createPipe(*int fds);

void sendFiles(int filesAmount, int slaveIndex, char* files[]);

void slaveManager(int argc, char* argv[]);

void closeAll();

#endif