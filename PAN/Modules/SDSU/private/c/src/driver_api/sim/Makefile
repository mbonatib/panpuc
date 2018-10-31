CC= gcc -shared -I/usr/src/linux/include -I../inc
WFLAGS = -Wall 
OFLAGS = -O
VERFLAG = -D_V14NEW_
D_SRCS = simLib.c
F_SRCS = diskImageLib/diskImageLib.c
DEBFLAG = -D_DEBUG_
CLIB32 = diskImageLib/libcfitsio.a
CLIB64 = diskImageLib/libcfitsio_64.a
SH_LIB = libsdsusim.so
OSBIT = $(shell getconf LONG_BIT)

ifeq ($(OSBIT), 64)
        OS = -DOS64
        FFLAG = -fpic
	CLIB = $(CLIB64)
else
        OS = -DOS32
        FFLAG =
	CLIB = $(CLIB32)
endif

all: clean shared install

shared: 
	$(CC) $(WFLAGS) $(OFLAGS) $(D_SRCS) $(F_SRCS) $(CLIB) $(VERFLAG) $(OS) $(FFLAG) $(DEBFLAG) -o $(SH_LIB)

install:
	mv $(SH_LIB) ../../../lib/.

clean: 
	-rm -f *.o *.a *.so ../../../lib/$(SH_LIB)
