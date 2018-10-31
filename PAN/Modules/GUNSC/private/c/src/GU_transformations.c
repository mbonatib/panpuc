#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "GU_transformations.h"

#define PI		3.141598
#define OK		0

//void *dptr_t (ptr_t inptr);

/**
* Receive degrees, return radians, cosine and sine
* @param[in] degrees input angle in degrees
* param[out] sine pointer to output angle sine
* param[out] cosine pointer to output angle cosine
**/
pan_double_t RI_angle_vals (i32 degrees, pan_double_t *sine, pan_double_t *cosine)
{
pan_double_t theta;

	theta = (pan_double_t) degrees * (pan_double_t) 2*PI/360;
	*sine = sin (theta);
	*cosine = cos (theta);

	if (fabs (*sine) < 0.0001)
		*sine = 0;
	if (fabs(*cosine) < 0.0001)
		*cosine = 0;
	return (theta);
}	

/**
* Routine to rotate SHORT image in the specified angle.
* @param[in] imptr image buffer pointer
* @param[in] ncols number of image columns
* @param[in] nrows number of image rows
* @para,[in] rotation desired rotation angle (90, 180 or 270 degrees only)
**/
i32 RI_SHORT (i16 *imptr, u32 ncols, u32 nrows, i32 rotation)
{
u32 x,y;
u32 bytes;
i16 *rotptr;
pan_double_t theta;
i32 i,j;
pan_double_t sine=0, cosine=0;
i32 jcorr, icorr;

	bytes = ncols*nrows*sizeof(i16);
	theta = RI_angle_vals (rotation, &sine, &cosine);
        rotptr = (i16 *) malloc (bytes);
        if ((imptr == NULL) || (rotptr == NULL))
                return (-EFAULT);
	if (rotation == 90) {jcorr =0; icorr = 1;}
	else {jcorr =1; icorr = 0;}

	pdebug ("RI_SHORT: rotating in %g radians...\n", theta);
	for (y=0; y<nrows; y++)
		for (x=0; x<ncols; x++) {
			i = (i32)((f32)(x-(f32)ncols/2)*(f32)cosine-(f32)(y-(f32)nrows/2)*(f32)sine + nrows/2 -icorr  ) ;
			j = (i32)((f32)(x-(f32)ncols/2)*(f32)sine+(f32)(y-(f32)nrows/2)*(f32)cosine + ncols/2 -jcorr ) ;
			*(rotptr + j*nrows + i) = *(imptr + y*ncols + x);
		}

      	memcpy ((void *)imptr, (void *)rotptr, bytes);
	free (rotptr);

        return (OK);
}


/**
* Routine to rotate LONG image in the specified angle.
* @param[in] imptr image buffer pointer
* @param[in] ncols number of image columns
* @param[in] nrows number of image rows
* @para,[in] rotation desired rotation angle (90, 180 or 270 degrees only)
**/
i32 RI_LONG (i32 *imptr, u32 ncols, u32 nrows, i32 rotation)
{
u32 x,y;
u32 bytes;
i32 *rotptr;
pan_double_t theta;
i32 i,j;
pan_double_t sine=0, cosine=0;
i32 jcorr, icorr;

	bytes = ncols*nrows*sizeof (i32);
	theta = RI_angle_vals (rotation, &sine, &cosine);
        rotptr = (i32 *) malloc (bytes);
        if ((imptr == NULL) || (rotptr == NULL))
                return (-EFAULT);

	if (rotation == 90) {jcorr =0; icorr = 1;}
	else {jcorr =1; icorr = 0;}
	pdebug ("RI_LONG: rotating in %g radians...\n", theta);
	for (y=0; y<nrows; y++)
		for (x=0; x<ncols; x++) {
			i = (i32)((f32)(x-(f32)ncols/2)*(f32)cosine-(f32)(y-(f32)nrows/2)*(f32)sine + nrows/2 -icorr ) ;
			j = (i32)((f32)(x-(f32)ncols/2)*(f32)sine+(f32)(y-(f32)nrows/2)*(f32)cosine + ncols/2 -jcorr ) ;
//			pdebug ("j %d i %d x %d y %d\n", j,  i, x, y);
			*(rotptr + j*nrows + i) = *(imptr + y*ncols + x);
		}

      	memcpy ((void *)imptr, (void *)rotptr, bytes);
	free (rotptr);
        return (OK);
}

/**
* Routine to rotate FLOAT image in the specified angle.
* @param[in] imptr image buffer pointer
* @param[in] ncols number of image columns
* @param[in] nrows number of image rows
* @para,[in] rotation desired rotation angle (90, 180 or 270 degrees only)
**/
i32 RI_FLOAT (f32 *imptr, u32 ncols, u32 nrows, i32 rotation)
{
u32 x,y;
u32 bytes;
f32 *rotptr;
pan_double_t theta;
i32 i,j;
pan_double_t sine=0, cosine=0;
i32 jcorr, icorr;

	bytes = ncols*nrows*sizeof(f32);
	theta = RI_angle_vals (rotation, &sine, &cosine);
        rotptr = (f32 *) malloc (bytes);
        if ((imptr == NULL) || (rotptr == NULL))
                return (-EFAULT);
	if (rotation == 90) {jcorr =0; icorr = 1;}
	else {jcorr =1; icorr = 0;}

	pdebug ("RI_FLOAT: rotating in %g radians...\n", theta);
	for (y=0; y<nrows; y++)
		for (x=0; x<ncols; x++) {
			i = (i32)((f32)(x-(f32)ncols/2)*(f32)cosine-(f32)(y-(f32)nrows/2)*(f32)sine + nrows/2 -icorr  ) ;
			j = (i32)((f32)(x-(f32)ncols/2)*(f32)sine+(f32)(y-(f32)nrows/2)*(f32)cosine + ncols/2 -jcorr ) ;
			*(rotptr + j*nrows + i) = *(imptr + y*ncols + x);
		}

      	memcpy ((void *)imptr, (void *)rotptr, bytes);
	free (rotptr);

        return (OK);
}


/**
* Routine to rotate an image in the specified angle. This calls the aproproate routine depending
* on the image datatype (bpp parameter)
* @param[in] imptr image buffer pointer
* @param[in] ncols number of image columns
* @param[in] nrows number of image rows
* @param[in] bpp bits per pixels in image (16, 32 or -32(float))
* @para,[in] rotation desired rotation angle (90, 180 or 270 degrees only)
**/
i32 Rotate_Image (ptr_t imageptr, u32 ncols, u32 nrows, i32 bpp, i32 rotation)
{
i32 ret;

        if (bpp == 16)
                ret=RI_SHORT ((i16 *)(dptr_t)imageptr, ncols, nrows, rotation);
        else if (bpp == 32)
                ret=RI_LONG ((i32 *)(dptr_t)imageptr, ncols, nrows, rotation);
        else if (bpp == -32)
                ret=RI_FLOAT ((f32 *)(dptr_t)imageptr, ncols, nrows, rotation);
        else return (-EINVAL);

        return (ret);
}

