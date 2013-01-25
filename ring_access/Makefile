CFLAGS = -m32 -Dlinux -D__i386 -D_LINUX -D_INTEL -D_USE_SCHED  -D_USE_PTHREADS -D_USE_TERMIOS
CC = gcc
INCLUDE = /path/to/ew/include/
LIBRARIES = /path/to/ew/lib/
LIB = -lm -lpthread

all: ring_access

ring_access:
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIBRARIES) $(LIB) -c ringwriter.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -L$(LIBRARIES) $(LIB) -c ringreader.c

clean:
	rm -rf *.o
