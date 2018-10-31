CC= gcc -shared
CC2= gcc 
WFLAGS = -Wall 
OFLAGS = -O
U_SRCS = univLib.c  getLib.c
M_SRCS = media_libs/DEV_SerialLib.c media_libs/DEV_TcpLib.c
DEBFLAG = -D_DEBUG_
SHLIB = libuniv.so
TSRCS = test.c
TEST = testapp

all: clean exec install

exec: 
	$(CC) $(WFLAGS) $(DEBFLAG) $(OFLAGS) $(U_SRCS) $(M_SRCS) -ldl -lm -o $(SHLIB)

test:
	$(CC2) $(WFLAGS) $(DEBFLAG) $(OFLAGS) $(TSRCS) ../../lib/libuniv.so -o $(TEST)
install:
	mv $(SHLIB) ../../lib/.
clean: 
	-rm -f *.so* *.o *.out *.so* ../../lib/$(SHLIB) 
