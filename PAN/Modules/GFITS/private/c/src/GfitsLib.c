#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#ifdef OS32
#include "fitsio.h"
#else
#include "fitsio_64.h"
#endif
#include "GfitsLib.h"
#include "../../../../GUNSC/public/c/include/GUnscrambling.h"

#ifdef _SHMEM_
#include "GF_shmem.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#ifdef TIMEDEB
  #include <time.h>
#endif

#ifdef _DEBUG_
#  define pdebug(fmt, args...) printf ("GFITS: " fmt, ## args)
#else
#  define pdebug(fmt, args...)
#endif

#define MAX_HEADER	24
#define OK		0
#define BADREF		0
#define DERTOIZ		1
#define IZTODER		-1
#define DIVIDES(a,n) (!((n) % (a)))

#ifdef TIMEDEB
time_t sum;
#endif
static i32 totEleRead[MAX_AMPS];	/**<total elements read per amplifier*/
static i32 Extension[MAX_AMPS];		/**<extensions array*/
static i32 datatype=0;
static fint hdutype;
static i32 nzim=1;
static i32 nzcnt=0;
static i32 bytes_pp=0;
static amp_t fitsamp[MAX_AMPS];  /**<internal fits amplifier structure. Filled externally, at Init*/
static u32 totElePerAmp;	/**<total number of elements per amplifier*/
static u32 totEle;		/**<total number of elements in the image*/
static u32 totcols=0;		/**<total number of columns*/
static u32 totrows=0;		/**<total number of rows*/
static i32 is_roi=0;		/**<is roi defined?*/
static dptr_t roiptr=0;		/**<is roi defined?*/
static u32 roisize=0;		/**<total buffer for roo*/
static u32 fitsbufsize=0;
static u32 xroist=0;		/**<ini x start in roi, if requested*/
static u32 yroist=0;		/**<in y start in roi, if requested*/
static u32 xroilen=0;		/**<total number of columns in roi, if requested*/
static u32 yroilen=0;		/**<total number of rows in roi, if requested*/
static u32 totPixels;		/**<total numbe rof pixels in the image*/
static i32 namps=0;		/**<total number of amplifiers in the image*/
static i16 mext;		/**<multiple or single extensions*/
static i16 memfits=0;
i32 shmid=-1;
i32 shmqid=-1;
i32 shmem=0;
i32 is_shared=0;
void *membuf=NULL;		/**<memory buffer pointer*/
size_t membufsize=0;
size_t memfitssize=0;
size_t imsize=0;
size_t hdrsize=0;
void *nextbuf;
i32 bufcnt=0;
i32 nbufs=1;
i32 minimy=1E6;
i32 minimx=1E6;
i32 ominx=-1;
i32 ominy=-1;
i32 omaxx=-1;
i32 omaxy=-1;
static dptr_t tmpptr[MAX_AMPS];		/**<temnpoeral pointers for data manipulation*/
static i16 amptype[MAX_AMPS];		/**<amplifier type , right to left or left to right*/
char error_msg[100];
//static i32 xstart[MAX_AMPS];
//static i32 ystart[MAX_AMPS];
//
void GF_check_roi();
/******************************************************/
/*
*
* 	functions from CFITSIO, v2.0
*  	adapted for being used easily with Labview
*	The following functios are just wrappers to the 
*	CFITSIO ones
*
******************************************************/

void GF_set_mbufs (i32 mbufs)
{
	if (mbufs < 0)
		mbufs = 1;

	nbufs = mbufs;
}

/**
* Sets the fits data size
* @param[in] data (ushort, short, float or long)
*/
void GF_set_bpp (i32 data)
{
       if ((data == TUSHORT) ||(data == TSHORT))
                bytes_pp = 2;
        else
                bytes_pp = 4;

	GF_check_roi();
}


void GF_printmsg (char *msg)
{
	printf ("** %s **\n", msg);
}

/**
* Sets memory fits file
*/
void GF_set_memfits (short flag)
{
	if (flag > 0)
		memfits = 1;
	else
		memfits = 0;
	printf ("Gfits: memfits is now %d\n", memfits);
}

/**
* is the fits file going to be written in memory?
*/
short GF_get_memfits (void)
{
	return (memfits);
}

/**
* Releases the memory block where the memory fits is allocated
*/
i32 GF_release_image_buf (i32 *err, i32 size, i32 snd)
{
        if ((membuf == NULL) || (membufsize == 0))
                return (OK);
#ifdef _SHMEM_
char msg[MAXLEN];
             if (shmid >= 0) {
                  if (snd){
                        sprintf (msg, "SHMEM NEWSIZE %d", size);
                        GF_shmSendMsg (shmqid, msg, MSGNEWFITS);
                  }
                  if ((*err = GF_shmDestroy (SHM_NAME, membuf, shmid, shmqid)) < 0) {
                           return (*err);
                  }
            }
#else
        free (membuf);
#endif
        membuf = NULL;
        membufsize = 0;
	bufcnt = 0;
	nextbuf = NULL;
	imsize = 0;
        return (OK);
}

/**
* Allocates a memory buffer for the fits image. If using shared memory the segment is shared
*/
void *GF_allocate_image_buf (size_t *size, i32 *id, i32 *qid)
{
void *add=NULL;

#ifdef _SHMEM_
        printf ("GF_allocate_image_buf: allocating shared memory segment\n");
        add = GF_shmAttach (size, SHM_NAME, NULL, id, qid);
#else
        printf ("GF_allocate_image_buf: allocating local memory segment\n");
        *id = -1;
        add = malloc (*size);
#endif
    return (add);
}


short GF_get_shmem (void)
{
#ifdef _SHMEM_
	shmem = 1;
#else
	shmem = 0;
#endif
        return (shmem);
}

void GF_set_shmem (short flag)
{
#if 0
        if (flag > 0) {
		if (!GF_get_shmem()) {
			if ((membufsize > 0) && (membuf != NULL)) { /*there is already a mem segment allocated*/
				if (!is_shared) {  /*but it is not shared*/
					GF_release_image_buf (NULL, 0, 0);
				}
			}
		}
		shmem = 1;
		GF_set_memfits (1);
        } else {
                shmem = 0;
		GF_set_memfits (0);
	}
	printf ("Gfits: shmem is now %d\n", shmem);
#endif
	GF_set_memfits (flag);
}


ptr_t GF_get_membufptr (i32 *fitsize)
{
	*fitsize = memfitssize;
	return ((ptr_t)(dptr_t) membuf);
}


