#ifndef SLAVE_H
#define SLAVE_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "commons.h"

#define CMD_NAME "md5sum "

void printMD5(const char path[]);
int digits(int n);

#endif