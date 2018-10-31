/*******************************************************************************
 * include(s):
 *******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include "DHS_ctcp_defs.h"

int sockfd=0, n;
struct sockaddr_in serv_addr;
struct hostent *server;

char buffer[256];
int pData;
//int pData[1000000];
char svradd[80];
int svrport=0;
int ntries=2;

int connected = 0;

int DHS_set_host (char *host, int port)
{
	strcpy (svradd, host);
	svrport = port;
	return (0);
}

int DHS_connect (char *host, int port)
{

    if ((connected) && (sockfd != 0)) {
	printf ("dhs_connect: already connected to %s, port %d\n", svradd, host);
	return (sockfd);
    }
    if (sockfd != 0)
	DHS_close (sockfd);

    DHS_set_host (host, port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server = gethostbyname(svradd);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(svrport);
    connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    connected = 1;
    printf ("DRV_DHS(connect): connected to %s, port %d\n", svradd, svrport);
    return (sockfd);
}

int DHS_close (int fd)
{
	close (sockfd);
	connected = 0;
	sockfd = 0;
	return (OK);
}

int DHS_openExp (int fd, char *datastr)
{
return (OK);
}

int DHS_closeExp (int fd, char *expID)
{
return (OK);
}

int DHS_send_metadata (int fd)
{
return (0);
}
 
/*******************************************************************************
 * dhsUtilSendPixelData ( ... )
 *******************************************************************************/ 
void DHS_SendPixelData ( long *istat,     /* inherited status               */
			    char *resp,      /* response message               */
			    dhsHandle dhsID, /* dhs handle                     */
			    void *pxlAddr,   /* address of data block          */
			    size_t blkSize,  /* size of data block             */
			    fpConfig_t *cfg, /* configuration of pixel data    */
			    double *expID,   /* exposure identifier            */
			    char *obsetID    /* observation set identifier     */
    ) 
{
    /* declare some variable and initialize them */
    long nelms=0L;
    long naxes[2] = { 0L, 0L };
    int errcnt=0;	
    long nsent=0;
    long bytes_pp, nbytes;

    /* check the inherited status */
    if ( (*istat) < 0 ) return;
    strcpy (resp,"\0");
    
    /* check input parameters */
    if ( pxlAddr==(void *)NULL || expID==(double *)NULL) {
	*istat = -EFAULT;
	sprintf(resp,"dhsSendPixelData: Bad parameter. %s",
			 		((expID==(double *)NULL) ? "expID ptr==NULL." : "FP config ptr == NULL."));
	strcpy (resp,"ERROR null pointer\n");
	return;
    }

    /* check input values */
    if ( dhsID<(dhsHandle)0 || blkSize<=(size_t)0) 
    {
	*istat = -EFAULT;
	sprintf(resp,"dhsSendPixelData: %s",((dhsID<(dhsHandle)0) ? "dhsHandle invalid" : "blkSize <= 0."));
	strcpy(resp,"ERROR invalid handle\n");
	return;
    }


    /* set output size */
    naxes[0] = cfg->xSize;
    naxes[1] = cfg->ySize;
    nelms = naxes[0] * naxes[1];
    errcnt = 0;

    nsent = 0; 
again:
    errcnt++;    
    if (!connected)
	DHS_connect(svradd, svrport);

    sprintf(buffer,"data %ld",(long)(cfg->xSize * cfg->ySize));
    n = write(sockfd,buffer,strlen(buffer));
    
    usleep (100000);
    int k;
    long *nptr;
    nptr = (long *)pxlAddr;
    bytes_pp = blkSize / 8;
    nbytes = cfg->xSize * cfg->ySize * bytes_pp;
    printf("DHS_ctcp: Sending %ld bytes (%d x %d, bpp %d)\n", (long)nbytes, cfg->xSize, cfg->ySize, bytes_pp); 
#if 0
    for(k = nsent; k < (long)(cfg->xSize * cfg->ySize); k++){
      pData = *(nptr+k);
      n = write(sockfd,&pData,sizeof(pData));
	usleep (50000);
//      n = write(sockfd,pData,4);
      printf ("DHS_ctcp: Sent pixel %d\n", k+1);
      if (n < 0) {
    	printf ("DHS_ctcp: ERROR Sending pixels (%d)\n", errcnt);
	DHS_close (sockfd);
	if (errcnt >= ntries) {
		*istat = -errno;
    		strcpy (resp, "ERROR Sending pixels\n");
    		printf ("DHS_ctcp: ERROR Sending pixels\n");
		return;
	} else {
    		printf ("DHS_ctcp: trying to reconnect\n");
		goto again;
	}
      }
      nsent += n; 
    }
#else
	void *ptr;
	ptr = (void *) pxlAddr;
send_again:
        printf ("DHS_ctcp: Sending pixels (%ld bytes)\n", nbytes - nsent);
	n = write(sockfd,(ptr + nsent),(nbytes-nsent));	
        printf ("DHS_ctcp: after Sending pixels)\n");
	if (n < 0) {
    		printf ("DHS_ctcp: ERROR Sending pixels (%d)\n", errcnt);
		DHS_close (sockfd);
		errcnt++;
        	if (errcnt >= ntries) {
                	*istat = -errno;
             	   	strcpy (resp, "ERROR Sending pixels\n");
            	    	printf ("DHS_ctcp: ERROR Sending pixels\n");
              	    	return;
        	} else {
                	printf ("DHS_ctcp: trying to reconnect\n");
			DHS_connect(svradd, svrport);
                	goto send_again;
        	}
      	}
      	nsent += n;
	if (nsent < nbytes)
		goto send_again;
#endif
    			  
    /* return */
    strcpy (resp, "DONE Success\n");
    printf ("DHS_ctcp: Success Sending pixels (%ld bytes)\n", nsent);
    return;
}

int DHS_sendPixels (int fd, unsigned long databuff, int ncols, int nrows, int bpp)
{
long stat;
char resp[100];
char obsetID[100];
fpConfig_t cfg;
double expID;
int dhsID;

	dhsID = 0;
	cfg.xSize = ncols;
	cfg.ySize = nrows;

	DHS_SendPixelData (&stat, resp, dhsID, (void *) databuff, (size_t) bpp, &cfg, &expID, obsetID);

	return (stat);
}
