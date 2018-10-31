CC= gcc -I /usr/src/linux/include
WFLAGS = -Wall 
OFLAGS = -O
A_SRCS = test.c
T_LIB = ../libtcsrpc.so
TEST = test

all: clean test

test: 
	$(CC) $(WFLAGS) $(OFLAGS) $(A_SRCS) $(T_LIB) -o $(TEST)

clean: 
	-rm -f $(TEST)
