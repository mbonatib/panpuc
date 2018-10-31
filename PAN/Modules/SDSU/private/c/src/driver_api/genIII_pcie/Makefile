CC= gcc -shared -I/usr/src/linux/include -ICArcDevice/src -I../inc
WFLAGS = -Wall 
OFLAGS = -O
VERFLAG = -D_V14NEW_
D_SRCS = genIII_pcieLib.c
DEBFLAG = #-D_DEBUG_
OSBIT = $(shell getconf LONG_BIT)

ifeq ($(OSBIT), 64)
        OS = -DOS64
        FFLAG = -fpic
else
        OS = -DOS32
        FFLAG =
endif


ARC_LIB = libCArcDevice.so
#ARC_LIB = libCArcDevice.a
SH_LIB = libsdsugenIII_pcie.so

all: clean shared install

shared: 
	$(CC) $(WFLAGS) $(OFLAGS) $(D_SRCS) $(VERFLAG) $(DEBFLAG) $(OS) $(FFLAG) $(ARC_LIB) -o $(SH_LIB)

install:
	mv $(SH_LIB) ../../../lib/.
clean: 
	-rm -f *.o $(SH_LIB) ./../../lib/$(SH_LIB)
