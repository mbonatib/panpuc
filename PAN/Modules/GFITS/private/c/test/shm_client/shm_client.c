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
#include "fitsio.h"
#include "longnam.h"
#include "cdl.h"


short wfits=1;
#ifdef _DISPLAY_
short display=1;
#endif
short process=1;
int outbufsize=0;
void *pix=NULL;


int Allocate_local_buffer (int size)
{
       if (outbufsize < size){
                printf ("reallocating outbuf ...");
                free (pix);
                pix = malloc (size);
                if (pix != NULL) {
                        outbufsize = size;
                } else {
                        outbufsize = 0;
                        printf ("null outbuf, pixels not read\n");
                        return (-ENOMEM);
                }
                printf (" %d\n", outbufsize);
        }
	return (0);
}



int GetPixels (fitsfile *fptr, int *bpp, long *nax1, long *nax2, void *pbuf)
{
int status=0;
int bitpix;
int naxis=0;
long naxes[2] = {1,1};
int datatype;
long fpixel[2]={1,1};
int nelems;

    if (pix == NULL) 
	return (-EINVAL);

    fits_get_img_param(fptr, 2,  &bitpix, &naxis, naxes, &status);
    nelems = naxes[0] * naxes[1];
    printf ("bitpix %d, naxis %d, naxes[0] %ld, naxes[1] %ld, nelems %d status %d\n", bitpix, naxis, naxes[0], naxes[1], nelems, status);

    if (bitpix == 16)
        datatype = TUSHORT;
    else
        datatype = TULONG;

    if (pbuf != NULL) {
    	  fits_read_pix (fptr, datatype, fpixel, nelems , NULL , pbuf, NULL, &status);
    }
    *bpp = bitpix;
    *nax1 = naxes[0];
    *nax2 = naxes[1];
    return (0);
}



int ProcImage (fitsfile *fptr, void *pbuf)
{
long ax1, ax2;
int bitpix;
int ret;

	if ((ret=GetPixels (fptr, &bitpix, &ax1, &ax2, pbuf)) < 0){
		return (ret);
	}
	printf ("Processing Image ...\n");
        printf ("pixel[0]= %d;\n", *(unsigned int *)pbuf);
        printf ("process finished\n");
	return (0);
}

int DisplayImage (fitsfile *fptr, void *pbuf)
{
#ifdef _DISPLAY_
int bitpix;
long ax1=0;
long ax2=0;
int ret;
CDLPtr cdl;

    cdl = cdl_open ("fifo:/dev/imt1i:/dev/imt1o");
    if ((CDLPtr) cdl != NULL) {
        if ((ret=GetPixels (fptr, &bitpix, &ax1, &ax2, pbuf)) < 0){
		return (-ENOMEM);
       }
        cdl_displayPix (cdl, (uchar *)pbuf, ax1, ax2, bitpix, 1, FB_AUTO, 1);
        cdl_close (cdl);
    } else {
        printf ("no display found\n");
    }
#endif
    return (0);
}



int Get_Image (void *buff, int size)
{
fitsfile *fptr, *fout;
int status=0;
size_t ssize;

    ssize = (size_t) size;
    printf ("Getting image ...\n");
    fits_open_memfile (&fptr, "whatever", READONLY, &buff, &ssize,0,0,&status);
    if (status != 0){
	printf ("error %d opening memfile\n", status);
	return (status);
    }
    if (Allocate_local_buffer (size) < 0)
	return (-ENOMEM);

    if (process) {
	ProcImage (fptr, pix);
    }
#ifdef _DISPLAY_
    if (display) {
    	DisplayImage (fptr, pix);
    }
#endif
    if (wfits) {
        fits_create_file (&fout, "test.fits", &status);
        fits_copy_file (fptr, fout, 1,1,1,&status);
        fits_close_file (fout, &status);
    }
    fits_close_file (fptr, &status);
    return (0);
}



int main()
{
    
    int shmid=0;
    int shmqid=0;
    long qtype;
    char *shm=NULL;
    char *nextbuf=NULL;
    char qmsg[MAXLEN];
    short running = 1;
    char arg1[10];
    char arg2[10];
    int shmsize;
    long imsize;
    long bufcnt=0;

    printf ("started\n");
    while (shmid <= 0) {
    	if ((shm = CLI_shmGetAdd ("pangfits", &shmsize, &shmid,  &shmqid)) == NULL) {
		printf ("error getting segment (%d) \r", shmid);
	}
	sleep (1);
    }
    printf ("\nshmid  %d shmqid: %d\n", shmid, shmqid);

    while (running) {
	qtype = 0;
    	printf ("\nwaiting message ...\n");
    	if (CLI_shmGetMsg (shmqid, qmsg, &qtype, 1) < 0){
		running = 0;
		qtype = 1;
		sprintf (qmsg, "SHMEM DESTROY");
	}
	if (qtype == 2){
		sscanf (qmsg, "%s %ld %ld", arg1, &imsize, &bufcnt);
    		printf ("message from queue: (type %ld) %s\n", qtype, qmsg);
    		printf ("shmsize %d imsize: %ld, bufcnt: %ld\n", shmsize, imsize, bufcnt);
		nextbuf = shm + imsize*bufcnt;
		Get_Image (nextbuf, imsize);
   	} else {
		printf ("%s\n", qmsg);
		sscanf (qmsg, "%s %s", arg1, arg2);
		printf ("%s\n", arg2);
		if (strcmp (arg2, "DESTROY") == 0)
			running = 0;
	}
    }

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
//    *shm = '*';

    CLI_shmDettach (shm);
    return(0);
}