/***********************************************************************************//**
*Sets the data compression for the fits image
*@param[in] name the name of the fits image, containing the compression directives
*@param[in] fitsfile the fits file descriptor
************************************************************************************/
i32 GF_compress_img (fitsfile *fptr, char *name)
{
char *i=NULL;
i32 stat=0;
/**parses the compressionn instructions in the name. This is used only for compressing
memory fits files; otherwise, the cfistio functions take care of this by themselves*/
//	i = strstr (name, "[compress");
	i = strstr (name, "[compress");
	if (i==NULL)		/*|<no compression*/
		return (OK);

	char tmpstr[20];
	i32 compress=NOCOMPRESS;
	i32 smooth = 0;
	strcpy (tmpstr, "NOCOMPRESS");

	i = strtok (name, " ");
	i = strtok (NULL, " ");	/*|<after "compress"*/
	if ((i==NULL) || (strncmp (i, "R",1) == 0)){
		compress = RICE_1;
		strcpy (tmpstr, "RICE_1");
	} else if (strncmp (i, "G",1)==0) {	
			compress = RICE_1;
			strcpy (tmpstr, "GZIP_1");
	} else if (strncmp (i, "H", 1) == 0) {
			compress = HCOMPRESS_1;
			strcpy (tmpstr, "HCOMPRESS_1");
			if (strncmp (i, "HS", 2) == 0) 
				smooth = 1;
			else
				smooth = 0;
        } else if (strncmp (i, "P", 1) == 0) {
                        compress = PLIO_1;
                        strcpy (tmpstr, "PLIO_1");
	}

	printf ("compressing to %s\n", tmpstr);
        fits_set_compression_type(fptr, compress, &stat);
        if (stat != 0){
              printf ("error %d setting compression\n", -stat);
              return (-stat);
        }

	const char delim[] = " ,;]"; 
	i = strtok (NULL, delim);
	if (i == NULL){		/*that was all*/
		return (OK);
	} 
/*tiles sizes*/

	 flong tilesize[2];
	 i32 val=0;
	 tilesize[0] = atoi (i);
	 if (tilesize[0] > 0) {
	 	i = strtok (NULL, delim);
		if (i == NULL)	/*this was an error -only 1 tile size*/
			return (OK);
	 	tilesize[1] = atoi (i);
	 	if (tilesize[0] <= 0)	/*this was en error -non numeric arg as second tile size*/
			return (OK);
	}
	 if ((tilesize[0] > 0) && (tilesize[1] > 0)){
		printf ("setting tiles to %ld,%ld\n",  tilesize[0], tilesize[1]);
		fits_set_tile_dim(fptr, 2, tilesize, &stat);
		if (stat != 0){
       		       printf ("error %d setting comp. tiles\n", -stat);
             	       return (-stat);
		}
		i = strtok (NULL, delim);
		if (i == NULL)		/*that was all*/
			return (OK);
	}
	 if (strncmp (i, "q", 1) == 0) {
	 	i = strtok (NULL, " ");
		if (i == NULL)
			return (OK);
		val = atof (i);
		printf ("quant %d\n", val);
		if (val > 0) {
			printf ("setting quantiz to %d\n",  val);
			fits_set_quantize_level(fptr, val, &stat);
		        if (stat != 0){
       			        printf ("error %d setting comp. quantization\n", -stat);
             			return (-stat);
			}
			printf ("setting smooth to %d\n",  smooth);
			fits_set_hcomp_smooth (fptr, smooth, &stat);
		        if (stat != 0){
       			        printf ("error %d setting comp. smooth\n", -stat);
             			return (-stat);
			}
	 		i = strtok (NULL, delim);
			if (i == NULL)
				return (OK);
        	}
	}
         if (strncmp (i, "s", 1) == 0) {
                i = strtok (NULL, " ");
		if (i == NULL)
			return (OK);
                val = atof (i);
		printf ("scale %d\n", val);
                if (val > 0) {
			printf ("setting scale to %d\n",  val);
                        fits_set_hcomp_scale(fptr, val, &stat);
                        if (stat != 0){
                                printf ("error %d setting comp. scale\n", -stat);
                                return (-stat);
                        }
                }
        }
	 
	return (0);
}

ptr_t GF_open_file (char *filename, i32 *err)
{
i32 status=0;
fitsfile *fptr;

	fits_open_file (&fptr, filename, READWRITE, &status);
	*err = status;
	if (status != 0)
		printf ("error %d opening image %s\n", status, filename);
	return ((ptr_t)(dptr_t) fptr);
}

	
/*************************************************************************//**
 * Creates fits file
 * called by GF_create_img()
 * @params[in] name fits file name (in disk)
 * @param[out] err status out
 ***************************************************************************/
dptr_t GF_create_file (char *name, i32 *err)
{
fitsfile *fptr;
i32 stat=0;
size_t bsize;

#ifdef TIMEDEB
	sum =0;
#endif
	if (memfits){
		imsize = totEle*bytes_pp + namps*5760;  /*last is for headers*/
		bsize = (size_t) nbufs*imsize;
		if (membufsize < bsize) {	
			if (membuf != NULL) {
				if (GF_release_image_buf (err, bsize, 1) < 0)
					return (0);
			}
			membuf = GF_allocate_image_buf (&bsize, &shmid, &shmqid);
			pdebug ("GFITS: allocating buffers of %ld bytes at 0x%lx (%d images)\n", (long)bsize, (dptr_t) membuf, nbufs);
			if (membuf == NULL) {
				membufsize = 0;
#ifdef _SHMEM_
				*err = shmid;
				return (shmid);
#else
				*err = -ENOMEM;
				return (-ENOMEM);
#endif
			} else {
				membufsize = bsize;
			}
			nextbuf = membuf;
			bufcnt = 0;
			pdebug ("GFITS: allocation succeed\n");
		}
		pdebug ("GFITS: memfits buffer at 0x%lx (%d)\n", (dptr_t) nextbuf, bufcnt);
		bzero (nextbuf, imsize);
		fits_create_memfile (&fptr, &nextbuf, &imsize, 0, NULL, &stat);
		GF_compress_img (fptr, name); 
	} else {
		fits_create_file (&fptr, name, &stat);
	}

	if (stat != 0) {
		pdebug ("returned err %d while trying to create file %s\n", stat, name);
		*err = stat;
		return (0);
	} else {
		pdebug ("OK creating file %s (ref %ld)\n", name, (dptr_t) fptr);
		*err = 0;
	}	
		
	return ((dptr_t) fptr);
}

/**
* Gets the error (status) in ascii form (returns the an ascii string)
* @params[in] status the status code
*/
char* GF_get_error_text (i32 status)
{
	if (status < 0)
		status *= -1;
	if ((status < 100) || (status == 0))
		return ("");

	fits_get_errstatus (status, error_msg);
	return (error_msg);
}

/**
* Sets the value of the Z coordinate (depth) in a datacube (third axis)
* @param[in] z amount of frames in cube
*/
i32 GF_fits_set_zaxis (i32 nz)
{
	if (nz <= 0) 
		return (-EINVAL);

	nzim = nz;
	return (OK);

}


i32 GF_set_roi (u32 xstart, u32 ystart, u32 xlen, u32 ylen)
{
	is_roi = xroist = yroist = xroilen = yroilen = 0;
	if (mext) 
		return (OK);

	if ((xstart > 0) && (ystart > 0) && (xlen > 0) && (ylen > 0)){
		xroist = xstart;
		yroist = ystart;
		xroilen = xlen;
		yroilen = ylen;
		if (roiptr != 0){
			free ((void *)roiptr);
			roiptr = 0;
			roisize = 0;
		}
		if ((roiptr = (dptr_t) malloc (xlen*ylen*bytes_pp)) == 0)
			return (-ENOMEM);
		roisize = xlen*ylen*bytes_pp;
		is_roi = 1;
	} 
	printf ("is_roi %d: %u %u %u %u (bpp %d)\n", is_roi, xroist, yroist, xroilen, yroilen, bytes_pp);

	return (OK);
}

i32 GF_get_roi (u32 *xstart, u32 *ystart, u32 *xlen, u32 *ylen)
{
	*xstart = xroist;	
	*ystart = yroist;	
	*xlen = xroilen;
	*ylen = yroilen;
	return (is_roi);
}

void GF_check_roi()
{
u32 rsize;

	if (is_roi){
		rsize = xroilen*yroilen*bytes_pp;
		if (roisize < rsize)
			GF_set_roi (xroist, yroist, xroilen, yroilen);
	}
}
		


