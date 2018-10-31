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
#include "ggmem_io.h"
#include "GTypeDefs.h"

#define OK		0
#define STATS		4
#define MEAN(val)	*(val + 0)
#define VAR(val)	*(val + 1)
#define MAX(val)	*(val + 2)
#define MIN(val)	*(val + 3)

#ifdef _HIGH_MEM_
  static char use_highmem = 1;
  static int highmem_fd=0;
#endif

/****************************************************************************
 * Look into a memory location and returns its value
 ***************************************************************************/
unsigned short CMN_mem_examine (ptr_t location, u32 off)
{
unsigned short value;

	if (location <= 0)
		return (-EFAULT);
	value = (unsigned short) *((short *)(dptr_t)location + off);
	printf ("value = 0x%x\n", value);
	return (value);
}


i32 CMN_get_imstats (ptr_t image_buf, u32 nelems, i32 bpp, f32 *stats)
{
u32 i;
u16 *image_ushort;
i32 *image_long;
f32 *image_float;
f32 sum;
pan_double_t sum2;
f32 mean, var, max, min, ele;

	printf ("getting stats from %ld (bpp %d)\n", image_buf, bpp);
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
	} else if (bpp == 32) {
                image_long = (i32 *)(dptr_t)image_buf;
                for (i=0; i<nelems; i++) {
                        ele = (f32) *(image_long + i);
                        sum += ele;
                        sum2 += (pan_double_t)(ele*ele);
                        if (ele > max)
                                max = ele;
                        if (ele < min)
                                min = ele;
                }
	} else if (bpp == -32) {
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


/****************************************************************************
 * Function for coadding images. It takes the image stored in the pointer
 * location, and adds it to the one in the coadd buffer location
 * (32 bits --> 32 bits)
 ***************************************************************************/
i32 CMN_coadd (ptr_t image_buf, ptr_t coadd_buff, u32 nelems)
{
u32 *coad;
u32 *subs_image;
i32 i;

	coad = (u32 *)(dptr_t) coadd_buff;
	subs_image = (u32 *)(dptr_t) image_buf;
	if ((subs_image == NULL) || (coad == NULL))
		return (-EFAULT);

	for (i=0; i<nelems; i++)
		*(coad + i)+= *(subs_image + i);

	return (OK);
}


int CMN_coadd2 (ptr_t exposed_buf, ptr_t reset_buf, ptr_t coadd_buf, u32 nelems, f32 *mean, f32 *var, f32 *max, f32 *min)
{
i32 *coad;
u32 *exp_frame, *reset_frame;
i32 i, ele;
pan_double_t sum=0, sum2=0;

	printf ("cmn performing %ld += (%ld - %ld)\n", coadd_buf, exposed_buf, reset_buf);
	*max = -pow (2, 32);
	*min = pow (2, 32);
	if ((exposed_buf <= 0) || (reset_buf <=0) || (coadd_buf <=0)) 
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
//	printf ("coadd2 mean %f var %f max %f min %f\n", *mean, *var, *max, *min);
	return (OK);
}



/****************************************************************************
 * Performs diferent actions over the coadd buffer (32 bits)  to an
 * output buffer (32 bits float) (by now only implemented the "mean")
 ***************************************************************************/
i32 CMN_operate_coadd (ptr_t coadd_buff, ptr_t out_buf, u32 nelems, u16 num_of_images, char *operation)
{
i32 *coad;
f32 *out_image;
i32 i;

	printf ("operating coadd (%s) from %ld to %ld (numof images %d)\n", operation, coadd_buff, out_buf, num_of_images);
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
i32 CMN_operate_images16To32 (ptr_t ptrimage1, ptr_t ptrimage2, ptr_t ptrimage3, u32 nelems, char *operation)
{
i32 i;
u16 *image1;
u16 *image2;
i32 *result;

	printf ("operate_image_16To32 %lu %lu %lu\n", ptrimage1, ptrimage2, ptrimage3);
	image1 = (u16 *)(dptr_t)ptrimage1;
	image2 = (u16 *)(dptr_t)ptrimage2;
	result = (i32 *)(dptr_t)ptrimage3;

	if ((image1 == NULL) || (image2 == NULL) || (result == NULL))
		return (-EFAULT);

	if (strcmp (operation, "copy") == 0){
		for (i=0; i<nelems; i++)
			*(image2 + i) = *(image1 + i);
		return (OK);
	}
	if (strcmp (operation, "substract") == 0) {
		for (i=0; i<nelems; i++)
			*(result + i) = (i32)((i32)*(image1 + i) - (i32)*(image2 + i));
		return (OK);
	}


	return (-EINVAL);

}	


/****************************************************************************
 * Performs operations between two 16-bits images, giving as a result a
 * 16-bits image.
 ***************************************************************************/
i32 CMN_operate_images16To16 (ptr_t ptrimage1, ptr_t ptrimage2, ptr_t ptrimage3, u32 nelems, char *operation, i32 offset)
{
i32 i;
u16 *image1;
u16 *image2;
i16  *result;

	image1 = (u16 *)(dptr_t)ptrimage1;
	image2 = (u16 *)(dptr_t)ptrimage2;
	result = (i16 *)(dptr_t)ptrimage3;

	if ((image1 == NULL) || (image2 == NULL) || (result == NULL))
		return (-EFAULT);

	if (strcmp (operation, "copy") == 0){
		for (i=0; i<nelems; i++)
			*(image2 + i) = *(image1 + i);
		return (OK);
	}
	if (strcmp (operation, "substract") == 0) {
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
i32 CMN_operate_images32To32 (ptr_t ptrimage1, ptr_t ptrimage2, ptr_t ptrimage3, u32 nelems, char *operation)
{
i32 i;
u32 *image1;
u32 *image2;
i32 *result;

        image1 = (u32 *)(dptr_t)ptrimage1;
        image2 = (u32 *)(dptr_t)ptrimage2;
        result = (i32 *)(dptr_t)ptrimage3;

        if ((image1 == NULL) || (image2 == NULL) || (result == NULL))
                return (-EFAULT);

        if (strcmp (operation, "copy") == 0){
                for (i=0; i<nelems; i++)
                        *(image2 + i) = *(image1 + i);
                return (OK);
        }
        if (strcmp (operation, "substract") == 0) {
                for (i=0; i<nelems; i++)
                        *(result + i) = (i32)((i32)*(image1 + i) - (i32)*(image2 + i));
                return (OK);
        }

        return (-EINVAL);

}



i32 CMN_operate_images_auto (ptr_t ptr1, ptr_t ptr2, ptr_t ptr3, u32 nelems, char *operation, i16 *bpp)
{
f32 stats1[STATS], stats2[STATS];
i32	max_dif;
	
	CMN_get_imstats (ptr1, nelems, 0, stats1);
	CMN_get_imstats (ptr2, nelems, 0, stats2);
	max_dif = MAX (stats2) - MIN (stats1);	/*absolute worst case*/	
	if (max_dif > pow (2,16)){		/*will use 32 bits out*/
		*bpp = 32;
		return (CMN_operate_images16To32 (ptr1, ptr2, ptr3, nelems, operation));	
	}
	if (max_dif > pow (2, 15)){		/*use 16 unsigned, add offset*/
		*bpp = 20;
		return (CMN_operate_images16To16 (ptr1, ptr2, ptr3, nelems, operation, MIN(stats1)));
	}
	*bpp = 16;				/*unsigned 16*/
	return (CMN_operate_images16To16 (ptr1, ptr2, ptr3, nelems, operation, 0));
}


/****************************************************************************
 * Clears a zone of memory
 ***************************************************************************/
int CMN_mem_clear (ptr_t buf_pt, u32 nelems)
{
void *buf;

	buf = (void *)(dptr_t) buf_pt;
	if (buf == NULL)
		return (-EFAULT);

	printf ("clearing %d bytes from %ld\n", nelems, buf_pt);
	memset (buf, 0, nelems);
	return (OK);
} 


/****************************************************************************
 * Initializes the highmem driver is we are using high memory for the buffers
 * Does nothing is using plain malloc
 ***************************************************************************/
int mem_init (void)
{
#ifdef _HIGH_MEM_
        if ((highmem_fd = open ("/dev/highmem", O_RDWR)) < 0){
                printf ("error opening %d\n", errno);
		use_highmem = 0;
                return (-errno);
	}
	use_highmem = 1;
#endif
	 return (OK); 
}


/****************************************************************************
 * Allocates a memory buffer. Uses the allocator driver if using high mem,
 * or plain malloc if using regular memory space
 ***************************************************************************/
ptr_t CMN_mem_alloc (char *name, u32 size)
{
ptr_t ptr;

	ptr = (ptr_t)(dptr_t)malloc (size);
	if (ptr <= 0)
		return (-ENOMEM);
	else
		return (ptr);
}


/****************************************************************************
 * Frees memory. Uses the high mem driver if using high memory, and just
 * free if not
 ***************************************************************************/
i32 CMN_mem_free (char *name, ptr_t buf)
{
printf ("freeing memory pointer %ld\n", buf);
	if ((void *)(dptr_t)buf != NULL)
        	free ((void *)(dptr_t)buf);
	
	return (OK);
}                                                                               
