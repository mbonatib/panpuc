CC= gcc -shared
CC2= gcc 
WFLAGS = -Wall 
SHARED_FLAGS = -shared 
OFLAGS = -O
MAIN_SRCS = dispatcher.c tcpserver.c 
SEND_SRCS = FsenderLib.c
DEBFLAG = -D_DEBUG_
FSAPP = dispatcher
FSLIB = libfsender.so

all: clean sender fsender

fsender:
	$(CC2) $(WFLAGS) $(OFLAGS) $(MAIN_SRCS) ./$(FSLIB) -lm -lpthread -o $(FSAPP)

sender:
	$(CC) $(WFLAGS) $(OFLAGS) $(SEND_SRCS) -lm  -o $(FSLIB)

clean: 
	-rm -f shm_cli $(SHLIB) $(FSAPP) ${FSLIB}
