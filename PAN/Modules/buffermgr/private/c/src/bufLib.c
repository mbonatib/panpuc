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
#include "../../../../Common/public/c/include/GTypeDefs.h"

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

u64 totMem=0;
u64 maxMem=0;		/*0 means no maximum limit*/


int BUFMGR_set_maxMem (u64 megs)
{
	if (megs <= 0)
		maxMem = 0;
	else
		maxMem = megs * 1024 * 1024;	/*to bytes*/
	
	return (OK);
}

int BUFMGR_released_size (u64 size)
{
	if (size <= totMem)
		totMem = totMem - size;
	else
		totMem = 0;

	return (OK);
}

u64 BUFMGR_get_maxMem ()
{
	return ((u64) (pan_double_t)maxMem/(1024*1024));
}

u64 BUFMGR_get_totMem ()
{
	return ((u64) (pan_double_t)totMem/(1024*1024));
}
	
/****************************************************************************
 * Look into a memory location and returns its value
 ***************************************************************************/
u16 mem_examine (ptr_t location, u64 off)
{
u16 value;

	if (location <= 0)
		return (-EFAULT);
	value = (u16) *((i16 *)(dptr_t)location + (mem_t) off);
	printf ("value = 0x%x\n", value);
	return (value);
}


/****************************************************************************
 * Clears a zone of memory
 ***************************************************************************/
int mem_clear (ptr_t buf_pt, u64 nelems)
{
void *buf;

	buf = (void *) (dptr_t) buf_pt;
	if (buf == NULL)
		return (-EFAULT);

	printf ("clearing %ld bytes from %ld\n", (ulong)nelems, (ulong)buf_pt);
	memset (buf, 0, (mem_t) nelems);
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
 * Allocates high memory (if using high memory)
 ***************************************************************************/
#ifdef _HIGH_MEM_
ptr_t highmem_alloc (char *name, u64 byte_size)
{
unsigned long info[2];
void *buff;
int ret;

	info[0] = (int) ceil (byte_size/1024);
/*	printf ("highmem: asked %ld kbytes\n", info[0]);*/
        if ((ret = ioctl (highmem_fd, ALLOCATOR_GETMEM, &info[0])) < 0) {
		printf ("error in ioctl %d\n", errno);
		return (0);
	}
        if ((buff = (void *) mmap (0, info[1] * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, highmem_fd, info[0])) < 0){
                printf ("mmap failed\n");
                return (0);
        }
/*
        printf ("info[0] = %ld info[1] = %ld buff=%p\n", info[0], info[1], buff); */
	return ((ptr_t) (dptr_t) buff);
}
#endif

#if 0
int highmem_free (char *name, unsigned long address)
{
int ret;
unsigned long add;

	add = address;
        if ((ret = ioctl (highmem_fd, ALLOCATOR_FREEMEM, &add)) < 0){
                printf ("error in ioctl %d\n", errno);
                return (-errno);
        } 
	return (OK);
}
#endif


/****************************************************************************
 * Frees high memory, if in use
 ***************************************************************************/
#ifdef _HIGH_MEM_
void highmem_cleanup (char *name, ptr_t address)
{
	ioctl (highmem_fd, ALLOCATOR_CLEANUP, 0);
	close (highmem_fd);
}	
#endif


/****************************************************************************
 * Allocates a memory buffer. Uses the allocator driver if using high mem,
 * or plain malloc if using regular memory space
 ***************************************************************************/
ptr_t mem_alloc (char *name, u64 size, i32 *err)
{
#ifdef _HIGH_MEM_
	if (use_highmem)
		return (highmem_alloc (name, size));
#endif
ptr_t ptr;
u64 newSize;

	*err = 0;
	newSize = totMem + size;
	if (maxMem > 0) { 
		if (newSize > maxMem) {
			*err = -ENOMEM; 
			return (0);
		}
	}
	ptr = (ptr_t) (dptr_t) malloc ((mem_t)size);
	if (ptr == 0) {
		*err = -ENOMEM; 
		return (0);
	} else {
		totMem = newSize;
		return (ptr);
	}
}


/****************************************************************************
 * Frees memory. Uses the high mem driver if using high memory, and just
 * free if not
 ***************************************************************************/
int mem_free (char *name, ptr_t buf)
{
printf ("freeing memory pointer %ld\n", (ulong)(dptr_t)buf);
#ifdef _HIGH_MEM_
	if (use_highmem){
		highmem_cleanup(name, buf);
		return (OK);
	}
#endif
	if ((void *)(dptr_t)buf != NULL)
        	free ((void *)(dptr_t)buf);
	
	return (OK);
}                                                                               
