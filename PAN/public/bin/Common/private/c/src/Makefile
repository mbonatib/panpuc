CC= gcc -shared -I /usr/src/linux/include
WFLAGS = -Wall 
OFLAGS = -O
TIMEFLAG = #-DTIMEDEB
SRCS = commonLib.c

CLIB = libcommon.so

all: clean clib install

clib:
	$(CC) $(WFLAGS) $(OFLAGS) $(TIMEFLAG) $(SRCS) -o $(CLIB)

install:
	mv $(CLIB) ../lib/.

clean: 
	-rm -f $(CLIB) ../lib/$(CLIB)
