#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fitsio.h"

#define OK		0

int MI_single (char *impath, unsigned long imptr, long *retcols, long *retrows, int *bpp, int *naxis)
{
fitsfile	*image_fd;
void *image_addr;
int stat;
long ncols, nrows;
long sizes[3];
char err_text[32];
unsigned long tot_pixels;
int datatype;
int bitpix=0;
int naxs=0;

	stat = 0;
	image_fd = NULL;
//	datatype = TUSHORT;
//	bitpix = USHORT_IMG;
	image_addr = (void *) imptr;
	if (image_addr == NULL)
		return (-EINVAL);

	printf ("opening file %s\n", impath);
        fits_open_file (&image_fd, impath, READONLY, &stat);
        if (image_fd == NULL){
		sprintf (impath, "%s.fits", impath);
		fits_open_file (&image_fd, impath, READONLY, &stat);
		if (image_fd == NULL){ 
			printf ("error %d opening image %s ; Exiting ...\n", stat, impath);
			fits_get_errstatus (stat, err_text);
			printf ("%s\n", err_text);
                	return (-stat);
		}
	}

	fits_get_img_dim (image_fd, &naxs, &stat);
	fits_get_img_size (image_fd, naxs, sizes, &stat);
	fits_get_img_type (image_fd, &bitpix, &stat);
	ncols = sizes[0];
	nrows = sizes[1];
	*retcols = ncols;
	*retrows = nrows;
	*bpp = bitpix;
	*naxis = naxs;
	tot_pixels = ncols*nrows;
	if (bitpix == 32)
		datatype = TULONG;
	else
		datatype = TUSHORT;
//	printf ("cols = %ld, rows = %ld bpp =%d naxis = %d\n", ncols, nrows, bitpix, naxs);
	if (tot_pixels <= 0){
		printf ("wrong value. Exiting ...\n");
		return (-1);
	}

	fits_read_img (image_fd, datatype, 1, tot_pixels, 0,  image_addr, 0, &stat);
	if (stat < 0) {
		printf ("error %d\n", stat);
		return (stat);
	}
	printf ("MI_single: OK, ciao\n");

        return (OK);
}

