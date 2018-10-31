CC= gcc 
WFLAGS = -Wall 
OFLAGS = -O
MAIN_SRCS = main.c tcpserver.c roiMask.c 
DEBFLAG = -D_DEBUG_
RAPP = roimasker
DLIB = libcdl.a

all: clean masker

masker:
	$(CC) $(WFLAGS) $(OFLAGS) $(MAIN_SRCS) ./libcfitsio.a ./libcdl.a -lm -lpthread -o $(RAPP)

clean: 
	-rm -f $(RAPP)
