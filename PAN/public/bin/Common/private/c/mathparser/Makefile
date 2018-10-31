CC= gcc -shared -I /usr/src/linux/include
WFLAGS = -Wall 
OFLAGS = -O
DEBFLAG = #-D_DEBUG_
P_SRCS = MathParserLib.c

PLIB = libmathparser.so

all: clean plib install

plib: 
	$(CC) $(WFLAGS) $(OFLAGS) $(DEBFLAG) $(P_SRCS) -lm -o $(PLIB)

install:
	mv $(PLIB) ../../../public/c/lib/.

clean: 
	-rm -f *.so* *.o *.out *.so.* ../../../public/c/lib/*.so* 