/**
* Sets basic setup values for the fits image to be created. This is called prior to writing the data
* Fills the internal fits amplifiers structure coming from outside (unscrambler/geometry modules)
* called by GF_create_img()
* @param[in] data image datatype (short, ushort, etc)
* @param[in] hdu hdutype
* @param[in] mult multiple (>0) or single (==0) extensions fits image
* @param[in] amplifiers pointer to the internal amplifier structure (containing the geometry)
**/
i32 GF_fits_set_values (i32 data, i32 hdu, i32 mult, ptr_t amplifiers)
{
i32 i;
i32 oldbytes_pp;
i32 oldmext;
amp_t *amp;
i32 maxx, maxy, etot, etot2;

	maxx = maxy = etot2 = -1;
	minimy = minimx = 1E6;
	oldbytes_pp = bytes_pp;
	oldmext = mext;
	datatype = data;
	mext = mult;
	hdutype = hdu;
	amp = (amp_t *)(dptr_t)amplifiers;
	if (amp == NULL)
		return (-EFAULT);

	GF_set_bpp (datatype);
//	pdebug ("set_values: bytes_pp %d oldbytes_pp %d datatype %d\n", bytes_pp, oldbytes_pp, datatype);
	for (i=0; i<namps; i++) {
		if (mext){
			fitsamp[i].xstart = 1;
			fitsamp[i].ystart = 1;
		} else {
			fitsamp[i].xstart = (amp + i)->xstart;
			fitsamp[i].ystart = (amp + i)->ystart;
		}
		fitsamp[i].ncols = (amp + i)->ncols;
		fitsamp[i].nrows = (amp + i)->nrows;
		fitsamp[i].type = (amp + i)->type;
		fitsamp[i].rotate = (amp + i)->rotate;
		fitsamp[i].flip = (amp + i)->flip;
		fitsamp[i].discard = (amp + i)->discard;
		if (fitsamp[i].rotate){
			if (fitsamp[i].type & LOWER) {
				if (fitsamp[i].rotate == 90) {
					fitsamp[i].xstart +=  fitsamp[i].nrows;
					amptype[i] = IZTODER;
				} else {
					amptype[i] = DERTOIZ;
				}
			} else {
				if (fitsamp[i].rotate == 90) {
					 amptype[i] = DERTOIZ;
				} else {
					 fitsamp[i].xstart +=  fitsamp[i].nrows;
					amptype[i] = IZTODER;
				}
			}
		}

		if (!mext) {
			if (!fitsamp[i].discard){
				if ((fitsamp[i].xstart + fitsamp[i].ncols - 1) > maxx)
					maxx = fitsamp[i].xstart + fitsamp[i].ncols - 1;
				if ((fitsamp[i].ystart + fitsamp[i].nrows - 1) > maxy)
					maxy = fitsamp[i].ystart + fitsamp[i].nrows - 1;
				if (fitsamp[i].xstart < minimx)
					minimx = fitsamp[i].xstart;	
				if (fitsamp[i].ystart < minimy)
					minimy = fitsamp[i].ystart;	
				if ((fitsamp[i].type & UPPER) && !fitsamp[i].rotate) 
					fitsamp[i].ystart += fitsamp[i].nrows;
				etot2 = (maxx-minimx+1)*(maxy-minimy+1);
			}
		}
		pdebug ("fitsamp[%d]: xstart %d ystart %d ncols %d nrows %d type %d discard %d maxx %d maxy %d minx %d miny %d\n", i, fitsamp[i].xstart, fitsamp[i].ystart, fitsamp[i].ncols, fitsamp[i].nrows, fitsamp[i].type, fitsamp[i].discard, maxx, maxy, minimx, minimy);
		if ((bytes_pp > oldbytes_pp) || (mext != oldmext)) {
			if (mext) 
				if (fitsamp[i].rotate) {
					if ((void *)tmpptr[i] == NULL){
						free ((void *)  tmpptr[i]);
						tmpptr[i] = 0;
					} 
					if ((tmpptr[i] = (dptr_t) malloc (fitsamp[i].ncols*fitsamp[i].nrows*bytes_pp)) == 0) 
						return (-ENOMEM);
//					pdebug ("set_values: allocationg buffer %ld for amp %d\n", tmpptr[i], i);
				}
		}
		totEleRead[i] = 0;
	}
	if ((bytes_pp > oldbytes_pp)||(mext!=oldmext) || (totEle != etot2) || (omaxx != maxx) || (omaxy != maxy) || (ominy != minimy) || (ominx != minimx))
		if (!mext) {
			if ((void *)tmpptr[0] != NULL){
				free ((void *) tmpptr[0]);
				tmpptr[0] = 0;
			}
			etot = totEle;
			totcols = maxx-minimx+1;
			totrows = maxy-minimy+1;
			totEle = etot2;
			ominy=minimy;
			ominx=minimx;
			omaxx=maxx;
			omaxy=maxy;
			if ((tmpptr[0] = (dptr_t) malloc ((int)(1.1*totEle*bytes_pp))) == 0)
//			if ((tmpptr[0] = (dptr_t) malloc ((totEle)*bytes_pp)) == 0)
				return (-ENOMEM);
			else {
				fitsbufsize = (int)(1.1*totEle*bytes_pp);
				bzero ((void *)tmpptr[0], totEle*bytes_pp);
				pdebug ("set_values: allocated ptr %ld %d bytes bytes_pp %d)\n", tmpptr[0], totEle*bytes_pp, bytes_pp);
				pdebug ("set_values: orig totEle %d end totEle %d maxx %d maxy %d minx %d miny %d)\n",etot, totEle, maxx, maxy, minimx, minimy);
			}
		}	
	return (OK);
}

/*************************************************************************//**
 * Creates an image in the file (acording to the given parameters)
 * called by the user program
 * @param[in] name image name
 * @param[in] data image datatype (ushort,short, etc)
 * @param[in] hdu hdu type
 * @param[in] mult multiple (>0) or single (==0) extensions file
 * @param[in] amplifiers pointer to the internal amplifiers structure
 * @param[in] nkeys optional parameter to preallocate space for the keywords. Ignored if <=0
 * @param[out] *err status code out
 ***************************************************************************/
