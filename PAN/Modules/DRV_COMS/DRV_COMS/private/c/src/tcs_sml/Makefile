CC= gcc 
WFLAGS = -Wall 
SHARED_FLAGS = -shared 
OFLAGS = -O
TCSLIB_SRCS = tcsSmlLib.c
CLI_SRCS = clitest.c
TCSLIB = libtcssml.so
CLI=clitest

all: clean tcslib client

client:
	$(CC) $(WFLAGS) $(OFLAGS) $(CLI_SRCS) ../../lib/$(TCSLIB) -o $(CLI)

tcslib:
	$(CC) $(SHARED_FLAGS) $(WFLAGS) $(OFLAGS) $(TCSLIB_SRCS) -o $(TCSLIB)
	mv $(TCSLIB) ../../lib/.

clean: 
	-rm -f ../lib/$(TCSLIB) $(CLI) 
