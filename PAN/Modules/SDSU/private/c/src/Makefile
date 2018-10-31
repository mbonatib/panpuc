CC= gcc -shared -I /usr/src/linux/include -Idriver_api/inc
WFLAGS = -Wall 
OFLAGS = -O
VERFLAG = -D_V14NEW_
#SHMFLAG = -D_SHMEM_
M_SRCS = astroIIILib.c 
D_SRCS = driver_api/sdsuLib.c
#STATLIB = driver_api/libsdsu.a
DEBFLAG = -D_DEBUG_
ifeq ($(SHMFLAG),-D_SHMEM_)
	SH_SRCS = SDSU_shmem.c
else
	SH_SRCS =
endif

OSBIT = $(shell getconf LONG_BIT)
ifeq ($(OSBIT), 64)
        OS = -DOS64
        FFLAG = -fpic
else
        OS = -DOS32
        FFLAG =
endif


LIB = libarcDHE.so

all: clean drivlibIII install

drivlibIII: 
	$(CC) $(WFLAGS) $(DEBFLAG) $(SHMFLAG) $(OFLAGS) $(D_SRCS) $(OS) $(FFLAG) $(M_SRCS) $(SH_SRCS) $(VERFLAG) $(STATLIB) -o $(LIB)

install:
	cp $(LIB) ../lib/.

clean: 
	-rm -f *.so* *.o *.out *.so* ../../../public/c/lib/*.so*
