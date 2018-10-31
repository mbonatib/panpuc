/****************************************************************************
 * Memory handling routines
 ***************************************************************************/

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mbuff.h"
#include "../../../../Common/public/c/include/GTypeDefs.h"

#define OK		0
#define STATS		4
#define MEAN(val)	*(val + 0)
#define VAR(val)	*(val + 1)
#define MAX(val)	*(val + 2)
#define MIN(val)	*(val + 3)
#define TFLOAT		-32
#define TLONG		32

#ifdef _HIGH_MEM_
  static char use_highmem = 1;
  static ptr_t highmem_fd=0;
#endif
static u32 *fowler_ptr=NULL;
ptr_t oldpos=0;
u32 gbl_pix=0;
u32 sample_cntr=0;
u32 ch_cntr=0;

#ifdef _DEBUG_
#  define pdebug(fmt, args...) printf ("LOGIC: " fmt, ## args)
#else
#  define pdebug(fmt, args...)
#endif

i32 Logic_mem_clear (ptr_t buf_pt, u32 nelems)
{
void *buf;

        buf = (void *)(dptr_t) buf_pt;
        if (buf == NULL)
                return (-EFAULT);

        pdebug ("clearing %d bytes from %lld\n", nelems, buf_pt);
        memset (buf, 0, nelems);
        return (OK);
}


/****************************************************************************
 * Look into a memory location and returns its value
 ***************************************************************************/
u16 mem_examine (ptr_t location, ptr_t off)
{
u16 value;

	if (location == 0)
		return (-EFAULT);
	value = (u16) *((u16 *)(dptr_t)location + off);
	printf ("value = 0x%x\n", value);
	return (value);
}


i32 get_imstats (ptr_t image_buf, u32 nelems, i32 bpp, f32 *stats)
{
ptr_t i;
u16 *image_ushort;
u32 *image_long;
f32 *image_float;
f32 sum;
pan_double_t sum2;
f32 mean, var, max, min, ele;

	pdebug ("getting stats from %lld (bpp %d)\n", image_buf, bpp);
	if ((u16 *)(dptr_t)image_buf == NULL)
		return (-EFAULT);
	max = -pow (2, 32) + 1;
	min = pow (2, 32) - 1; 
	sum = sum2 = 0;
	if (bpp == 16) {
		image_ushort = (u16 *)(dptr_t)image_buf;
		for (i=0; i<nelems; i++) {
                	ele = (f32) *(image_ushort + i);
                	sum += ele;
                	sum2 += (pan_double_t)(ele*ele);
                	if (ele > max)
                       		max = ele;
                	if (ele < min)
                       		min = ele;
		}
	} else if (bpp == TLONG) {
                image_long = (u32 *)(dptr_t)image_buf;
                for (i=0; i<nelems; i++) {
                        ele = (f32) *(image_long + i);
                        sum += ele;
                        sum2 += (pan_double_t)(ele*ele);
                        if (ele > max)
                                max = ele;
                        if (ele < min)
                                min = ele;
                }
	} else if (bpp == TFLOAT) {
                image_float = (f32 *)(dptr_t)image_buf;
                for (i=0; i<nelems; i++) {
                        ele = *(image_float + i);
                        sum += ele;
                        sum2 += (pan_double_t)(ele*ele);
                        if (ele > max)
                                max = ele;
                        if (ele < min)
                                min = ele;
                }
	} else
		return (-EINVAL);

	mean = (f32)(sum/nelems);
	var = (f32)((sum2/nelems) - (pan_double_t)(mean*mean));
	MEAN (stats) = mean;
	VAR (stats) = var;
	MAX (stats) = max;
	MIN (stats) = min;
	printf ("mean %f var %f min %f max %f\n", mean, var, min, max);
	return (OK);
}	

i32 reset_counters (void)
{
	pdebug ("Reset cntrs (was gbl_pix  %d. scntr %d)\n", gbl_pix, sample_cntr);
	sample_cntr=0;
	gbl_pix=0;
	ch_cntr=0;
	return (OK);
}


