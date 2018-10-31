/*
 *  This file is provided for use with the unix-socket-faq.  It is public
 *  domain, and may be copied freely.  There is no copyright on it.  The
 *  original work was by Vic Metcalfe (vic@brutus.tlug.org), and any
 *  modifications made to that work were made with the understanding that
 *  the finished work would be in the public domain.
 *
 *  If you have found a bug, please pass it on to me at the above address
 *  acknowledging that there will be no copyright on your work.
 *
 *  The most recent version of this file, and the unix-socket-faq can be
 *  found at http://www.interlog.com/~vic/sock-faq/.
 */

#ifndef _SOCKHELP_H_
#define _SOCKHELP_H_

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <limits.h>
#include <netdb.h>
#include <arpa/inet.h>

/* On some crazy systems, these aren't defined. */
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C int atoport( char *service, char *proto );
EXTERN_C struct in_addr *atoaddr( char *address );
/** following commented out, apparently conflicts with libecpg. -- ajwm **/
/** EXTERN_C int get_connection( int socket_type, u_short port, int *listener ); **/

EXTERN_C int make_connection( char *service, int type, char *netaddress );
EXTERN_C int sock_read( int sockfd, char *buf, size_t count );
EXTERN_C int sock_write( int sockfd, const char *buf, size_t count );
EXTERN_C int sock_gets( int sockfd, char *str, size_t count );
EXTERN_C int sock_puts( int sockfd, const char *str );
EXTERN_C void ignore_pipe(void);

#endif
