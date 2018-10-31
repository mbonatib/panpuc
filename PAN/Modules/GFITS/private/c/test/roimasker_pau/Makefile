CC= gcc 
CC_S= gcc -c -I /usr/src/linux/include 
WFLAGS = -Wall 
OFLAGS = -O
MAIN_SRCS = main.c tcpserver.c roiMask.c 
LIB_SRCS = roiMask.c 
TEST_SRCS = test.c 
DEBFLAG = -D_DEBUG_
TIMEFLAG = #-D_TIMEDEB_
RAPP = roimasker
RTEST = maskroi_d
RTEST_ST = maskroi
LIBOUT = libroimasker.so
ST_LIB = libroimasker.a
ST_OBJ = libroimasker.o
OSBIT = $(shell getconf LONG_BIT)
ifeq ($(OSBIT), 64)
        OS = -DOS64
        FFLAG = -fpic
        FLIB = libcfitsio_64.a
else
        OS = -DOS32
        FFLAG =
        FLIB = libcfitsio.a
endif

all: clean masker shlib static

masker:
	$(CC) $(WFLAGS) $(OFLAGS) $(DEBFLAG) $(TIMEFLAG) $(MAIN_SRCS) $(FLIB) $(FFLAG) -lm -lpthread -o $(RAPP)
shlib:
	$(CC) -shared $(WFLAGS) $(OFLAGS) $(DEBFLAG) $(TIMEFLAG) $(LIB_SRCS) $(FLIB) $(FFLAG) -lm -o $(LIBOUT)
	$(CC) $(WFLAGS) $(OFLAGS) $(TEST_SRCS) ./$(LIBOUT) -o $(RTEST)
static:
	$(CC_S) $(WFLAGS) $(OFLAGS) $(DEBFLAG) $(TIMEFLAG) $(LIB_SRCS) -lm -o $(ST_OBJ)
	ar rcs $(ST_LIB) $(ST_OBJ)
	$(CC) $(WFLAGS) $(OFLAGS) $(TEST_SRCS) ./$(ST_LIB) -lm $(FLIB) $(FFLAG)  -o $(RTEST_ST)

clean: 
	-rm -f $(RAPP) $(LIBOUT) $(RTEST)