i32 samplesCoadd16To32 (ptr_t image_buf, ptr_t coadd_buff, u32 nelems, i32 nsamples, i32 nchann, i32 nskip)
{
u32 *coad;
u16 *raw_image;
u32 pix, inipix;

        coad = (u32 *)(dptr_t) coadd_buff;
        raw_image = (u16 *)(dptr_t) image_buf;
        if ((raw_image == NULL) || (coad == NULL))
                return (-EFAULT);

        if (gbl_pix == 0)               /*first frame*/
                inipix = nskip * nchann;        /*skip "nskip" pixels per channel*/
        else
                inipix = 0;

        for (pix=0; pix<nelems; pix++){
                *(coad + gbl_pix + ch_cntr) += (u32) (*(raw_image + pix));
                ch_cntr++;
                if (ch_cntr == nchann) {
                        ch_cntr=0;
                        sample_cntr++;
                }
                if (sample_cntr == nsamples){
                        gbl_pix+=nchann;
                        sample_cntr=0;
			ch_cntr=0;
                }
        }
        return (OK);
}

i32 samplesCoadd32To32 (ptr_t image_buf, ptr_t coadd_buff, u32 nelems, i32 nsamples, i32 nchann, i32 nskip)
{
u32 *coad;
u32 *raw_image;
u32 pix, inipix;

        coad = (u32 *)(dptr_t) coadd_buff;
        raw_image = (u32 *)(dptr_t) image_buf;
        if ((raw_image == NULL) || (coad == NULL))
                return (-EFAULT);

	if (gbl_pix == 0)		/*first frame*/
		inipix = nskip * nchann;	/*skip "nskip" pixels per channel*/
	else
		inipix = 0;

        for (pix=inipix; pix<nelems; pix++){
                *(coad + gbl_pix + ch_cntr) += *(raw_image + pix);
		ch_cntr++;
		if (ch_cntr == nchann) {
			ch_cntr=0;
                	sample_cntr++;
		}
                if (sample_cntr == nsamples){
                        gbl_pix+=nchann;
                        sample_cntr=0;
			ch_cntr=0;
                }
        }
        return (OK);
}



i32 samplesCoadd (ptr_t image_buf, ptr_t coadd_buff, u32 nelems, i32 nsamples, i32 bpp, i32 nchann, i32 nskip)
{
	pdebug ("samplescoadd: gbl_pix  %d. scntr %d nsamples %d, bpp %d\n", gbl_pix, sample_cntr, nsamples, bpp);
	if (nelems == 0)
		return (-EINVAL);
	if (nsamples <= 0)
		nsamples = 1;
	if (nchann <=0 ) 
		nchann = 1;
	if (nskip < 0 ) 
		nskip = 0;
	if (bpp == 16)
		return (samplesCoadd16To32 (image_buf, coadd_buff, nelems, nsamples, nchann, nskip));
	else
		return (samplesCoadd32To32 (image_buf, coadd_buff, nelems, nsamples, nchann, nskip));
	
}	

/****************************************************************************
 * Function for coadding images. It takes the image stored in the pointer
 * location, and adds it to the one in the coadd buffer location
 * (32 bits --> 32 bits)
 ***************************************************************************/
i32 coadd32To32 (ptr_t image_buf, ptr_t coadd_buff, u32 nelems)
{
u32 *coad;
u32 *subs_image;
u32 i;

	coad = (u32 *)(dptr_t) coadd_buff;
	subs_image = (u32 *)(dptr_t) image_buf;
	if ((subs_image == NULL) || (coad == NULL))
		return (-EFAULT);

	for (i=0; i<nelems; i++)
		*(coad + i)+= *(subs_image + i);

	return (OK);
}

i32 coadd16To32 (ptr_t image_buf, ptr_t coadd_buff, u32 nelems)
{
u32 *coad;
u16 *raw_image;
u32 i;

        coad = (u32 *)(dptr_t) coadd_buff;
        raw_image = (u16 *)(dptr_t) image_buf;
        if ((raw_image == NULL) || (coad == NULL))
                return (-EFAULT);

        for (i=0; i<nelems; i++)
                *(coad + i)+= (u32)*(raw_image + i);

        return (OK);
}
 
i32 coadd (ptr_t image_buf, ptr_t coadd_buff, u32 nelems, i32 in_bpp)
{

        if (in_bpp == 16)
                return (coadd16To32 (image_buf, coadd_buff, nelems));
        else
                return (coadd32To32 (image_buf, coadd_buff, nelems));
}