ptr_t GF_create_img (char *name, i32 data, i32 hdu, i32 mult, ptr_t amplifiers, i32 nkeys, i32 *err)
{
fitsfile *fptr;
i32 stat=0, i;
i32 bitpix;
flong sizes[3];
i32 ret;
dptr_t ref;
i32 na=2;
#ifdef TIMEDEB
struct timeval tval;
time_t beg, end;

        gettimeofday (&tval, NULL);
        beg = 1000*tval.tv_sec + tval.tv_usec/1000;
#endif
	if ((totcols == 0 || totrows == 0 || datatype == 0)) {
		pdebug ("GF_create_img: Call GF_init first!!\n");
		*err = -EINVAL;
		return (BADREF);
	}

        if (memfits)
                GF_set_bpp (data);

	ref = GF_create_file (name, &stat);
        if ((ref == 0) || (stat != 0)) { 
		*err= -stat;
		pdebug ("GF_create_img: ERROR %d, returning 0 ref %ld\n", *err, ref);
                return (BADREF);
	}

	if ((ret=GF_fits_set_values (data, hdu, mult, amplifiers)) < 0){
		*err = -stat;
		return (BADREF);
	}
	pdebug ("GF_create_img: after set vals (ref %ld, amplifiers %ld)\n", ref, (ulong)amplifiers);

	fptr = (fitsfile *) ref;  
	nzcnt = 0;

	if (nkeys < 0)
		nkeys = 0;

	if (datatype == TLONG)
		bitpix = LONG_IMG;
	else if (datatype == TFLOAT)
		bitpix = FLOAT_IMG;
	else
		bitpix = USHORT_IMG;
	if (!mext) {
		sizes[0] = totcols;
		sizes[1] = totrows;
		if (is_roi){
			is_roi = 0;
			if ((xroist + xroilen -1) <= totcols){
				sizes[0] = xroilen;
				is_roi = 1;
			} 
				is_roi = 0; 
			if ((yroist + yroilen -1) <= totrows){
				sizes[1] = yroilen;
				is_roi = 1;
			} else
				is_roi = 0;
		}

		if (nzim > 1) {
			na = 3;
			sizes[2] = nzim;
		} else {
			na = 2;
			sizes[2] = 1;
		}
       		fits_create_img (fptr, bitpix, na, sizes, &stat);
		pdebug ("GF_create_img: adding keys %d\n", nkeys);
		fits_set_hdrsize (fptr, nkeys, &stat);
		if (stat != 0){
			*err= -stat;
			return (BADREF);
		}
	} else {
		flong ampsize[2];
		/*now create the right images according to the desired extension order*/
		ampsize[0] = 0;
		ampsize[1] = 0;
		if (stat != 0){
			*err= -stat;
			return (BADREF);
		}
		pdebug ("GF_create_img: about to create image %d\n", *err);
		fits_create_img (fptr, bitpix, 0, ampsize, &stat);
		pdebug ("GF_create_img: adding keys %d\n", nkeys);
		fits_set_hdrsize (fptr, nkeys, &stat);
		if (stat != 0){
			*err= -stat;
			return (BADREF);
		}
		for (i=0; i<namps; i++) {
		  if (fitsamp[i].discard <= 0){
			if (fitsamp[i].rotate == 0) {
				ampsize[0] = fitsamp[Extension[i]].ncols;
				ampsize[1] = fitsamp[Extension[i]].nrows;
			} else {
				ampsize[0] = fitsamp[Extension[i]].nrows;
				ampsize[1] = fitsamp[Extension[i]].ncols;
			}
			pdebug ("moving to hdu %d (type %d)\n", Extension[i], hdutype);
			fits_create_hdu (fptr, &stat);
			 if (stat != 0){
				 *err = -stat;
				 return (BADREF);
			}
			pdebug ("inserting image %d bitpix %d\n", Extension[i], bitpix);
        		fits_insert_img (fptr, bitpix, 2, ampsize, &stat);
			pdebug ("GF_create_img: adding keys %d (amp %d)\n", nkeys, i);
			fits_set_hdrsize (fptr, nkeys, &stat);
			if (stat != 0){
				*err= -stat;
				return (BADREF);
			}
		  } else
			pdebug ("found amplifier to be skipped\n");
		}
//        	fits_movabs_hdu (fptr, 1, (int *)hdutype, &stat);
        	fits_movabs_hdu (fptr, 1, &hdutype, &stat);
		if (stat != 0){
			*err= -stat;
			return (BADREF);
		}
	}
	pdebug ("GF_create_img: returning %ld\n", ref);
#ifdef TIMEDEB
        gettimeofday (&tval, NULL);
        end = 1000*tval.tv_sec + tval.tv_usec/1000;
	printf ("Creating image took %ld\n", end-beg);
#endif	
        return ((ptr_t)ref);
}



/*************************************************************************//**
 * Closes fits file
 ***************************************************************************/
i32 GF_fits_close_file (ptr_t ref, i32 err)
{
fitsfile *fptr;
fint stat=0;

        if (ref == 0)
                return (-EBADF);

	fptr = (fitsfile *) (dptr_t) ref; 
	pdebug ("GF: closing file (ref %ld)\n", (ulong) ref);
	usleep (1000);
	fits_flush_file (fptr, &stat);	
	stat = 0;
	if (memfits){
		fint hdunum;
		flong headstart, datastart, dataend;
		stat = 0;
	        fits_get_num_hdus(fptr, &hdunum, &stat);
		printf ("mem status %d\n", stat);
		stat = 0;
		fits_movabs_hdu(fptr, hdunum, 0, &stat);
		printf ("mem status %d\n", stat);
		stat = 0;
		fits_get_hduaddr(fptr, &headstart, &datastart, &dataend, &stat);
		printf ("GF: memfits hdunum %d, dataend %ld, status %d\n", hdunum, dataend, stat);
		memfitssize = dataend;
		hdrsize = memfitssize - totEle*bytes_pp;
		stat = 0;
#ifdef _SHMEM_
	char msg[MAXLEN];
		pdebug ("closed memfits nextbud 0x%lx (%d), fitssize %d, hdrsize %d\n", (dptr_t) nextbuf, bufcnt, memfitssize, hdrsize);
               if (err)
                        sprintf (msg, "NEWFITS 0\n");
                else
			sprintf (msg, "NEWFITS %d %d\n", imsize, bufcnt);
		GF_shmSendMsg (shmqid, msg, MSGNEWFITS);
#endif
                bufcnt++;
                if (bufcnt >= nbufs)
                        bufcnt=0;
               nextbuf = membuf + (imsize*bufcnt);
	}
        fits_close_file (fptr, &stat);
	pdebug ("GF: closing file with %d status (ref %ld)\n", -stat, (ulong) ref);
        return (-stat);
}

/**
* Writes image in mmeory to the file name specified
* @param[in] filename name of the output file
**/
i32 GF_fits_write_from_mem (char *filename)
{
int i;
fint hdunum;
fitsfile *infptr;
fitsfile *outfptr;
i32 status=0;

	if (memfits) {
		if ((membuf == NULL) || (membufsize == 0)){
			printf ("GFITS: ERROR no memory allocated\n");
			return (-EFAULT);
		}
		pdebug ("GFITS: writing from memory fits ...(%s)\n", filename);
		printf ("GFITS: writing from memory %ld\n", (ulong)(ptr_t)(dptr_t) membuf);
		fits_open_memfile (&infptr, "any_text_will_do", READONLY, &membuf, &membufsize, 0, NULL, &status); 
		if (status != 0){
			printf ("GFITS: ERROR -%d opening memfile\n", status);
			return (-status);
		}
		fits_create_file(&outfptr, filename, &status);
		if (status != 0){
			printf ("GFITS: ERROR -%d creating file\n", status);
			return (-status);
		}
		fits_get_num_hdus(infptr, &hdunum, &status);
		for (i=1; i<=hdunum; i++){
		    	fits_movabs_hdu(infptr, i, &hdutype, &status);
		    	if (status != 0){
				printf ("GFITS: ERROR %d moving hdu %d\n", status, i);
      				return(-status);
			}
    		    	fits_copy_hdu(infptr, outfptr, 0, &status);
    		    	if (status != 0){
				printf ("GFITS: ERROR %d copying hdu %d\n", status, i);
      				return (-status);
			}
		}
		fits_close_file(infptr, &status);
		if (status != 0){
			printf ("GFITS: ERROR %d closing memfile\n", status);
    			return (-status);
		}
		fits_close_file(outfptr, &status);
		if (status != 0){
			printf ("GFITS: ERROR %d closing file\n", status);
    			return (-status);
		}
	}
	pdebug ("GFITS: writting from memory fits ...DONE!\n");
	return (OK);
}

/*************************************************************************//**
 * Writes the creation date into the headers 
 *  @param[in] ref fits reference
 ***************************************************************************/
i32 labfits_write_date (ptr_t ref)
{
fitsfile *fptr;
i32 stat=0;

        if (ref == 0)
                return (-EBADF);

	fptr = (fitsfile *) (dptr_t) ref;
	fits_write_date (fptr, &stat);
	return (-stat);
}



/*************************************************************************//**
 * Writes a comment into the headers
 *  @param[in] ref fits reference
 *  @paraam[in] comment string to write
 ***************************************************************************/
i32 labfits_write_comment (ptr_t ref, char *comment)
{
fitsfile *fptr;
i32 stat=0;

        if (ref == 0)
                return (-EBADF);

        fptr = (fitsfile *) (dptr_t) ref;
        fits_write_comment (fptr, comment, &stat);
        return (-stat);
}



/*************************************************************************//**
 * Add into the headers all the geometry keywords that correspons to a
 * single-extension fits file. DEPRECATED.
 ***************************************************************************/
