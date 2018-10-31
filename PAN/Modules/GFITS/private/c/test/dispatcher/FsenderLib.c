#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
//#include "fitsio.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/stat.h>


#ifdef TIMEDEB
  #include <time.h>
#endif

#ifdef _DEBUG_
#  define pdebug(fmt, args...) printf ("FSENDER: " fmt, ## args)
#else
#  define pdebug(fmt, args...)
#endif

#define OK		0
#define NTRIES		3

char error_msg[100];
char svradd[100];
int svrport;
int sockfd, n;
struct sockaddr_in serv_addr;
struct hostent *server;
int connected=0;


int FS_set_host (char *host, int port)
{
	strcpy (svradd, host);
	svrport = port;

	return (OK);
}

int FS_close (void)
{
        close (sockfd);
        connected = 0;
        sockfd = 0;
        return (OK);
}


int FS_connect (char *host, int port)
{
    if ((connected) && (sockfd != 0)) {
        printf ("fs_connect: already connected to %s, port %d\n", svradd, svrport);
        return (sockfd);
    }
    if (sockfd != 0)
        FS_close ();

    FS_set_host (host, port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket\n");
        return (-ECOMM);
    }
    server = gethostbyname(svradd);
    if (server == NULL) {
        printf("ERROR, no such host\n");
	FS_close();
        return (-ECOMM);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
        serv_addr.sin_port = htons(svrport);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
          printf("ERROR connecting \n");
	  FS_close ();
          return (-ECOMM);
    }
    connected = 1;
    return (sockfd);
}


int FS_SendFits (unsigned long buf, unsigned long bufsize)
{
int stat=0, errcnt;
size_t membufsize;
void *membuf;

	membuf = (void *) buf;
	membufsize = (size_t) bufsize;
	if ((membuf == NULL || membufsize == 0))
		return (-EFAULT);

        char buffer[256];
	errcnt = 1;
again:
	pdebug ("FS_sen: trying to send fits image (attempt %d\n)", errcnt);
	if ((stat = FS_connect (svradd, svrport)) < 0){
			FS_close ();
			return (stat);
	}

        long dataend;
        stat = 0;
	dataend = bufsize;
        sprintf(buffer,"%ld",dataend);
        n = send(sockfd, buffer, sizeof buffer, 0);
	if (n<=0) {
		connected = 0;
		if (errcnt >= NTRIES){
			FS_close();
			printf ("FS_send: Error sending size, returning %d\n", -errno);
			return (-errno);
		}
		errcnt++;
		goto again;
	}
#if 0
        int i;
        for(i = 0; i < dataend/4; i++){
          n = send(sockfd, &membuf[i*4], 4, 0);
        }
#else
	pdebug ("FS_sen: about to send fits data\n");
        n = send(sockfd, membuf, dataend, 0);
	pdebug ("FS_sen: after send fits data\n");
#endif
	if (n<=0) {
		FS_close ();
		printf ("FS_send: Error sending data, returning %d\n", -errno);
		return (-errno);
	}
	
	pdebug ("closing connection\n");	
        stat = 0;
	FS_close ();
        return (-stat);
}