i32 coadd2 (ptr_t exposed_buf, ptr_t reset_buf, ptr_t coadd_buf, u32 nelems, f32 *mean, f32 *var, f32 *max, f32 *min)
{
i32 *coad;
u32 *exp_frame, *reset_frame;
u32 i, ele;
pan_double_t sum=0, sum2=0;

	pdebug ("performing %lld += (%lld - %lld)\n", coadd_buf, exposed_buf, reset_buf);
	*max = -pow (2, 32);
	*min = pow (2, 32);
	if ((exposed_buf == 0) || (reset_buf ==0) || (coadd_buf ==0)) 
		return (-EFAULT);

	coad = (i32 *)(dptr_t) coadd_buf;
	exp_frame = (u32 *)(dptr_t) exposed_buf;
	reset_frame = (u32 *)(dptr_t) reset_buf;

	for (i=0; i<nelems; i++){
		*(coad + i)+= (i32)(*(exp_frame + i) - *(reset_frame + i));
		ele = *(coad + i);
		sum += (pan_double_t)(ele);
		sum2 += (pan_double_t)(ele*ele);
		if ((f32)ele > *max)
                       *max = (f32)ele;
                if ((f32)ele < *min)
                       *min = (f32)ele;
	}
	*mean = (f32) (sum/nelems);
	*var = (f32)((sum2/nelems) - (pan_double_t)((*mean) * (*mean)));
	pdebug ("coadd2 mean %f var %f max %f min %f\n", *mean, *var, *max, *min);
	return (OK);
}

i32 coadd3 (ptr_t exposed_buf, ptr_t reset_buf, ptr_t coadd_buf, u32 nelems, f32 *mean, f32 *var, f32 *max, f32 *min, i32 num_fowler)
{
f32 *coad;
f32 fval;
u32 *exp_frame, *reset_frame;
u32 i;
f32 ele;
pan_double_t sum=0, sum2=0;

	pdebug ("performing %lld += (%lld - %lld)\n", coadd_buf, exposed_buf, reset_buf);
	*max = -pow (2, 32);
	*min = pow (2, 32);
	if ((exposed_buf == 0) || (reset_buf ==0) || (coadd_buf ==0)) 
		return (-EFAULT);

	if (num_fowler <=0) 
		num_fowler = 1;

	coad = (f32 *)(dptr_t) coadd_buf;
	exp_frame = (u32 *)(dptr_t) exposed_buf;
	reset_frame = (u32 *)(dptr_t) reset_buf;

	for (i=0; i<nelems; i++){
		fval = (f32)((f32)((i32)(*(exp_frame + i) - *(reset_frame + i)))/num_fowler);
//		fval = (float) (fval / num_fowler);
		*(coad + i)+= fval;
		ele = *(coad + i);
		sum += (pan_double_t)(ele);
		sum2 += (pan_double_t)(ele*ele);
		if ((f32)ele > *max)
                       *max = (f32)ele;
                if ((f32)ele < *min)
                       *min = (f32)ele;
	}
	*mean = (f32) (sum/nelems);
	*var = (f32)((sum2/nelems) - (pan_double_t)((*mean) * (*mean)));
//	printf ("coadd3 sum %f mean %f var %f max %f min %f nelems %ld num_fowler %d\n", sum, *mean, *var, *max, *min, nelems, num_fowler);
	return (OK);
}



/*performs the classic fowler sampling, in a frame to frame basis*/
i32 fowler (ptr_t image_buf, ptr_t faw_buff, u32 nelems)
{
u32 *fawler;
u16 *image;
u32 i;

	pdebug ("fowler samplig: image_buf %llu fow_buff %llu\n", image_buf, faw_buff);
	fawler = (u32 *)(dptr_t) faw_buff;
	image = (u16 *)(dptr_t) image_buf;
	if ((image == NULL) || (fawler == NULL))
		return (-EFAULT);

	for (i=0; i<nelems; i++)
		*(fawler + i)+= *(image + i);

	pdebug ("printf data %d, fowler %d\n", *image, *fawler);
	return (OK);
}

