CC= gcc -shared -I /usr/src/linux/include
WFLAGS = -Wall 
OFLAGS = -O
DEBFLAG =#-D_DEBUG_
#TIMEFLAG = -DTIMEDEB
U_SRCS = GUnscrambling.c
A_SRCS = GU_transformations.c GU_algors.c
#GU_SRSC= GU_queue.c
GLIB = libunsc.so
ALIB = libunscalgors.so

OSBIT = $(shell getconf LONG_BIT)

ifeq ($(OSBIT), 64)
	OS = -DOS64
	FFLAG = -fpic
else
	OS = -DOS32
	FFLAG = 
endif

all: clean unsclib algorlib install

unsclib: 
	$(CC) $(WFLAGS) $(OFLAGS) $(FFLAG) $(DEBFLAG) $(TIMEFLAG) $(U_SRCS) ${GU_SRSC} ${OS} -ldl -o $(GLIB)

algorlib: 
	$(CC) $(WFLAGS) $(OFLAGS) $(FFLAG) $(DEBFLAG) $(TIMEFLAG) $(A_SRCS) ${OS} -ldl -lm -o $(ALIB)

install:
	mv $(GLIB) ../../../public/c/lib/.
	mv $(ALIB) ../../../public/c/lib/.

clean: 
	-rm -f *.so* *.o *.out *.so.* ../../../public/c/lib/*.so* 
