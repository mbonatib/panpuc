CC= gcc -shared -I /usr/src/linux/include
WFLAGS = -Wall 
OFLAGS = -O
TIMEFLAG = #-DTIMEDEB
DEBFLAG = #-D_DEBUG_
SHMFLAG = #-D_SHMEM_
GF_SRCS = GfitsLib.c GF_shmem.c
PP_SRCS = GfitsPPLib.c 

OSBIT = $(shell getconf LONG_BIT)
ifeq ($(OSBIT), 64)
        OS = -DOS64
        FFLAG = -fpic
	FLIB = libcfitsio_64.a
else
        OS = -DOS32
        FFLAG =
	FLIB = libcfitsio.a
endif

GFLIB = libgfits.so
PPLIB = libgfitspp.so

all: clean gfits pp install

gfits:
	$(CC) $(WFLAGS) $(OFLAGS) ${OS} $(DEBFLAG) $(TIMEFLAG) $(SHMFLAG) $(GF_SRCS) $(FFLAG) ../lib/${FLIB} -o $(GFLIB)

pp:
	$(CC) $(WFLAGS) $(OFLAGS) ${OS} $(DEBFLAG) $(TIMEFLAG) $(PP_SRCS) $(FFLAG) -o $(PPLIB)

install:
	mv $(GFLIB) ../lib/.
	mv $(PPLIB) ../lib/.

clean: 
	-rm -f *.so* *.o *.out *.so.* ../lib/*.so* 