i32 LOGIC_Set_fowler_ptr (ptr_t fow_buf)
{
	if (fow_buf == 0)
		return (-EFAULT);
	fowler_ptr = (u32 *)(dptr_t) fow_buf; 
	return (OK);
}

/*performs fowler sampling in a line by line basis*/
i32 LOGIC_fowler_L2L (ptr_t image_buf, u32 nelems, u32 nsamples, u32 totcols)
{
u32 *fowler;
u16 *image;
u32 pix, row, samp, nrows, col;

	fowler = fowler_ptr;
	image = (u16 *)(dptr_t) image_buf;
	if ((image == NULL) || (fowler == NULL))
		return (-EFAULT);
	pix=0;
	nrows = (nelems/totcols)/nsamples;
	printf ("fowler samplig L2L: image_buf %lu fowler_ptr %lu nrows %d nsamples %d\n", (ulong)image_buf, (ulong)(ptr_t)(dptr_t) fowler_ptr, nrows, nsamples);
	for (row=0; row<nrows; row++){
		for (samp=0; samp<nsamples; samp++) {
			for (col=0; col<totcols; col++){
				*(fowler + row*totcols + col)+= *(image + pix);
				pix++;
			}
		}
	}
	printf ("fowler samplig L2L: pixels %d totmempos %ld\n", pix, (dptr_t) fowler + nrows*totcols - (dptr_t) fowler_ptr);
	fowler_ptr = fowler + nrows*totcols;
	return (OK);
}	

/*performs fowler sampling in a pixel by pixel basis*/
i32 LOGIC_fowler_P2P (ptr_t image_buf, u32 nelems, u32 nsamples, u32 totamps)
{
u32 *fowler;
u16 *image;
u32 pix, pixperamp, samp, amp, block;

	fowler = fowler_ptr;
	image = (u16 *)(dptr_t) image_buf;
	if ((image == NULL) || (fowler == NULL))
		return (-EFAULT);
	pix=0;
	pixperamp = (nelems/totamps)/nsamples;
	printf ("fowler samplig P2P: image_buf %ld fowler_ptr %ld totamps %d nsamples %d\n", (ulong)image_buf, (ulong)(ptr_t)(dptr_t) fowler_ptr, totamps, nsamples);
	for (block=0; block<pixperamp; block++){
		for (samp=0; samp<nsamples; samp++) {
			for (amp=0; amp<totamps; amp++){
				*(fowler + block*totamps + amp)+= *(image + pix);
				pix++;
			}
		}
	}
	fowler_ptr = fowler + pixperamp*totamps;
	return (OK);
}	
	

i32 operate_coadd_float (ptr_t coadd_buff, ptr_t out_buf, u32 nelems, u16 num_of_images, char *operation)
{
f32 *coad;
f32 *out_image;
u32 i;

	printf ("operating coadd float (%s) from %ld to %ld (numof images %d)\n", operation, (ulong)coadd_buff, (ulong)out_buf, num_of_images);
	coad = (f32 *)(dptr_t) coadd_buff;
	out_image = (f32 *)(dptr_t) out_buf;
	 if ((coad == NULL) || (out_image == NULL))
                return (-EFAULT);

	if (strcmp (operation, "mean") == 0) 
        	for (i=0; i<nelems; i++) {
	 		*(out_image + i) =  (f32)(((f32)*(coad + i))/num_of_images);
		}
	return (OK);
}


/****************************************************************************
 * Performs diferent actions over the coadd buffer (32 bits)  to an
 * output buffer (32 bits float) (by now only implemented the "mean")
 ***************************************************************************/
i32 operate_coadd (ptr_t coadd_buff, ptr_t out_buf, u32 nelems, u16 num_of_images, char *operation, i16 in_bpp)
{
i32  *coad;
f32 *out_image;
u32 i;

	if (in_bpp == TFLOAT)
		return (operate_coadd_float (coadd_buff, out_buf, nelems, num_of_images, operation));

	printf ("operating coadd long (%s) from %ld to %ld (numof images %d)\n", operation, (ulong)coadd_buff, (ulong)out_buf, num_of_images);
	coad = (i32 *)(dptr_t) coadd_buff;
	out_image = (f32 *)(dptr_t) out_buf;
	 if ((coad == NULL) || (out_image == NULL))
                return (-EFAULT);

	if (strcmp (operation, "mean") == 0) 
        	for (i=0; i<nelems; i++) {
	 		*(out_image + i) =  (f32)(((f32)*(coad + i))/num_of_images);
		}
	return (OK);
}


