#define MAXLEN    50

typedef struct pangfits_msgbuf {
        long mtype;
        char msg[MAXLEN];
} qmsg_t;


int CLI_shmGetKey (char *name);
void *CLI_shmGetAdd (char *name, int *size, int *id, int *qid);
void CLI_shmDettach (void *add);
int CLI_shmGetMsg (int qid, char *msg, long *type, int wait);
void CLI_shmSendMsg (int qid, char *msg, long type);
