CC= gcc -shared -I /usr/src/linux/include
WFLAGS = -Wall 
OFLAGS = -O
TIMEFLAG = #-DTIMEDEB
DEBFLAG = #-D_DEBUG_
HIGHFLAG = #-D_HIGH_MEM_
LOGIC_SRCS = LogicLib.c
OSBIT = $(shell getconf LONG_BIT)

ifeq ($(OSBIT), 64)
        OS = -DOS64
        FFLAG = -fpic
else
        OS = -DOS32
        FFLAG =
endif

LLIB = libLogic.so

all: clean Logiclib install

Logiclib:
	$(CC) $(WFLAGS) $(DEBFLAG) $(OFLAGS) ${OS} ${FFLAG} $(LOGIC_SRCS) $(HIGHFLAG)  -o $(LLIB)

install:
	mv $(LLIB) ../../../public/c/lib/.

clean: 
	-rm -f *.so* *.o *.out *.so.* ../lib/*.so* 
