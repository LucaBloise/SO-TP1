#ifndef SLAVE_H
#define SLAVE_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "commons.h"

#define MD5SUM_PATH "/usr/bin/md5sum"

void printMD5(char path[]);
int digits(int n);

#endif