# makefile utility for tcs communication library. Configured for Linux OS.
#
# modification history
# --------------------
# 01a 10nov98, rcr - Created.

#
# Some handy variables. Change MACHINE to your target architecture.
# Valid names for target architecture are sol, sun and linux
#

BASENAME      = tcscomm
MACHINE       = fcx
STATICLIBRARY = lib$(BASENAME)_$(MACHINE).a
SHAREDIR      = ..

#
# Compiler and compiler options
#

CC     = gcc
#CFLAGS = -Wall
CFLAGS = -Wall -g
SHFLAGS = -shared
SHLIB = libtcsrpc.so


#
# Source files and objects
#

CFILES= tcsCmd.c
OFILES= $(CFILES:%c=%o)

#
# Target rules
#

default	: shared install

static	: $(OFILES)
	ar rv ./$(STATICLIBRARY) $(OFILES)

shared:
	$(CC) $(CFLAGS) $(SHFLAGS) $(CFILES) -o $(SHLIB)


install	:
#	-cp tcsCmd.h $(SHAREDIR)/include
#	-cp $(STATICLIBRARY) ../../lib
	-cp $(SHLIB) ../../lib

all	: clean static shared install

clean	: 
	-rm -f *.o *.a *.bak core

distclean: clean
	-rm Makefile config.cache config.log config.status

# auto dependency make

depend	:
	$(CC) -MM $(CFLAGS) $(CFILES) > makedep
	echo '/^# DO NOT DELETE THIS LINE/+1,$$d' > eddep
	echo '$$r makedep' >> eddep
	echo 'w' >> eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	rm eddep makedep

# DO NOT DELETE THIS LINE -- make depend uses it

