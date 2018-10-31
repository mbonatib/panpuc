#define OK 0
#define MAXDETS 1
#define EXTPERDET 4
#define XDATASIZE 512
#ifdef _DEBUG_
#  define pdebug(fmt, args...) printf ("RMASK: " fmt, ## args)
#else
#  define pdebug(fmt, args...)
#endif


int RMASK_GetBuffer (int nelems, int bpp);
int RMASK_GetImage (char *impath, int delimg, int display, int overcorr, int outut);
void RMASK_Close (void);
