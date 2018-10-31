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
#define DHE SHMNAME "pandhe"
#define MSGNEWIMG 2
#define MSGSETPARS 3
#define MSGSHM 1

typedef struct pandhe_msgbuf {
	int mtype;
	void *bufptr;
	int ncols;
	int nrows;
	int bpp;
} pqmsg_t;

int DHE_shmDestroy (char *name, void *buffer, int id, int qid);
void *DHE_shmAttach (size_t *psize, char *name, void *buffer, int *shmid, int *qid);
void DHE_shmSendMsg (int qid, long type, int ncols, int nrows, int bpp);