/****************************************************************************
 * Performs operations between two 16-bits images, giving as a result a
 * 32-bits image. Used, for example, when substracting reset and exposed
 * frames in IR operations
 ***************************************************************************/
i32 operate_images16To32 (ptr_t ptrimage1, ptr_t ptrimage2, ptr_t ptrimage3, u32 nelems, char *operation)
{
u32 i;
u16 *image1;
u16 *image2;
i32 *result;

	printf ("operate_image_16To32 %lu %lu %lu\n", (ulong)ptrimage1, (ulong)ptrimage2, (ulong)ptrimage3);
	image1 = (u16 *)(dptr_t)ptrimage1;
	image2 = (u16 *)(dptr_t)ptrimage2;
	result = (i32 *)(dptr_t)ptrimage3;


	if (strcmp (operation, "copy") == 0){
		if ((image1 == NULL) || (result == NULL))
			return (-EFAULT);
		for (i=0; i<nelems; i++)
			*(result + i) = (i32)(*(image1 + i));
		return (OK);
	}
	if (strcmp (operation, "substract") == 0) {
		if ((image1 == NULL) || (image2 == NULL) || (result == NULL))
			return (-EFAULT);
		for (i=0; i<nelems; i++)
			*(result + i) = (i32)(*(image1 + i) - *(image2 + i));
		return (OK);
	}

	return (-EINVAL);

}	


/****************************************************************************
 * Performs operations between two 16-bits images, giving as a result a
 * 16-bits image.
 ***************************************************************************/
int operate_images16To16 (ptr_t ptrimage1, ptr_t ptrimage2, ptr_t ptrimage3, u32 nelems, char *operation, i32 offset)
{
u32 i;
u16 *image1;
u16 *image2;
i16 *result;

	image1 = (u16 *)(dptr_t)ptrimage1;
	image2 = (u16 *)(dptr_t)ptrimage2;
	result = (i16 *)(dptr_t)ptrimage3;


	if (strcmp (operation, "copy") == 0){
		if ((image1 == NULL) || (result == NULL))
			return (-EFAULT);
		for (i=0; i<nelems; i++)
			*(result + i) = *(image1 + i);
		return (OK);
	}
	if (strcmp (operation, "substract") == 0) {
		if ((image1 == NULL) || (image2 == NULL) || (result == NULL))
			return (-EFAULT);
		for (i=0; i<nelems; i++)
			*(result + i) = (i16)(*(image1 + i) - *(image2 + i) + offset);
		return (OK);
	}

	return (-EINVAL);

}	


/****************************************************************************
 * Performs operations between two 32-bits images, giving as a result a
 * 32-bits image. Used, for example, when substracting reset and exposed
 * frames in IR operations
 ***************************************************************************/
i32 operate_images32To32 (ptr_t ptrimage1, ptr_t ptrimage2, ptr_t ptrimage3, u32 nelems, char *operation)
{
u32 i;
u32 *image1;
u32 *image2;
i32 *result;

        image1 = (u32 *)(dptr_t)ptrimage1;
        image2 = (u32 *)(dptr_t)ptrimage2;
        result = (i32 *)(dptr_t)ptrimage3;

        if (strcmp (operation, "copy") == 0){
        	if ((image1 == NULL) || (result == NULL))
                	return (-EFAULT);
                for (i=0; i<nelems; i++)
                        *(result + i) = *(image1 + i);
                return (OK);
        }
        if (strcmp (operation, "substract") == 0) {
        	if ((image1 == NULL) || (image2 == NULL) || (result == NULL))
                	return (-EFAULT);
                for (i=0; i<nelems; i++)
                        *(result + i) = (i32)(*(image1 + i) - *(image2 + i));
                return (OK);
        }

        return (-EINVAL);

}


i32 operate_images (ptr_t ptr1, ptr_t ptr2, ptr_t ptr3, u32 nelems, char *operation, i16 in_bpp)
{
	if (in_bpp == 16)
		return (operate_images16To32 (ptr1, ptr2, ptr3, nelems, operation));
	else
		return (operate_images32To32 (ptr1, ptr2, ptr3, nelems, operation));	
}


