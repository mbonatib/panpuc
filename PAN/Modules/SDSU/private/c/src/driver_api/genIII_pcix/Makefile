CC= gcc -shared -I/usr/src/linux/include -I../inc
WFLAGS = -Wall 
OFLAGS = -O
VERFLAG = -D_V14NEW_
D_SRCS = genIIILib.c
DEBFLAG = #-D_DEBUG_
OSBIT = $(shell getconf LONG_BIT)

ifeq ($(OSBIT), 64)
        OS = -DOS64
        FFLAG = -fpic
else
        OS = -DOS32
        FFLAG =
endif



SH_LIB = libsdsugenIII_pcix.so

all: clean shared install

shared: 
	$(CC) $(WFLAGS) $(OFLAGS) $(D_SRCS) $(VERFLAG) $(DEBFLAG) $(OS) $(FFLAG) -o $(SH_LIB)

install:
	mv $(SH_LIB) ../../../lib/.
clean: 
	-rm -f *.o *.a *.so ./../../lib/$(SH_LIB)
