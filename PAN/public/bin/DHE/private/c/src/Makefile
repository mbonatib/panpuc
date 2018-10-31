CC= gcc -shared -I /usr/src/linux/include
WFLAGS = -Wall 
OFLAGS = -O
TIMEFLAG = #-DTIMEDEB
DEBFLAG = #-D_DEBUG_
DHE_SRCS = dheLib.c
OSBIT = $(shell getconf LONG_BIT)

ifeq ($(OSBIT), 64)
        OS = -DOS64
        FFLAG = -fpic
else
        OS = -DOS32
        FFLAG =
endif

DLIB = libgendhe.so

all: clean dhelib install

dhelib:
	$(CC) $(WFLAGS) $(DEBFLAG) $(OFLAGS) ${OS} ${FFLAG} $(DHE_SRCS) -o $(DLIB)

install:
	mv $(DLIB) ../lib/.

clean: 
	-rm -f *.so* *.o *.out *.so.* ../lib/*.so* 
