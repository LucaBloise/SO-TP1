#ifndef SLAVE_H
#define SLAVE_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PIPE_CAP 4096
#define MAX_PATH_LENGTH 256
#define MD5_LENGTH 32

void printMD5(const char path[]);

#endif