CC= gcc -shared -I/usr/src/linux/include -I../inc
WFLAGS = -Wall 
OFLAGS = -O
VERFLAG = -D_V14NEW_
D_SRCS = simLib.c
DEBFLAG = -D_DEBUG_

SH_LIB = libsdsusim.so

all: clean shared install

shared: 
	$(CC) $(WFLAGS) $(OFLAGS) $(D_SRCS) $(VERFLAG) $(DEBFLAG) -o $(SH_LIB)

install:
	mv $(SH_LIB) ../../../lib/.

clean: 
	-rm -f *.o *.a *.so ../../../lib/$(SH_LIB)
