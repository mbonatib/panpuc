CC= gcc 
WFLAGS = -Wall 
OFLAGS = -O
SOCK_SRCS = sendcmd.c
SOCKOUT = sendsockcmd


all: socket install

socket:
	$(CC) $(WFLAGS) $(OFLAGS) $(SOCK_SRCS) -o $(SOCKOUT)
	

install:
	cp $(SOCKOUT) ../../.

clean: 
	-rm -f $(SOCKOUT) *.o
