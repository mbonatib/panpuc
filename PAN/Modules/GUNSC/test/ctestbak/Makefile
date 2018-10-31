CC= gcc -I /usr/src/linux/include
WFLAGS = -Wall 
OFLAGS = -O
DFLAGS = -D_DEBUG_
SHAREDFLAG = -shared
MAP_SRC = mapLib.c
MAP_LIB = libmap.so
MAIN_SRC = main.c
UNSC_SRC = GUnscrambling.c
MAIN_OUT = unsc
CFITSIO_LIB = /home/ArcVIEW/Modules/FITS/Clib/lib/libcfitsio.a
GFITS_LIB = /home/ArcVIEW/Modules/GFITS/private/c/lib/libgfits.so
GDISP_LIB = /home/ArcVIEW/Modules/RTD/Clib/src/lidgdisp.so
all: main

maplib:
	$(CC) $(SHAREDFLAG) $(WFLAGS) $(OFLAGS) $(MAP_SRC) $(CFITSIO_LIB)  -o $(MAP_LIB)

main:
	$(CC) $(WFLAGS) $(OFLAGS) $(DFLAGS) $(MAIN_SRC) $(UNSC_SRC) $(GDISP_LIB) -ldl ./libmap.so $(GFITS_LIB) -o $(MAIN_OUT)

install:
	mv $(SCR_OUT) ../.
	

clean: 
	-rm -f  *.o *.so
