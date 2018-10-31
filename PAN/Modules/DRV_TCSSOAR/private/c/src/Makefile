CC= gcc 
WFLAGS = -Wall 
SHARED_FLAGS = -shared 
OFLAGS = -O
TCSLIB_SRCS = tcsSoarLib.c
CLI_SRCS = clitest.c
TCSLIB = libtcs_soar.so
CLI=clitest

all: clean tcslib client

client:
	$(CC) $(WFLAGS) $(OFLAGS) $(CLI_SRCS) ../lib/$(TCSLIB) -o $(CLI)

tcslib:
	$(CC) $(SHARED_FLAGS) $(WFLAGS) $(OFLAGS) $(TCSLIB_SRCS) -o $(TCSLIB)
	mv $(TCSLIB) ../lib/.

clean: 
	-rm -f ../lib/$(TCSLIB) $(CLI) 
