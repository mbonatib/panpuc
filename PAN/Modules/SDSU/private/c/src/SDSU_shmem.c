#include "SDSU_shmem.h"

/**
* Init shared memory segment
@ param[init *fname name of the memroy segment (ID)
**/
key_t SDSU_shmInit (char *fname)
{
FILE *lockFile = (FILE *) NULL;
int posit = 0;
key_t key=-1;
int new=0;
        if ((lockFile=fopen(fname,"a")) == (FILE *) NULL)
                return (-errno);
	printf ("shmInit: after fopen\n");
        if ((posit = (int)ftell((FILE *)lockFile)) == 0) {
                new = 1;
        }
        (void) fprintf(lockFile,"%ld:\t", (long) getpid());
        fflush(lockFile);
        (void) fclose(lockFile);
        if ((key=ftok(fname, (strlen(fname) & 0xFF))) < 0)
                return (-errno);
	printf ("shmInit returned key %d\n", (int) key);
        return (key);
}

/**
* Checks size of the shared memory segment to be sure it is multplie of the page size
**/
size_t SDSU_shmChkSize (size_t insize, size_t base)
{
int outsize;

        outsize = insize;
        if ((insize==0) || (base==0))
                return (-EINVAL);
        if ( ! DIVIDES(base,insize) ) outsize = ((insize/base+1L)*base);
        return (outsize);
}

/**
* Destroys shared memory segment
**/
int SDSU_shmDestroy (char *name, void *buffer, int id, int qid)
{
char fname[64] = {'\0'};
int lockFile = 0;
struct shmid_ds _detBuf;

        if ((name == NULL) || (buffer == NULL))
                return (-EINVAL);

        if (shmctl(id,IPC_STAT,&_detBuf) < 0)
                return (-errno);
        if (shmdt (buffer) < 0)
                return (-errno);
        if (shmctl(id,IPC_RMID,&_detBuf) < 0)
                return (-errno);

	printf ("shmdestroy: after shmctl\n");
        (void) memset((void *)&fname[0],'\0',64);
        (void) sprintf(fname,"/tmp/%s.LCK",name);
	printf ("shmdestroy: about to delete %s\n", fname);
        if ((lockFile=unlink(fname)) < 0)
                return (-errno);
	msgctl (qid, IPC_RMID, NULL);
        return (OK);
}

/**
* Attach shared memory buffer and return ID of the segment
* @param[out]  *psize as input, the desired size, as output the ctual attached size
* @param[in] attached shared memory buffer
* @param[out] *shmid shared memory ID out
* @param[out] *qid shared meory queue ID out
**/
void *SDSU_shmAttach (size_t *psize, char *name, void *buffer, int *shmid, int *qid)
{
key_t key= (key_t) 0;
int shmFlag = IPC_CREAT | 0666;
char fname[64];
FILE *lockFile = (FILE *) NULL;
void *shmadd=NULL;
size_t size;

        (void) memset((void *)&fname[0],'\0',64);
        (void) sprintf(fname,"/tmp/%s.LCK",name);

        if ((key=SDSU_shmInit (fname)) < 0){
		*shmid = key;
                return (NULL);
	}
	size = *psize;
	printf ("shmAttach: after Init (key %d, size %ld\n", (int) key, (ulong)size);
#if LINUX_VERSION_CODE >= KERNEL_VERSION (2,6,30)
	size = SDSU_shmChkSize (size, getpagesize());
#else
	size = SDSU_shmChkSize (size, PAGE_SIZE);
#endif
	*psize = size;
	printf ("shmAttach: after chksize, size %ld\n", (ulong)size);
        if ((*shmid = shmget (key, size, shmFlag)) < 0){
		*shmid = -errno;
                return (NULL);
	}
	printf ("shmAttach: after shmget\n");
        if ((shmadd = shmat (*shmid, buffer, 0)) < 0){
		*shmid = -errno;
                return (NULL);
	}
	printf ("shmAttach: after shmatt\n");
        if ((lockFile=fopen(fname,"a")) == (FILE *) NULL){
		*shmid = -errno;
                return (NULL);
	}
        (void) fprintf(lockFile,"%ld, %ld\n", (long)*shmid, (long)size);
        fflush(lockFile);
        (void) fclose(lockFile);
	*qid = msgget (key, shmFlag);
	msgctl (*qid, IPC_RMID, NULL);
	*qid = msgget (key, shmFlag);
	printf ("shmAttach: returned add %ld, id %d\n", (long) shmadd, *shmid);
        return (shmadd);
}

/**
* Sends a message trhough shared memory queue
* @param[in] qid shared memory queue ID
* @params *msg string message to send
* @param[in] type message type to send
**/
void SDSU_shmSendMsg (int qid, char *msg, long type)
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
//	printf ("message sent: qid %d, message %s, type %ld (len: %d)\n", qid, message.msg, type, len);
}

void SDSU_shmSendNewImg (int qid, int bufcnt, int bpp, int columns, int rows)
{
char msg[512];
	sprintf (msg, "%d %d %d %d", bufcnt,bpp,columns,rows);
//	printf ("sending %s\n", msg);
	return (SDSU_shmSendMsg (qid, msg, MSGNEWIMG));
}
