CC= gcc -shared -I /usr/src/linux/include
WFLAGS = -Wall 
OFLAGS = -O
TIMEFLAG = #-DTIMEDEB
HIGHFLAG = #-D_HIGH_MEM_
MEM_SRCS = bufLib.c
OSBIT = $(shell getconf LONG_BIT)

ifeq ($(OSBIT), 64)
        OS = -DOS64
        FFLAG = -fpic
else
        OS = -DOS32
        FFLAG =
endif


MLIB = libbuf.so

all: clean buflib install

buflib:
	$(CC) $(WFLAGS) $(OFLAGS) ${OS} ${FFLAG} $(MEM_SRCS) $(HIGHFLAG)  -o $(MLIB)


install:
	mv $(MLIB) ../lib/.

clean: 
	-rm -f *.so* *.o *.out *.so.* ../lib/*.so* 