i32 operate_images_auto (ptr_t ptr1, ptr_t ptr2, ptr_t ptr3, u32 nelems, char *operation, i16 *bpp)
{
f32 stats1[STATS], stats2[STATS];
u32	max_dif;
	
	get_imstats (ptr1, nelems, 0, stats1);
	get_imstats (ptr2, nelems, 0, stats2);
	max_dif = MAX (stats2) - MIN (stats1);	/*absolute worst case*/	
	if (max_dif > pow (2,16)){		/*will use 32 bits out*/
		*bpp = 32;
		return (operate_images16To32 (ptr1, ptr2, ptr3, nelems, operation));	
	}
	if (max_dif > pow (2, 15)){		/*use 16 unsigned, add offset*/
		*bpp = 20;
		return (operate_images16To16 (ptr1, ptr2, ptr3, nelems, operation, MIN(stats1)));
	}
	*bpp = 16;				/*unsigned 16*/
	return (operate_images16To16 (ptr1, ptr2, ptr3, nelems, operation, 0));
}


/***************************************************************************
 *  * Return a pointer to the place where the last image chunk was written
 *   * This function is used for real time displaying and real time writing to disk
 *    ***************************************************************************/
ptr_t return_data_pointer (ptr_t mbuff, u32 buf_read, u32 cbuff_size, u32 dbuf_size, u32 *elems_to_write, i16 raw_bpp)
{

i32 bytes_to_write, bytes_remain, current_pos;

        if (buf_read <= 0)
	      return (0);

	if ((void *)(dptr_t) mbuff == NULL)
	      return (-ENOMEM);

	current_pos = (buf_read - 1) * dbuf_size;
        bytes_remain = cbuff_size -  current_pos;
        if (bytes_remain <0)
	        return (0);

        if (bytes_remain >= dbuf_size) 
                bytes_to_write = dbuf_size;
        else
                bytes_to_write = bytes_remain;

        *elems_to_write = (u32) bytes_to_write/((f32)raw_bpp/8);

	if (raw_bpp == 16) {
		u16 *sbuff;
		sbuff = (u16 *)(dptr_t)mbuff;
        	return ((ptr_t) (dptr_t) (sbuff + current_pos/sizeof(u16)));
	} else {
		u32 *lbuff;
		lbuff = (u32 *)(dptr_t)mbuff;
        	return ((ptr_t) (dptr_t) (lbuff + current_pos/sizeof(i32)));
	}
}

void clear_oldpos ()
{
        oldpos = 0;
}

ptr_t return_data_pointer2 (ptr_t mbuff, u32 buf_read, u32 cbuff_size, u32 dbuf_size, u32 *elems_to_write, i16 raw_bpp, u32 totbufs)
{

i32 bytes_to_write, bytes_remain, current_pos, newpos;

        if (buf_read <= 0)
              return (0);

        if ((void *)(dptr_t) mbuff == NULL)
              return (-ENOMEM);

        newpos = buf_read * dbuf_size;
       	bytes_remain = cbuff_size -  oldpos;

        if (bytes_remain <0){
                printf ("error, bytes_remain= %d !!)\n", bytes_remain);
                return (0);
        }
        if ((bytes_remain < dbuf_size) || (buf_read == totbufs))
                bytes_to_write = bytes_remain;
        else
                bytes_to_write = newpos - oldpos;


        *elems_to_write = (u32) bytes_to_write/((f32)raw_bpp/8);

        pdebug ("bytes_to_write: %d (%d ele), remaining %d (dbuf %d cbuff %d\n", bytes_to_write, *elems_to_write, bytes_remain, dbuf_size, cbuff_size);
        current_pos = oldpos;
        oldpos = newpos;
        if (raw_bpp == 16) {
                u16 *sbuff;
                sbuff = (u16 *)(dptr_t)mbuff;
                return ((ptr_t) (dptr_t) (sbuff + current_pos/sizeof(i16)));
        } else {
                u32 *lbuff;
                lbuff = (u32 *)(dptr_t)mbuff;
                return ((ptr_t) (dptr_t) (lbuff + current_pos/sizeof(i32)));
        }
}

