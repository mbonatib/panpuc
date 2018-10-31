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


qmsg_t message;

int CLI_shmGetKey (char *name)
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
        return (key);
}

void *CLI_shmGetAdd (char *name, int *size, int *id, int *qid)
{
key_t key;
void *shm;
struct shmid_ds st;

    if ((key = CLI_shmGetKey (name)) < 0){
	*id = key;
	return (NULL);
    }
    printf ("shmGetAdd: got key %d\n", (int) key);
	
    if ((*id = shmget(key, *size, 0666)) < 0) {
        perror("shmget");
	*id = -errno;
        return(NULL);
    }
    shmctl (*id, IPC_STAT, &st);
    *size = st.shm_segsz;
    printf ("shmGetAdd: got id %d\n", *id);
    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(*id, NULL, 0)) == (char *) -1) {
        perror("shmat");
	*id = -errno;
        return(NULL);
    }
    *qid = msgget (key, 0666);
    printf ("shmGetAdd: returned add %d\n", (int) shm);
    return (shm);
}

void CLI_shmDettach (void *add)
{
	shmdt (add);
}

int CLI_shmGetMsg (int qid, char *msg, long *type, int wait)
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
