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
#include <dlfcn.h>
#include "../../public/c/include/GUnscrambling.h"
#define OK		0
#define LL		0
#define LR		1
#define UL		2
#define UR		3
#define DISPLAY		1

int main (int argc, char *argv[])
{
unsigned long map_addr=0, unsc_addr=0;
int writeprog;
char *filename, *outfilename;
char detsec[30];
long ncols=0, nrows=0;
int file_d;
int datatype, bitpix, i, naxis;
int bpp=0;
int nelems = 0, bytes_pp = 2;
int maxX, maxY;
int ret=OK;
int namp=4;
int mext=1;
amp_t amp[4];
int Extension[4];
unsigned long amplifiers;
int dref, pieces;
unsigned long off;
	if (argc < 2){
	         printf ("falta argumentos\n");
	         return (-1);
	}					        

	filename = argv[1];
	outfilename = argv[2];
	maxX = 4096;
	maxY = 4096;

	printf ("getting mem\n");
	map_addr = (unsigned long) malloc (maxX*maxY*bytes_pp);

	printf ("about to map\n");
	if ((ret= MI_single (filename, map_addr, &ncols, &nrows, &bitpix, &naxis))<0)
			return (ret);
	printf ("map returned: cols = %ld, rows = %ld bpp =%d naxis = %d\n", ncols, nrows, bitpix, naxis);
	if (bitpix == 32) {  	/*LONG_IMG*/
		datatype = TLONG;
		printf ("datatype1 = %d\n", datatype);
		bpp = bitpix;
		bytes_pp = 4;
	} else {		/*USHORT_IMG*/
		datatype = TUSHORT;
		bpp = bitpix;
		bytes_pp = 2;
	}
	amp[0].xstart = 1;
	amp[0].ystart = 1;
	amp[0].ncols = ncols/2;
	amp[0].nrows = nrows/2;
	amp[0].type =LR;
	amp[0].flip = 0;
	amp[0].rotate = 0;
	Extension[0] = 1;		/*map second amplifier to first ext*/

	amp[1].xstart = 1025;
	amp[1].ystart = 1;
	amp[1].ncols = ncols/2;
	amp[1].nrows = nrows/2;
	amp[1].type = LR;
	amp[1].flip = 0;
	amp[1].rotate = 90;
	Extension[1] = 0;		/*map first amplifier to second extension*/

	amp[2].xstart = 1;
	amp[2].ystart = 1025;
	amp[2].ncols = ncols/2;
	amp[2].nrows = nrows/2;
	amp[2].type = LL;
	amp[2].rotate = -90;
	amp[2].flip = 0;
	Extension[2] = 2;

	amp[3].xstart = 1025;
	amp[3].ystart = 1025;
	amp[3].ncols = ncols/2;
	amp[3].nrows = nrows/2;
	amp[3].type = UL;
	amp[3].rotate = 0;
	amp[3].flip = 0;
	Extension[3] = 3;

	nelems = ncols*nrows;
	unsc_addr = (unsigned long) malloc (nelems*bytes_pp);	
	printf ("bytes_pp %d\n", bytes_pp);
	if ((ret = GU_open ()) < 0)
		return (ret);
	printf ("datatype2 = %d\n", datatype);
	if ((ret = GU_compute (bpp)) < 0)
		return (ret);
	if ((ret = GU_call_clear_amps ()) < 0)
	        return (ret);

	for (i=0 ; i<namp; i++)
		if ((ret= GU_call_add_amp (amp[i].xstart, 0, amp[i].ncols , 0, amp[i].ystart, 0, amp[i].nrows, 0, 1, 1, amp[i].type , amp[i].rotate, amp[i].flip, Extension[i]))<0)
			     return (ret);

	GU_call_show_amps ();
	amplifiers = GU_call_get_amps_ptr ();
	if (bpp == 32)
		datatype = 41;
	else
		datatype = 20;
	GF_init (datatype, 0, mext, amplifiers, namp, ncols, nrows);
        if ((file_d = GF_create_img (outfilename, datatype, 0, mext, amplifiers)) <= 0) {
                 printf ("no pude crear file (retruned %d", file_d);
		 goto clean;
	}
        printf ("writing image from buffer %ld\n", unsc_addr);

	if ((dref=GRTD_compute (outfilename, bpp, 1,  amplifiers, namp, ncols, nrows, ncols*nrows*bytes_pp)) < 0)
		return (dref);
	pieces= 8;
	for (i=0; i<pieces; i++) {
		off = (unsigned long)((double)i*(double)bytes_pp*(double)nelems/pieces);
		printf ("dummy reading piece %d (off %ld)\n", i+1, off);

		if ((amplifiers=(unsigned long)GU_call_unscrambling (map_addr+off, unsc_addr, (int)(double)nelems/pieces)) < 0)
			return (amplifiers);	
        	if ((ret=GF_writeImage (file_d, amplifiers, (int)(double)nelems/pieces, &writeprog)) < 0){
			printf ("error writing image (%d)\n", ret);
               		return (ret);
		}

		if ((ret=GRTD_rtd (dref, amplifiers, (int)nelems/pieces, -1., -1.)) < 0)
               		return (ret);
		if (i < pieces-1)
		getchar ();
	}
	GRTD_close (dref);
	//
	/*NOW, update keywords*/
	for (i=0; i<namp; i++) {
		GF_fits_movabs_hdu (file_d, i+2, 0);
		sprintf (detsec, "[%d:%ld,%d:%ld]", amp[i].xstart, amp[i].xstart + amp[i].ncols -1, amp[i].ystart, amp[i].ystart + amp[i].nrows - 1);
		labfits_write_geom_multiple (file_d, "[1:1024,1:1024]", "", "[1:1024,1:1024]", "[1:1024,1:1024]", "[1:1024,1:1024]", detsec, "[1:1024,1:1024]", "");
	}
	GF_fits_movabs_hdu (file_d, 1, 0);
	GF_fits_close_file (file_d);
	printf ("closing file\n");

	GU_close ();
	GF_close ();
clean:
	if (map_addr > 0)
		free ((void *)map_addr);
#if 0
	if (unsc_addr > 0)
		free ((void *)unsc_addr);
#endif
	printf ("OK, ciao\n");
        return (ret);
}