i32 labfits_write_geom_single (ptr_t ref, char *Asec, char *Bsec, char *Csec, char *Dsec, char *Tsec, char *amplifier)
{
fitsfile *fptr;     
i32 stat1=0, stat2=0, stat3=0, stat4=0, stat5=0;
char keyname[10];

	if (ref == 0)
		return (-EBADF);

	fptr = (fitsfile *)(dptr_t) ref;

	sprintf (keyname, "ASEC%s", amplifier);
	fits_write_key (fptr, 16, keyname, Asec, "amplifier section", &stat1); 
	sprintf (keyname, "BSEC%s", amplifier);
        fits_write_key (fptr, 16, keyname, Bsec, "bias section", &stat2);
        sprintf (keyname, "CSEC%s", amplifier);
        fits_write_key (fptr, 16, keyname, Csec, "ccd section", &stat3);
        sprintf (keyname, "DSEC%s", amplifier);
        fits_write_key (fptr, 16, keyname, Dsec, "data section", &stat4);
        sprintf (keyname, "TSEC%s", amplifier);
        fits_write_key (fptr, 16, keyname, Tsec, "trim section", &stat5);

	return (-(stat1 + stat2 + stat3 + stat4 + stat5));
}


/*************************************************************************//**
 * Add into the headers all the geometry keywords that correspons to a
 * multiple-extension fits file (per-amp basis). DEPRECATED
 ***************************************************************************/
i32 labfits_write_geom_multiple (ptr_t ref, char *data, char *bias, char *ccd, char *amp, char *trim, char *det, char *ccdsize, char *amplifier)
{
fitsfile *fptr;
i32 stat1=0, stat2=0, stat3=0, stat4=0, stat5=0, stat6=0, stat7=0;

        if (ref == 0)
                return (-EBADF);

        fptr = (fitsfile *)(dptr_t) ref;

        fits_write_key (fptr, 16, "DATASEC", data, "data section", &stat1);
	fits_write_key (fptr, 16, "BIASSEC", bias, "overscan section", &stat2);
        fits_write_key (fptr, 16, "CCDSEC", ccd, "ccd section", &stat3);
        fits_write_key (fptr, 16, "AMPSEC", amp, "amplifier section", &stat4);
        fits_write_key (fptr, 16, "TRIMSEC", trim, "trim section", &stat5);
	fits_write_key (fptr, 16, "DETSEC", det, "detector section", &stat6);  
	fits_write_key (fptr, 16, "CCDSIZE", ccdsize, "ccd size", &stat7);

        return (-(stat1 + stat2 + stat3 + stat4 + stat5 + stat6 + stat7));
}  



/*************************************************************************//**
 * Adds a keyword into the headers
 * @param[in] ref reference to fits file
 * @param[in] datatype type of keyword (cfitsio types)
 * @param[in] keyname name of the keyword
 * @param[in] value pointer to value
 * @param[in] comment string comment to write to the keyword
 ***************************************************************************/
i32 labfits_write_key (ptr_t ref, i32 datatype, char *keyname, void *value, char *comment)
{
fitsfile *fptr;
i32 stat=0;

        if (ref == 0)
                return (-EBADF);

        fptr = (fitsfile *) (dptr_t) ref;
        fits_write_key (fptr, datatype, keyname, value, comment, &stat);
        return (-stat);
}


/*************************************************************************//**
 * Adds a keyword into the headers, taking the value as a string, and converting it internally
 * @param[in] ref reference to fits file
 * @param[in] datatype type of keyword (cfitsio types)
 * @param[in] keyname name of the keyword
 * @param[in] value string value. For example. "5"
 * @param[in] comment string comment to write to the keyword
 ***************************************************************************/
i32 labfits_write_key_from_char (ptr_t ref, i32 datatype, char *keyname, char *value, char *comment)
{
fitsfile *fptr;
i32 stat=0;
flong l_val;
fint i_val;
fulong ul_val;
fuint ui_val;
f32 f_val;
pan_double_t d_val;
i16 s_val;
u16 us_val;
void *val;
#ifdef TIMEDEB
fint hdunum;
struct timeval tval;
time_t beg, end;
#endif

        if (ref == 0)
                return (-EBADF);

        fptr = (fitsfile *) (dptr_t) ref;

	if (strlen(keyname) <= 0)
		return (0); 


	switch (datatype) {
		case TSTRING: {val = (void *)value; break;}
		case TLONG: {l_val = atol (value); val = (void *)&l_val; break;}
		case TINT: {i_val = atoi (value); val = (void *)&i_val; break;}
		case TULONG: {ul_val = strtoul(value, NULL, 10); val = (void *)&ul_val; break;}
		case TUINT: {ui_val = strtoul(value, NULL, 10); val = (void *)&ui_val; break;}
		case TFLOAT: {f_val = atof (value); val = (void *)&f_val; break;}
		case TCOMPLEX: {d_val = strtod (value, NULL); val = (void *)&d_val; break;}
		case TDOUBLE: {d_val = strtod (value, NULL); val = (void *)&d_val; break;}
		case TSHORT: {s_val = atoi (value); val = (void *)&s_val;break;}
		case TBYTE: {us_val = atoi (value); val = (void *)&s_val;break;}
		case TUSHORT: {us_val = atoi (value); val = (void *)&us_val;break;}
		case TLOGICAL: {i_val = atoi (value); val = (void *)&i_val; break;}
		default: {val = (void *)value;}
	}
#ifdef TIMEDEB
	gettimeofday (&tval, NULL);
	beg = 1000*tval.tv_sec + tval.tv_usec/1000;
#endif
        fits_write_key (fptr, datatype, keyname, val, comment, &stat);	
	if (stat != 0){
		printf ("ERROR %d writing key %s\n", stat, keyname);
		return (-stat);
	}
//	fits_flush_buffer (fptr, 0, &stat);
#ifdef TIMEDEB
	gettimeofday (&tval, NULL);
	end = 1000*tval.tv_sec + tval.tv_usec/1000;
	sum+=end-beg;
	if ((end-beg) > 0){
		printf (" %s after %ld (%ld)\n", keyname, end-beg, sum);
		fits_get_hdu_num (fptr, &hdunum);
		printf ("writekey %s (%s type %d), hdunum %d\n", keyname, value, datatype, hdunum);
	}
#endif
//	fits_flush_file (fptr, &stat);
//	fits_flush_buffer (fptr, 0, &stat);
        return (-stat);
}

/*************************************************************************//**
 * Adds an array into several keyword into the headers
 * @param[in] ref reference to fits file
 * @param[in] typearr array containing the datatype of each keyword in the array
 * @param[in] namearr array of names of each keyword in the array (same size as typearr)
 * @param[in] valarr char values of each keyword in the array (same size as namearr and typearr)
 * @param[in] commarr string array of comment for each keyword (same size as the others)
 ***************************************************************************/
i32 labfits_writw_key_from_array (ptr_t ref, dptr_t *typearr, char *namearr, char *valarr, char *commarr, i32 nkeys)
{
i32 k;
i32 stat=0;
char *key, *val, *comm;
char *np, *vp, *cp;

	np = vp = cp = NULL;
        key = strtok_r (namearr, "\n", &np);
        val = strtok_r (valarr, "\n", &vp);
        comm = strtok_r (commarr, "\n", &cp);
        for (k=0; k<nkeys; k++){
               	stat = labfits_write_key_from_char (ref, typearr[k], key, val, comm);
                key = strtok_r (NULL, "\n", &np);
                val = strtok_r (NULL, "\n", &vp);
                comm = strtok_r (NULL, "\n", &cp);
        }
        return (-stat);
}



