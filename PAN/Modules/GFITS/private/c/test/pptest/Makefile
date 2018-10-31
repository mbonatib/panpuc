CC= gcc 
WFLAGS = -Wall 
OFLAGS = -O
MAIN_SRCS = pptest.c  
RAPP = pptest

all: clean test

test:
	$(CC) $(WFLAGS) $(OFLAGS) $(MAIN_SRCS) ../../lib/libgfitspp.so -o $(RAPP)

clean: 
	-rm -f $(RAPP)
