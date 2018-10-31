#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "shmclientLib.h"
#include "FsenderLib.h"
#include "tcpserver.h"
#include <pthread.h>

#define	CMD	1
#define DATA	2

void *GetCmd ();
int shmqid=0;
int running=1;
int ready=0;

int main()
{
pthread_t thread1;
int rc1;
int shmid=0;
long qtype;
char *shm=NULL;
char *nextbuf=NULL;
char qmsg[MAXLEN];
char arg1[10];
char arg2[10];
int shmsize=0;
long imsize;
long bufcnt=-1;

    if( (rc1=pthread_create( &thread1, NULL, &GetCmd, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc1);
   }
    printf ("maint thread started\n"); 
    running = 1;
init:
    printf ("Looking for segment ...\n"); 
    while (shmid <= 0) {
        if ((shm = CLI_shmGetAdd ("pangfits", &shmsize, &shmid,  &shmqid)) == NULL) {
                printf ("error getting segment (%d) \r", shmid);
        }
	if (!running)
		goto bye;
        sleep (1);
    }
    printf ("\nshmid  %d shmqid: %d\n", shmid, shmqid);
    FS_set_host ("localhost", 5001);

    ready = 1;
    while (running) {
        qtype = 0;
        printf ("\nwaiting message ...\n");
        if (CLI_shmGetMsg (shmqid, qmsg, &qtype, 1) < 0){
    //            running = 0;
                qtype = CMD;
                sprintf (qmsg, "SHMEM DESTROY");
        }
        if (qtype == DATA){
                sscanf (qmsg, "%s %ld %ld", arg1, &imsize, &bufcnt);
                printf ("message from queue: (type %ld) %s\n", qtype, qmsg);
                printf ("shmsize %d imsize: %ld, bufcnt: %ld\n", shmsize, imsize, bufcnt);
		if (bufcnt < 0)
			bufcnt = 0;
                nextbuf = shm + imsize*bufcnt;
                FS_SendFits ((unsigned long) nextbuf, imsize);
        } else {
                printf ("main received cmd %s\n", qmsg);
                sscanf (qmsg, "%s %s", arg1, arg2);
                printf ("%s\n", arg2);
                if (strncmp (arg2, "DESTROY", 7) == 0) {
    			CLI_shmDettach (shm);
			sleep (5);
			shmid = 0;
			goto init;
		} else { 
                	if (strncmp (arg1, "SHUTDOWN", 8) == 0)
                        	running = 0;
		}
        }
    }
bye:
    CLI_shmDettach (shm);
    running = 0;
    printf ("main thread stoped\n");
    pthread_join( thread1, NULL);

    return(0);
}


void *GetCmd()
{
#if 0
    while ((running) && (!ready)){
	sleep (1);
    }
#endif
    printf ("tcp thread started\n"); 
    tcp_server ();
    printf ("tcp thread stoped\n"); 
   return (0);
}
