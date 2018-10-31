CC= gcc 
WFLAGS = -Wall 
SHARED_FLAGS = -shared 
OFLAGS = -O
CLILIB_SRCS = shmclientLib.c
CLI_SRCS = shm_client.c
FITSLIB = ../../lib/libcfitsio.a
DISPFLAG = -D_DISPLAY_
ifeq ($(DISPFLAG), -D_DISPLAY_)
	DISPLIB = ./libcdl.a
else
	DISPLIB = 
endif
SHLIB = libclishm.so

all: clean client 

client:
	$(CC) $(WFLAGS) $(OFLAGS) $(CLI_SRCS) $(CLILIB_SRCS) $(FITSLIB) $(DISPFLAG) $(DISPLIB) -lm -o shm_cli

clilib:
	$(CC) $(SHARED_FLAGS) $(WFLAGS) $(OFLAGS) $(CLILIB_SRCS) -o $(SHLIB)

clean: 
	-rm -f shm_cli $(SHLIB)