/**********************************************************************
 *
 * HDU routines (Header Data Units). For multiple extension fits files
 *
 ***********************************************************************/


/*************************************************************************//**
 * Creates an hdu in the file
 ***************************************************************************/
i32 labfits_create_hdu (ptr_t ref)
{
fitsfile *fptr;
i32 stat=0;

        if (ref == 0)
                return (-EBADF);
 
        fptr = (fitsfile *) (dptr_t) ref;
        fits_create_hdu (fptr, &stat);
        return (-stat);
}



/*************************************************************************//**
 * Inserts a new image into a file (for a new extension)
 * @param[in] ref reference to fits file
 * @param[in] bitpix type of image (BITPIX cfitsio types)
 * @param[in] naxis number of axes in the image (2 for regular fits image, 3 for a datacube)
 * @param[in] naxes array containing the size (in elements/pixels) in each dimension
 ***************************************************************************/
i32 labfits_insert_img (ptr_t ref, i32 bitpix, i32 naxis, flong *naxes)
{
fitsfile *fptr;
i32 stat=0;

        if (ref == 0)
                return (-EBADF);
 
        fptr = (fitsfile *) (dptr_t) ref;
        fits_insert_img (fptr, bitpix, naxis, naxes, &stat);
        return (-stat);
}




/*************************************************************************//**
 * Moves the current hdu to the specified absolute one hdunum (over which the operations 
 * are performed) in an absolute way (absolute reference)
 * @param[in] ref reference to fits file
 * @param[in] hdunum number of target hdu 
 * @param[in] hdutyp type of hdu (cfitsio types)
 ***************************************************************************/
i32 GF_fits_movabs_hdu (ptr_t ref, i32 hdunum, i32 hdutyp)
{
fitsfile *fptr;
i32 *hdutype_p;
i32 stat=0;                                                                     

        if (ref == 0)
                return (-EBADF);

        fptr = (fitsfile *) (dptr_t) ref;
	hdutype_p = &hdutyp;
//	fits_flush_buffer (fptr, 0, &stat);
	fits_flush_file (fptr, &stat);
        fits_movabs_hdu (fptr, hdunum, hdutype_p, &stat);
        return (-stat);
}                                             


/*************************************************************************//**
 * Moves the current hdu to the specified relative nmoves (over which the operations
 * are performed) in a relative way (offset relative to the current hdu)
 * @param[in] ref reference to fits file
 * @param[in] nmove number of  relative movements
 * @param[in] hdutyp type of hdu (cfitsio types)
 ***************************************************************************/
i32 labfits_movrel_hdu (ptr_t ref, i32 nmove, i32 *hdutyp)
{
fitsfile *fptr;
i32 *hdutype_p;
i32 stat=0;

        if (ref == 0)
                return (-EBADF);
 
        fptr = (fitsfile *) (dptr_t) ref;
	hdutype_p = hdutyp;
        fits_movrel_hdu (fptr, nmove, hdutype_p, &stat);
        return (-stat);
}    


/**
* closes all, and releases the buffers
**/
void GF_close ()
{
i32 i;
i32 err;

	for (i=0; i<namps; i++) {
		if ((void *)tmpptr[i] != NULL){
			free ((void *)tmpptr[i]);
			tmpptr[i] = 0;
			fitsamp[i].ncols = fitsamp[i].nrows = 0;
		}
	}
	GF_release_image_buf (&err, 0, 0);
	namps = 0;
	bytes_pp = totcols = totrows = datatype = hdutype = 0;
}

/**
* Initialies the extensions structure
**/
void GF_InitExtensions (void)
{
i32 i;

	for (i=0; i<MAX_AMPS; i++)
		Extension[i] = i;
}

/**
* Gets the maximum number of amplifiers allowed
**/
i32 GF_GetMaxAmps (void)
{
	return (MAX_AMPS);
}

/**
*Gets the extension to amplifier mapping
* @param[out] *array array of extensions
* @param[out] *discard discarded extensions in the output image
* @param[in] nelems number of extensions to map
**/
i32 GF_GetExtensions (dptr_t *array, i32 *discard, i32 nelems)
{
i32 i, discarded;
i32 numext=0;

	if (nelems < MAX_AMPS)
		return (-ENOMEM);

	discarded=0;
	for (i=0; i<namps; i++){
/*
		if (fitsamp[i].discard){
			*(array+i) = -1;
			discarded++;
		} else
			*(array+i) = Extension[i]-discarded;
*/
		*(array+i) = Extension[i];
		*(discard+i) = fitsamp[i].discard;
		if (!fitsamp[i].discard)
			numext++;
	}
	return (numext);
}

/**
* Set the mapping metween extensions and amplifiers
@param[in] array array containing mapping
@param[in] elements number of extensions to map
**/
i32 GF_SetExtensions (dptr_t array, i32 elements)
{
i32 *extptr;
i32 ele;
i32 i;

	extptr = (i32 *) array;
	if (extptr == NULL)
		return (-EFAULT);

	for (i=0; i<namps; i++) {
		if (i > elements)
			ele = i;
		else {
			ele = *extptr;
			extptr++;
		}
		Extension[i] = ele;
		printf ("setting extension[%d] = %d\n", i, Extension[i]);
	}

	return (OK);
}	

/**
* Inits image. This has to be called before everything else
*@param[in] data datatype (cfitsio types)
*@param[in] hdu dhu type (cfitsio)
*@param[in] mult multiple or single (flat) extensions
*@param[in] amplifiers pointer to internal amplifiers extensions
*@param[in] number of amplifiers
*@param[in] total number of cols and rows
**/ 
i32 GF_init (i32 data, i32 hdu, i32 mult, ptr_t amplifiers, i32 namp, i32 totalcols, i32 totalrows)
{
i32 ret;
amp_t *amp;

	pdebug ("totcols %d totrows %d, namps %d\n", totalcols, totalrows, namp);
	namps = namp;
	GF_close();
	namps = namp;
	totcols = totalcols;
	totrows = totalrows;
	totEle = totcols*totrows;
	amp = (amp_t *) (dptr_t) amplifiers;
//	GF_InitExtensions();
	if ((ret=GF_fits_set_values (data, hdu, mult, amplifiers)) < 0)
		return (ret);

	pdebug ("after\n");
	totElePerAmp = amp -> ncols * amp->nrows;
	totPixels = namps*totElePerAmp;
	pdebug ("totcols %d totrows %d totEle %d totElePeramp %d totPixels %d amcols %d amprows %d\n", totcols, totrows, totEle, totElePerAmp, totPixels, amp -> ncols, amp->nrows);
	return (OK);
}


/**
* Adds a subimage into a memory buffer. This is used to generate a simple (flat) fits image from
* multiple amplifiers. Basicaly "builds" the image geometry on memory
* @param[in] genptr pointer to big (complete) image buffer
* @param[in] subimptr pointer to pice of image to be added
* @param[in] subxstart x position of the subimage to be added, in general image coordinate
* @param[in] subxstart x position of the subimage to be added, in general image coordinate
* @params[in] subcols number of cols of subimage
* @params[in] subrows number of rows of subimage
* @params[in] totalcols total number of cols of big image
**/
i32 GF_add_subimage (dptr_t genptr, dptr_t subimptr, i32 subxstart, i32 subystart, i32 subcols, i32 subrows, i32 totalcols)
{
i32 y, subyend;
void *imptr;
void *subptr;
void *currptr;

	imptr = (void *)genptr;
	subptr = (void *)subimptr;
	if ((imptr == NULL) || (subptr == NULL))
		return (-EFAULT);
	subyend = subystart + subrows - 1;
	for (y=subystart-1; y<subyend ; y++) {
		currptr = imptr + (y*totalcols + (subxstart-1))*bytes_pp;
		memcpy (currptr, subptr, subcols*bytes_pp);
		subptr += subcols*bytes_pp;
	}
	return (OK);
}


