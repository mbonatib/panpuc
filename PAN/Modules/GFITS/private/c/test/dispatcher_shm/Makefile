CC= gcc -shared
CC2= gcc 
WFLAGS = -Wall 
SHARED_FLAGS = -shared 
OFLAGS = -O
MAIN_SRCS = dispatcher.c tcpserver.c 
SHM_SRCS = shmclientLib.c 
SEND_SRCS = FsenderLib.c
DEBFLAG = -D_DEBUG_
FSAPP = dispatcher
SHLIB = libclishm.so
FSLIB = libfsender.so

all: clean shmem sender fsender

fsender:
	$(CC2) $(WFLAGS) $(OFLAGS) $(MAIN_SRCS) ./$(SHLIB) ./$(FSLIB) -lm -lpthread -o $(FSAPP)

sender:
	$(CC) $(WFLAGS) $(OFLAGS) $(SEND_SRCS) -lm  -o $(FSLIB)

shmem:
	$(CC) $(WFLAGS) $(OFLAGS) $(SHM_SRCS) $(DEBFLAG) -lm -o $(SHLIB)

clean: 
	-rm -f shm_cli $(SHLIB) $(FSAPP) ${FSLIB}
