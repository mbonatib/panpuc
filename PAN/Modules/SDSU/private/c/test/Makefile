CC= gcc 
WFLAGS = -Wall
OFLAGS = -O
SRC = test2.c
SRCe = test_pcie.c
LIB = ../lib/libarcDHE.so

all: clean pcie 

testop: 
	$(CC)  $(WFLAGS) $(OFLAGS) $(SRC) $(LIB) -ldl -lm -o testapp

pcie: 
	$(CC)  $(WFLAGS) $(OFLAGS) $(SRCe) $(LIB) -ldl -lm -o testappe

clean: 
	-rm -f  *.o *.out testapp testappe