/**
* Writes a multiple extension fits image or piece of fits image to disk (or memory)
* @param[in] ref reference to fits image
* @param[in] amplifiers pointer to amplifiers structure
* @param[in] eleRead number of elements to write
* @param[out] *progress percentage of progress over the total image
* @params[out] *zcn counbter of number of frames if writen a datacube (3 axes)
* @params[out] *num_amps number of valid amplifiers (total amps - discraded amps)
**/
i32 GF_writeImage_Multiple (ptr_t ref, ptr_t amplifiers, i32 eleRead, i32 *progress, i32 *zcnt, i32 *num_amps)
{
i32 firstel;
fitsfile *fptr;
i32 stat=0;
i32 *hdutype_p;
i32 ampdata;
i32 i;
amp_t *amp;
void *buffer;
i32 chunkcols;
i32 ret;
i32 discarded;
i32 lasttotread=0;
#ifdef TIMEDEB
struct timeval tval;
time_t beg, end;
        gettimeofday (&tval, NULL);
        beg = 1000*tval.tv_sec + tval.tv_usec/1000;
#endif
 
	pdebug ("writing multiple image (datatype %d ref %ld)\n", datatype, (ulong)ref);
	printf ("writing data\r");
        if (ref == 0)
                return (-EBADF);

	if (eleRead <=0)
		return (OK);

	amp = (amp_t *)(dptr_t) amplifiers;
	if (amp == NULL)
		return (-EFAULT);
	pdebug ("GFITS: amplifiers %ld, eleread %d datatype %d\n", (ulong)(ptr_t)amplifiers, eleRead, datatype);
	if (eleRead > totPixels) {
		printf ("WARNING: multiple received more pixels (%d instead of %d) than expected. Inconsistency!. Ignoring extra pixels\n", eleRead, totPixels);
		eleRead = totPixels;
	}
        hdutype_p = &hdutype;
        fptr = (fitsfile *) (dptr_t) ref;
	ampdata = eleRead/namps;
	discarded = 0;
	for (i=0; i<namps; i++) {
	    if (fitsamp[i].discard <= 0){
		buffer = (void *)(amp + i)->dataptr;
		if (buffer == NULL)
			return (-EFAULT);
         	totEleRead[i] += ampdata;
		pdebug ("buffer %ld totEleRead[%d] = %d ampdata %d tmpptr %ld\n", (dptr_t) buffer, i, totEleRead[i], ampdata, tmpptr[i]);
		if (!fitsamp[i].rotate) {
         		if ((fitsamp[i].type == LL) || (fitsamp[i].type == LR)) {
                        	firstel = totEleRead[i] - ampdata + 1;
				pdebug ("writing amp %d, extension %d, buffer %ld\n", i+1, Extension[i]+2-discarded, (dptr_t) buffer);
				fits_movabs_hdu (fptr, Extension[i]+2-discarded, hdutype_p, &stat);
                        	fits_write_img (fptr, datatype, firstel, ampdata, buffer, &stat);
				if (stat != 0) {
					pdebug ("mult ERROR lower %d\n", stat);
					return (-stat);
				}
          		} else {
				firstel = totElePerAmp - totEleRead[i] + 1;
				fits_movabs_hdu (fptr, Extension[i]+2-discarded, hdutype_p, &stat);
				fits_write_img (fptr, datatype, firstel, ampdata, buffer, &stat);
//				if (stat != 0) {
//					pdebug ("mult ERROR upper %d\n", stat);
//					return (-stat);
//				}
	 		}
		} else {
			chunkcols = ampdata/fitsamp[i].ncols;
//			pdebug ("GF: xstart %d ystart %d xsize %d ysize %d data %ld\n", fitsamp[i].xstart,  fitsamp[i].ystart, chunkcols, fitsamp[i].ncols, (dptr_t) buffer);
			 if (amptype[i] == DERTOIZ) {		/*right to left*/	
				if ((ret=GF_add_subimage (tmpptr[i], (dptr_t)buffer, fitsamp[i].xstart, fitsamp[i].ystart, chunkcols, fitsamp[i].ncols, fitsamp[i].nrows)) <0)
					return (ret);
				fitsamp[i].xstart += chunkcols ;
			} else {				/*left to right*/
				fitsamp[i].xstart -= chunkcols ;
				if ((ret=GF_add_subimage (tmpptr[i], (dptr_t)buffer, fitsamp[i].xstart, fitsamp[i].ystart, chunkcols, fitsamp[i].ncols, fitsamp[i].nrows)) <0)
					return (ret);
			}
			if (totEleRead[i] >= totElePerAmp) {
				fits_movabs_hdu (fptr, Extension[i]+2-discarded, hdutype_p, &stat);
				fits_write_img (fptr, datatype, 1, totElePerAmp, (void *)tmpptr[i], &stat);
			}
		}
		lasttotread = totEleRead[i];
	    } else {
		discarded++;
		pdebug ("skipping amplifier ...(%d)\n", discarded);	
	    }
	}
	*progress = ((float)lasttotread/totElePerAmp) * 100;
#ifdef TIMEDEB
        gettimeofday (&tval, NULL);
        end = 1000*tval.tv_sec + tval.tv_usec/1000;
	printf ("\ndata write took %ld\n\n", end-beg);
#endif
	*zcnt=0;
	*num_amps = namps - discarded;
	pdebug ("GF mult: returning with %d stat\n", -stat);
	stat=0;
        return (-stat);
}



