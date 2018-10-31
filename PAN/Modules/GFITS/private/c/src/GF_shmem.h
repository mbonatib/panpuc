#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE <= KERNEL_VERSION (2,6,30)
#include <asm/page.h>
#endif
#include <sys/stat.h>
#include <string.h>

#define DIVIDES(a,n) (!((n) % (a)))
#define SHMSZ     27
#define OK     0
#define MAXLEN 30
#define SHM_NAME "pangfits"
#define MSGNEWFITS 2
#define MSGSHM 1

typedef struct pangfits_msgbuf {
	long mtype;
	char msg[MAXLEN];
} qmsg_t;

int GF_shmDestroy (char *name, void *buffer, int id, int qid);
void *GF_shmAttach (size_t *psize, char *name, void *buffer, int *shmid, int *qid);
void GF_shmSendMsg (int qid, char *msg, long type);
