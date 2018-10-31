CC= gcc -shared -I /usr/src/linux/include
WFLAGS = -Wall 
OFLAGS = -O
TIMEFLAG = #-DTIMEDEB
SRCS = commonLib.c
OSBIT = $(shell getconf LONG_BIT)

ifeq ($(OSBIT), 64)
        OS = -DOS64
        FFLAG = -fpic
else
        OS = -DOS32
        FFLAG =
endif

CLIB = libcommon.so

all: clean clib install

clib:
	$(CC) $(WFLAGS) $(OFLAGS) $(TIMEFLAG) $(SRCS) $(OS) $(FFLAG) -o $(CLIB)

install:
	mv $(CLIB) ../lib/.

clean: 
	-rm -f $(CLIB) ../lib/$(CLIB)
