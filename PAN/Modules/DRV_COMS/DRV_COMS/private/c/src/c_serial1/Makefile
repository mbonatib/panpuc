#
#	Produces shared version of test shared library
#

BASENAME = serial
MACHINE = 1
V = 1
MV = 0
SHAREDLIB = lib$(BASENAME)_$(MACHINE).so
DESTDIR  = ../../lib/.

CC = gcc
# CFLAGS = -O2 -Wall -g -I./ -I/home/german/rtai/rtai-1.6/include
CFLAGS = -O2 -Wall -g -I./
LD = ld
LDFLAGS =  -lm -lc
# INCS = -I/home/german/rtai/rtai-1.6/include

CFILES = commLib.c
	
OFILES = $(CFILES:%c=%o)

# default : shared
all : shared 
shared : $(OFILES)
	$(LD) -G -o $(SHAREDLIB) -h $(SHAREDLIB) $(OFILES) $(LDFLAGS)
	
install :
	-cp $(SHAREDLIB) $(DESTDIR)
#	-/sbin/ldconfig
#
#	don't forget ldconfig
	
clean :
	-rm -f *.so.* core
	


