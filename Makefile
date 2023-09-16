CC = gcc
CFLAGS = -Wall -g -lpthread -lrt

SRC_APP = application.c sharedMemADT.c
SRC_SLAVE = slave.c
SRC_VIEW = view.c sharedMemADT.c

OBJ_APP = $(SRC_APP:.c=.o)
OBJ_SLAVE = $(SRC_SLAVE:.c=.o)
OBJ_VIEW = $(SRC_VIEW:.c=.o)

all: md5 slave vista

md5: $(OBJ_APP)
	$(CC) $(CFLAGS) -o $@ $^

slave: $(OBJ_SLAVE)
	$(CC) $(CFLAGS) -o $@ $^

vista: $(OBJ_VIEW)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf md5 slave vista $(OBJ_APP) $(OBJ_SLAVE) $(OBJ_VIEW)

.PHONY: all clean
