/* tcsCmd.h - tcsCmd structure header file */

/*
This is the network structure of a TCP system message.  It is used
for both a command and a response.
*/

#ifndef __TCSCMD_H
#define __TCSCMD_H

struct tcp_msg {
	int how;
	char *message;
};
typedef struct tcp_msg tcp_msg;


/* "tcp_msg" = 0x7463705f 0x6d736700 in hex format */
#define TCP_MSG_PROG ((u_long)0x200123)
#define TCP_MSG_VERS ((u_long)1)
#define SEND_TCP_MSG ((u_long)1)
#define ERROR        (-1)
#define OK           (0)
#define HOW          (1)
#define DEFTMOUT     3000

/* extern variables */
extern tcp_msg *tcpLastReply;

/* function prototypes */
int      tcpCmd (int ref, char *msg, int how);
bool_t   xdr_tcp_msg_cln(XDR *xdrs, tcp_msg *objp);
char    *tcpReply();

extern int tcsOpen (char *name, int *err);
extern int tcsClose(int ref);
extern int sendTCS (int ref, char *line, int tmout);
extern int setTimeout (int tm);
#endif
