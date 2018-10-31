#include <sys/types.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "fitsio.h"
#include "cdl.h"
#include "longnam.h"

char tmpstr[150];
int nextimg = 0;
void *pbuf=NULL;
void *imbuf=NULL;
unsigned long pbufsize = 0;
#define OK 0
extern int delimg;
extern int display;
extern CDLPtr cdl;

int RMASK_ParseSec (char *sec, int *xs, int *xe, int *ys, int *ye)
{
const char delim[] ="'[:,]";
char *tok=NULL;

	*xs = *xe = *ys = *ye = -1;	
	tok = strtok (sec, delim);
	if (tok != NULL)
		*xs = atoi (tok);
	else
		return (OK);
	tok = strtok (NULL, delim);
	if (tok != NULL)
		*xe = atoi (tok);
	else
		return (OK);
	tok = strtok (NULL, delim);
	if (tok != NULL)
		*ys = atoi (tok);
	else
		return (OK);
	tok = strtok (NULL, delim);
	if (tok != NULL)
		*ye = atoi (tok);

	return (OK);
}

int RMASK_GetBuffer (int nelems, int bpp)
{
unsigned long rsize;

	rsize = nelems * bpp;
	if (pbufsize < rsize){
//		printf ("allocating buffer of %d bytes (nelems %d, bpp %d)\n", rsize, nelems, bpp);
		if (pbuf != NULL);
			free (pbuf);
		pbuf = NULL;
		pbuf = malloc (rsize);
		if (pbuf == NULL)
			return (-ENOMEM);
		pbufsize = rsize;
	}
	return (OK);
}
		
int RMASK_GetImage (char *impath)
{
fitsfile *fptr;
fitsfile *outfptr;
int status=0;
int bitpix;
int naxis=0;
long naxes[2] = {1,1};
int datatype;
long fpixel[2]={1,1};
long inc[2]={1,1};
long lpixel[2]={0,0};
char sec[60];
//char outname[256];
char *outname;
char outpath[256];
char outsync[256];
char impath2[256];
int nelems;
int ret=0;
FILE *sfd;
int bpp;
int dxs, dxe, dys, dye, rxs, rxe, rys, rye;
int xs, xe, ys, ye;

    printf ("Getting image ...\n");
    fits_open_file (&fptr, impath, READONLY, &status);
    if (status != 0){
	printf ("error %d opening file %s\n", status, impath);
	return (-status);
    }
   fits_get_img_param(fptr, 2,  &bitpix, &naxis, naxes, &status);
   if (status != 0){
	ret = -status;
	goto bye_img;
    }
    switch (bitpix){
		case SHORT_IMG: 
			datatype = TSHORT;
			break;
		case USHORT_IMG: 
			datatype = TUSHORT;
			break;
		case LONG_IMG: 
			datatype = TLONG;
			break;
		case ULONG_IMG: 
			datatype = TULONG;
			break;
		case FLOAT_IMG: 
			datatype = TFLOAT;
			break;
		default:
			datatype = bitpix;
    }
    nelems = naxes[0] * naxes[1];
    bpp = abs(bitpix/8);
    if ((ret = RMASK_GetBuffer (nelems, bpp)) < 0){
	goto bye_img;
    }
    fits_read_keyword (fptr, "RSEC1", sec, NULL, &status);
    if (status != 0){	/*no roisection1 defined*/
	rxs = 0;
	status = 0;
    } else {
    	RMASK_ParseSec (sec, &rxs, &rxe, &rys, &rye);
    	printf ("roisec1 = %d %d %d %d\n", rxs, rxe, rys, rye);
    }
    fits_read_keyword (fptr, "RSEC2", sec, NULL, &status);
    if (status != 0){	/*no roisection2 defined*/
	dxs = 0;
	status = 0;
    } else {		/*2 extensions*/
    	RMASK_ParseSec (sec, &dxs, &dxe, &dys, &dye);
    	printf ("roisec2 = %d %d %d %d\n", dxs, dxe, dys, dye);
    }
    if (rxs > 0){		/*roisection1 ok*/
	xs = rxs;
	ys = rys;
	if (dxs > 0){		/*roisection2 ok*/
		xe = dxe;
		ye = dye;
	} else {		/*no roisection2 defined, use only roisection1*/
		xe = rxe;
		ye = rye;
	}
    } else {
	if (dxs > 0){		/*roisection1 no defined, use roisection2*/
		xs = dxs;
		xe = dxe;
		ys = dys;
		ye = dye;
	} else {		/*roisections1 and 2 are invalid. Use the full frame*/
		xs = 1;
		ys = 1;
		xe = naxes[0];
		ye = naxes[0];
	}
    }
    printf ("roi = %d %d %d %d\n", xs, xe, ys, ye);
    fpixel[0] = xs;
    fpixel[1] = ys;
    lpixel[0] = xe;
    lpixel[1] = ye;
    naxes[0] = (xe - xs + 1);
    naxes[1] = (ye - ys + 1);
    nelems = naxes[0] * naxes[1];
    strcpy (impath2, impath);
    outname = strtok (impath2, ".");
    sprintf (outpath, "%s", outname);
    sprintf (outsync, "%s", outname);
    strcat (outpath, "_roi.fits");
    strcat (outsync, "_roi.DONE");
    printf ("%s\n", outpath);
    fits_read_subset (fptr, datatype, fpixel, lpixel, inc, NULL, pbuf, NULL, &status);
    if (status != 0){
	ret = -status;
	goto bye_img;
    }
    fits_create_file (&outfptr, outpath, &status);
    if (status != 0){
	ret = -status;
	goto bye_img;
    }
    fits_create_img (outfptr, bitpix, 2, naxes, &status);
    if (status != 0){
    	fits_close_file (outfptr, &status);
	ret = -status;
	goto bye_img;
    }
    fits_write_img (outfptr, datatype, 1, nelems, pbuf, &status);
    if (status != 0){
    	fits_close_file (outfptr, &status);
	ret = -status;
	goto bye_img;
    }
    fits_close_file (outfptr, &status);
    if (status != 0){
	ret = -status;
	goto bye_img;
    }
bye_img:	
    fits_close_file (fptr, &status);
    if (display){
		if (cdl != NULL)
			cdl_displayFITS(cdl, outpath, 1, FB_AUTO, -1);
    }
    if (ret == 0) {
	printf ("outsync %s\n", outsync);
	sfd = fopen (outsync, "w+");
	fclose (sfd);
	if (delimg) {
    		if (unlink (impath)< 0)
			ret = -errno;
	}
    }

    printf ("returned %d\n", ret);
    return (ret);
}

void RMASK_Close (void)
{
	if (pbuf != NULL){
		free (pbuf);
		pbufsize = 0;
	}
}
