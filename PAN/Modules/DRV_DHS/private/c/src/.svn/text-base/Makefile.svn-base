CC= gcc -shared -I /usr/src/linux/include
WFLAGS = -Wall 
OFLAGS = -O
DEBFLAG = #-D_DEBUG_
U_SRCS = DHS_ctcp_mainLib.c
T_SRCS = DHS_ctcp_test.c
#A_SRCS = DS_algors.c DS_SendPixelData.c

CTCPLIB = libdhs_ctcp.so

all: clean ctcplib install

ctcplib: 
	$(CC) $(WFLAGS) $(OFLAGS) $(DEBFLAG) $(U_SRCS) $(A_SRCS) -lm -o $(CTCPLIB)

install:
	mv $(CTCPLIB) ../lib/.

test:
	gcc $(WFLAGS) $(OFLAGS) $(DEBFLAG) $(T_SRCS) ../lib/$(CTCPLIB) -lm -o test

clean: 
	-rm -f *.so* *.o *.out *.so.* ../lib/*.so* 
