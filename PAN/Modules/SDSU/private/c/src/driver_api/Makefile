CC= gcc -shared -I /usr/src/linux/include -Iinc
WFLAGS = -Wall 
OFLAGS = -O
VERFLAG = -D_V14NEW_
D_SRCS = sdsuLib.c
DEBFLAG = #-D_DEBUG_
OLDPWD = `pwd`
OSBIT = $(shell getconf LONG_BIT)

ifeq ($(OSBIT), 64)
        OS = -DOS64
        FFLAG = -fpic
else
        OS = -DOS32
        FFLAG =
endif

SH_LIB = libsdsu.so
ST_LIB = libsdsu.a
ST_OBJ = libsdsu.o

all: clean submods static

submods:
	cd genIII_pcix; make
	cd $(OLDPWD)
	cd sim; make
	cd $(OLDPWD)

static:
	$(CC) $(WFLAGS) $(OFLAGS) $(D_SRCS) $(OS) $(FFLAG) $(VERFLAG) $(DEBFLAG) -o $(ST_OBJ)
	ar rcs $(ST_LIB) $(ST_OBJ)

clean: 
	-rm -f *.o *.a *.so
