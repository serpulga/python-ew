CFLAGS = -m32 -Dlinux -D__i386 -D_LINUX -D_INTEL -D_USE_SCHED  -D_USE_PTHREADS -D_USE_TERMIOS
CC = gcc
EW_INCLUDE = $(EW_HOME)/include/
EW_LIB = $(EW_HOME)/lib/
LIB = -lm -lpthread

all: ring_access

ring_access:
	$(CC) $(CFLAGS) -I$(EW_INCLUDE) -L$(EW_LIB) $(LIB) -c ringwriter.c
	$(CC) $(CFLAGS) -I$(EW_INCLUDE) -L$(EW_LIB) $(LIB) -c ringreader.c

install: ring_access
	cp *.o $(EW_LIB)
	cp *.h $(EW_INCLUDE)

clean:
	rm -rf *.o
