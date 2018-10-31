CC= gcc 
WFLAGS = -Wall 
OFLAGS = -O
MAIN_SRCS = main.c tcpserver.c roiMask.c 
LIB_SRCS = roiMask.c 
TEST_SRCS = test.c 
DEBFLAG = #-D_DEBUG_
TIMEFLAG = #-D_TIMEDEB_
RAPP = roimasker
RTEST = maskroi
LIBOUT = libroimasker.so

all: clean masker shlib

masker:
	$(CC) $(WFLAGS) $(OFLAGS) $(DEBFLAG) $(TIMEFLAG) $(MAIN_SRCS) ./libcfitsio.a -lm -lpthread -o $(RAPP)
shlib:
	$(CC) -shared $(WFLAGS) $(OFLAGS) $(DEBFLAG) $(TIMEFLAG) $(LIB_SRCS) ./libcfitsio.a -lm -o $(LIBOUT)
	$(CC) $(WFLAGS) $(OFLAGS) $(TEST_SRCS) ./$(LIBOUT) -o $(RTEST)

clean: 
	-rm -f $(RAPP) $(LIBOUT) $(RTEST)
