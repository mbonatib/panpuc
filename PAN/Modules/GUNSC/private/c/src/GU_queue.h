#define MAXLEN 2048
#define CMD     1
#define DATA    2

typedef struct pangfits_msgbuf {
        long mtype;
        char msg[MAXLEN];
} qmsg_t;
