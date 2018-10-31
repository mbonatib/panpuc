#define OK 0
#define MAXDETS 4
#define EXTPERDET 2
#define XDATASIZE 1024
#ifdef _DEBUG_
#  define pdebug(fmt, args...) printf ("RMASK: " fmt, ## args)
#else
#  define pdebug(fmt, args...)
#endif


int RMASK_GetBuffer (int nelems, int bpp);
int RMASK_GetImage (char *impath, int delimg, int display, int overcorr, int outut);
void RMASK_Close (void);
