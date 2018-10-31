CC= gcc -shared -I /usr/src/linux/include -I../../public/c/include/
WFLAGS = -Wall 
OFLAGS = -O
TIMEFLAG = #-DTIMEDEB
HIGHFLAG = #-D_HIGH_MEM_
#VERFLAG = -D_V14NEW_
MEM_SRCS = memLib.c
SOCK_SRCS = sockLib.c

MLIB = libmem.so.20
SOCKLIB = libsock.so.20

OSBIT = $(shell getconf LONG_BIT)
ifeq ($(OSBIT), 64)
        OS = -DOS64
        FFLAG = -fpic
else
        OS = -DOS32
        FFLAG =
endif


all: clean memlib install

memlib:
	$(CC) $(WFLAGS) $(OFLAGS) $(OS) $(FFLAG) $(MEM_SRCS) $(HIGHFLAG)  -o $(MLIB)

socket:
	$(CC) $(WFLAGS) $(OFLAGS) $(OS) $(FFLAG) $(SOCK_SRCS) -o $(SOCKLIB)
	

install:
	mv $(MLIB) ../lib/.

clean: 
	-rm -f *.so* *.o *.out *.so.* ../lib/*.so* 
