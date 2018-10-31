#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include<netdb.h>
#include <sys/time.h>
#include "roiMask.h"
#include "cdl.h"
#include "tcpserver.h"
#include <pthread.h>

#define	CMD	1
#define DATA	2

void *GetCmd ();
int running=1;
int delimg=0;
int display=0;
CDLPtr cdl;

int main(int argc, char *argv[])
{
pthread_t thread1;
int rc1;
int sock, timeout;
struct timeval tvstart, tvend;
int bufsize = 8192;
int nread;
char *message = malloc(bufsize);
char *hostname = malloc (30);
int i, port, mode;
struct sockaddr_in address;
struct hostent *host_struct;
char impath[256];
char token[64];
char expid[64];

    if( (rc1=pthread_create( &thread1, NULL, &GetCmd, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc1);
   }
   printf ("maint thread started\n"); 
   running = 1;
   mode = 0;                    /* 0: wait, 1: not wait*/
   hostname = "localhost";
   port = 5435;
   timeout = 10;
   i = 1;
   if (argc > 1)
        while (i < argc) {
                if (argv[i][0] == '-') {
                        if (argv[i][1] == 'h')
                                {hostname = argv[i+1]; i++;}
                        else
                                if (argv[i][1] == 'p')
                                        {port = atoi (argv[i+1]); i++;}
                        else
                                if (argv[i][1] == 't')
                                        {timeout = atoi(argv[i+1]); i++;}
                        else
                                if (argv[i][1] == 'm')
                                        {mode = atoi (argv[i+1]); i++;}
			else
                                if (argv[i][1] == 'd')
					delimg = 1;
			else
                                if (argv[i][1] == 's') {
					display = 1;
					cdl = cdl_open ("fifo:/dev/imt1i:/dev/imt1o");
					if (cdl != NULL)
						printf ("images will be displayed\n");
					else printf ("error starting display\n");
				}	
                }
                i++;
        }
   if ((host_struct = gethostbyname (hostname)) == NULL){
           printf ("ERROR %d getting_mem\n", errno);
           return (-errno);
   }

   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("ERROR %d creating_socket\n", errno);
        return (-errno);
   }

   if (fcntl (sock, F_SETFL, O_NONBLOCK) <0){
                printf ("ERROR %d setting_socket", errno);
                close (sock);
                return (-errno);
   }
   bcopy (host_struct->h_addr, (char *) &address.sin_addr.s_addr, host_struct->h_length);
   address.sin_family = AF_INET;
   address.sin_port = htons(port);

   if (gettimeofday (&tvstart, NULL) <0) {
        printf ("ERROR %d reading_time\n", errno);
        close (sock);
        return (-errno);
   }

init:
    if ((connect(sock,(struct sockaddr *) &address, sizeof(address)) == -1) && (errno != EINPROGRESS)) {
        printf ("ERROR %d connecting\r", errno);
	if (running){
		sleep (1);
		goto init;
	}
    }

    printf ("started. Connected to %s, port %d\n", hostname, port); 
    if (delimg)
    	printf ("Original images will be deleted\n"); 
    while (running) {
        while ((nread = read (sock, message, bufsize)) < 0) {
                if (errno != EAGAIN) {
                	if (errno != ECONNREFUSED) {
                       	 	printf ("ERROR %d reading\n", errno);
                       	 	close (sock);
                       	 	return (-errno);
			}
			printf ("can't hear a thing ...\n");
                }
                usleep (10000);
                gettimeofday (&tvend, NULL);
                if (!running) {
			break;
                }
        }
	if (nread > 0){	/*got message*/
//		printf ("message received: %s\n", message);
	        sscanf (message, "%*s %s %s %s", token, expid, impath);
                if (strncmp (message, "ASYNC", 5) == 0){
                	if (strncmp (token, "NEWFITS", 7) == 0){
				printf ("newfits %s %s\n", expid, impath);
				RMASK_GetImage (impath);
			}
		}
	}
    }
    running = 0;
    RMASK_Close();
    if (display)
    	cdl_close (cdl);
    printf ("main thread stoped\n");
    pthread_join( thread1, NULL);

    return(0);
}


void *GetCmd()
{
    printf ("tcp thread started\n"); 
    tcp_server ();
    printf ("tcp thread stoped\n"); 
   return (0);
}