/**
* Writes a single extension fits image or piece of fits image to disk (or memory)
* @param[in] ref reference to fits image
* @param[in] amplifiers pointer to amplifiers structure
* @param[in] eleRead number of elements to write
* @param[out] *progress percentage of progress over the total image
* @params[out] *zcn counbter of number of frames if writen a datacube (3 axes)
* @params[out] *num_amps number of valid amplifiers (total amps - discraded amps)
**/
i32 GF_writeImage_Single (ptr_t ref, ptr_t amplifiers, i32 eleRead, i32 *progress, i32 *zcnt, i32 *num_amps)
{
fitsfile *fptr;
i32 stat=0;
i32 ampdata, chunkrows, chunkcols;
i32 i, ret;
amp_t *amp;
i32 discarded;
dptr_t iniptr, destptr;
u32 line, linebytes;

	pdebug ("writing single image, amps %ld, tmpptr[0] %ld\n", (ulong) amplifiers, tmpptr[0]);
        if (ref == 0)
                return (-EBADF);

	if (eleRead <=0)
		return (OK);

	if ((void *)tmpptr[0] == NULL)
		return (-EFAULT);

	pdebug ("after tmpptr\n");
	amp = (amp_t *)(dptr_t) amplifiers;
	if (amp == NULL)
		return (-EFAULT);
	pdebug ("after amp\n");

        fptr = (fitsfile *) (dptr_t) ref;
	if (eleRead > totPixels) {
		printf ("WARNING: single received more pixels (%d instead of %d) than expected. Inconsistency!. Ignoring extra pixels\n", eleRead, totPixels);
		eleRead = totPixels;
	}
	discarded = 0;
	ampdata = eleRead/namps;
	totEleRead[0] += eleRead;
	for (i=0; i<namps; i++) {
	    if (fitsamp[i].discard <= 0){
		fitsamp[i].dataptr = (amp + i)->dataptr;
		if (fitsamp[i].dataptr == 0)
			return (-EFAULT);
		if (!fitsamp[i].rotate) {
			chunkrows = ampdata/fitsamp[i].ncols;
			pdebug ("xstart %d ystart %d xsize %d ysize %d dataprt %ld\n", fitsamp[i].xstart,  fitsamp[i].ystart, fitsamp[i].ncols, chunkrows, fitsamp[i].dataptr);
         		if (fitsamp[i].type & LOWER) {
				if ((ret=GF_add_subimage (tmpptr[0], fitsamp[i].dataptr, fitsamp[i].xstart-minimx+1, fitsamp[i].ystart-minimy+1,fitsamp[i].ncols, chunkrows, totcols)) <0)
					return (ret);
				fitsamp[i].ystart += chunkrows;
          		} else {
				fitsamp[i].ystart -= chunkrows;
				if ((ret=GF_add_subimage (tmpptr[0], fitsamp[i].dataptr, fitsamp[i].xstart-minimx+1, fitsamp[i].ystart-minimy+1,fitsamp[i].ncols, chunkrows, totcols)) < 0)
					return (ret);
	 		}
				/*amplifier is rotated*/
		} else {
			chunkcols = ampdata/fitsamp[i].ncols;
//			pdebug ("xstart %d ystart %d xsize %d ysize %d\n", fitsamp[i].xstart,  fitsamp[i].ystart, chunkcols, fitsamp[i].nrows);
			 if (amptype[i] == DERTOIZ) {		/*right to left*/	
				if ((ret=GF_add_subimage (tmpptr[0], fitsamp[i].dataptr, fitsamp[i].xstart-minimx+1, fitsamp[i].ystart-minimy+1, chunkcols, fitsamp[i].ncols, totcols)) <0)
					return (ret);
				fitsamp[i].xstart += chunkcols;
			} else {				/*left to right*/
				fitsamp[i].xstart -= chunkcols;
				if ((ret=GF_add_subimage (tmpptr[0], fitsamp[i].dataptr, fitsamp[i].xstart-minimx+1, fitsamp[i].ystart-minimy+1, chunkcols, fitsamp[i].ncols, totcols)) <0)
					return (ret);
			}
		}
	  } else
		discarded++;	
	}
	*progress = ((float)(nzcnt*totPixels + totEleRead[0])/(nzim*totPixels)) * 100;
//	printf ("progress %d (%d %d)\n", *progress, nzcnt, totEleRead[0]);
	if (totEleRead[0] == totPixels) {
		pdebug ("wrting image from %ld (nzcnr %d progress %d ele[0] = %d bytes_pp %d is_roi %d, totPixels %d\n", tmpptr[0], nzcnt, *progress, (i32) tmpptr[0], bytes_pp, is_roi, totPixels);
		if (is_roi) {
			destptr = roiptr;
			linebytes = xroilen*bytes_pp;
			for (line=0; line<yroilen; line++){
				iniptr = tmpptr[0] + ((yroist-1+line)*totcols + xroist-1)*bytes_pp;
				memcpy ((void *)destptr, (void *)iniptr, linebytes);
				destptr = destptr + linebytes;
			}
			fits_write_img (fptr, datatype, 1, xroilen*yroilen, (void *)roiptr, &stat);
		} else {
			pdebug ("datatype %d, totEle %d, ptr %ld, bufsize %d\n", datatype, totEle, (unsigned long) tmpptr[0], fitsbufsize);
			fits_write_img (fptr, datatype, totEle*nzcnt + 1, totEle, (void *)tmpptr[0], &stat);
		}
		if (nzim >= 1) {
			nzcnt++;
			GF_fits_set_values (datatype, hdutype, mext, amplifiers);
		}
	}
	*zcnt = nzcnt;
	*num_amps = namps - discarded;
        return (-stat);
}

/**
* Writes a single or multiple extension fits image or piece of fits image to disk (or memory)
* @param[in] ref reference to fits image
* @param[in] amplifiers pointer to amplifiers structure
* @param[in] eleRead number of elements to write
* @param[out] *progress percentage of progress over the total image
* @params[out] *zcn counbter of number of frames if writen a datacube (3 axes)
* @params[out] *num_amps number of valid amplifiers (total amps - discraded amps)
**/
i32 GF_writeImage (ptr_t ref, ptr_t amplifiers, i32 eleRead, i32 *progress, i32 *zcnt, i32 *num_amps)
{
	if (mext)
		return (GF_writeImage_Multiple (ref, amplifiers, eleRead, progress, zcnt, num_amps));
	else
		return (GF_writeImage_Single (ref, amplifiers, eleRead, progress, zcnt, num_amps));
}

/**
* Writes to file an image from a memorty buffer
* @param[in] ref reference to fits image
* @param[in] amplifiers pointer to amplifiers structure
* @param[in] eleRead number of elements to write
* @param[out] *progress percentage of progress over the total image
* @params[out] *zcn counbter of number of frames if writen a datacube (3 axes)
* @params[out] *num_amps number of valid amplifiers (total amps - discraded amps)
**/
i32 GF_writeImage_from_buf (ptr_t ref, ptr_t buffer, i32 eleRead, i32 *progress, i32 *zcnt, i32 *num_amps)
{
i32 ampdata_bytes, i;

	if (buffer == 0)
		return (-EFAULT);
	ampdata_bytes = (eleRead*bytes_pp)/namps;
	for (i=0; i<namps; i++)
		fitsamp[i].dataptr = (dptr_t) buffer + i*ampdata_bytes;
	return (GF_writeImage (ref, (ptr_t)(dptr_t) fitsamp, eleRead, progress, zcnt, num_amps));
}

/**
* Writes a fits image using no internal structure, but just thedata passed here, from the provided buffer
* @param[in] name is the name of the fits file to be written (full path)
* @param[in] ncols total number of columns
* @param[in] nrows total number of rows
* @param[in] dtype image data type (cfitsio types)
* @param[in] *bptr pointer to buffer with the pixel data to be written
* @param[in] ntries number of attemptrs before giving up 
* @params[out] *err output status
**/
ptr_t GF_simple_data_write (char *name, i32 ncols, i32 nrows, i32 dtype, ptr_t bptr, i32 ntries, i32 *err) 
{
i32 stat;
i32 bitpix, errcnt;
flong sizes[2];
void *buffer;
fitsfile *fptr;
dptr_t ref;
char imname[1024];

	*err = 0;
        if ((ncols == 0 || nrows == 0 || dtype == 0)) {
                pdebug ("undefined rows, cols, or datatype\n");
		*err = -EINVAL;
                return (BADREF);
        }
	buffer = (void *) (dptr_t) bptr;
	if (buffer == NULL) {
		*err = -EFAULT;
		return (BADREF);
	}

	if (ntries <=0)
		ntries = 1;

	errcnt = 0;
	sprintf (imname, "%s.fits", name);
        pdebug ("simple data write type %d, buf %ld, before creating file\n", dtype, (dptr_t) bptr);
try_again:
	ref = GF_create_file (imname, &stat);
        if ((ref == 0) || (stat != 0)) { 
		if (errcnt >= ntries){
			*err= -stat;
               		return (BADREF);
		} else {
			sprintf (imname, "%s_%d.fits", name, errcnt);
			errcnt += 1;
			goto try_again;
		}
	}
        fptr = (fitsfile *) ref;

        if (dtype == TLONG)
                bitpix = LONG_IMG;
        else if (dtype == TFLOAT)
                bitpix = FLOAT_IMG;
        else
                bitpix = USHORT_IMG;

       sizes[0] = ncols;
       sizes[1] = nrows;
       fits_create_img (fptr, bitpix, 2, sizes, &stat);
       if (stat != 0){
	      *err = -stat;
              return (BADREF);
       }
       fits_write_img (fptr, dtype, 1, ncols*nrows, buffer, &stat);
       if (stat != 0){
	      *err = -stat;
              return (BADREF);
	}
       
       pdebug ("returning %ld\n", (ulong) (ptr_t) ref);
       return ((ptr_t) ref);
}
