#ifdef _DEBUG_
#  define pdebug(fmt, args...) printf ("DSENDER: " fmt, ## args)
#else
#  define pdebug(fmt, args...)
#endif

#define OK      0


typedef
struct fpConfig_struct{
	unsigned int xSize;	
	unsigned int ySize;	
} fpConfig_t;

typedef unsigned int dhsHandle;


/***prototypes****/
int DHS_connect (char *host, int port);
int DHS_close (int fd);
int DHS_openExp (int fd, char *datastr);
int DHS_closeExp (int fd, char *expID);
int DHS_send_metadata (int fd);
void DHS_SendPixelData ( long *istat,     /* inherited status               */
			    char *resp,      /* response message               */
			    dhsHandle dhsID, /* dhs handle                     */
			    void *pxlAddr,   /* address of data block          */
			    size_t blkSize,  /* size of data block             */
			    fpConfig_t *cfg, /* configuration of pixel data    */
			    double *expID,   /* exposure identifier            */
			    char *obsetID    /* observation set identifier     */
    ); 
int DHS_sendPixels (int fd, unsigned long databuff, int ncols, int nrows, int bpp);
