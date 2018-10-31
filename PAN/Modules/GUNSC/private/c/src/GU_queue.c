#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "GU_queue.h"


qmsg_t message;

int GUQ_GetRef (char *name)
{
char fname[64];
FILE *lockFile = (FILE *) NULL;
key_t key;

        (void) memset((void *)&fname[0],'\0',64);
        (void) sprintf(fname,"/tmp/%s.LCK",name);

        if ((lockFile=fopen(fname,"r")) == (FILE *) NULL)
                return (-errno);

        (void) fprintf(lockFile,"%ld:\t", (long) getpid());
        fflush(lockFile);
        (void) fclose(lockFile);
        if ((key=ftok(fname, (strlen(fname) & 0xFF))) < 0)
                return (-errno);
        printf ("getkey returned key %d\n", (int) key);
        return (msgget (key, 0666));
}

int GUQ_GetMsg (int qid, char *msg, long *type, int wait)
{
int msgflag;
int ret;

	if (wait)
		msgflag = 0;
	else
		msgflag = IPC_NOWAIT;
	msg[0] = message.msg[0] = '\0';
//	printf ("qid %d\n", qid);
	ret = msgrcv (qid, &message, sizeof (message), 0, msgflag);
	if (ret>0){
		strcpy (msg, message.msg);
		*type = message.mtype;
		return (0);
	} else
		return (ret);
		
//	printf ("after recmsg (%s), type %ld, len %d\n", msg, *type, strlen (message.msg));
}

void GUQ_SendMsg (int qid, char *msg, long type)
{
qmsg_t message;
int len;

        if (strlen (msg) > MAXLEN)
                len = MAXLEN;
        else
                len = strlen (msg);

        strncpy (message.msg, msg, len);
        message.mtype = type;
        msgsnd (qid, &message, sizeof (message), 0);
        printf ("message sent: qid %d, message %s, type %ld (len: %d)\n", qid, message.msg, type, len);
}

int GUQ_WaitMessages (int qid)
{
long qtype;
char qmsg[MAXLEN];
char arg1[10];
int running;

    while (running) {
        qtype = 0;
        printf ("\nwaiting message ...\n");
        if (GUQ_GetMsg (qid, qmsg, &qtype, 1) < 0){
    //            running = 0;
                qtype = CMD;
                sprintf (qmsg, "command received");
        }
        if (qtype == DATA){
                sscanf (qmsg, "%ld", &fmptr);
                printf ("message from queue: (type %ld) %s\n", qtype, qmsg);
                printf ("frameptr: %ld\n", frameptr);
                if (bufcnt < 0)
                        bufcnt = 0;
        } else {
                printf ("gunsc received cmd %s\n", qmsg);
                sscanf (qmsg, "%s", arg1);
                printf ("%s\n", arg1);
                if (strncmp (arg1, "STOP", 4) == 0) 
			running =0;
	}
bye:
	return (OK);
}

