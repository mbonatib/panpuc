/*
	A few definitions to handle data transfer between LabVIEW
	and Linux shared libraries.
	
	In LabVIEW, array structures, being them characters or numbers,
	consists of a size parameter followed by the actual data.
	
	When calling a share library function, LabVIEW passes
	a handle, that consists of a pointer to a pointer to
	the four-byte value of the size parameter.
	
	added TrueTime GPS card support (G.S. 6.aug.2002)
	
*/

#ifndef _soar_lv_H
#define _soar_lv_H

#include <linux/ioctl.h>

#define u32 unsigned long
#define u16 unsigned int
#define i32 long
#define u8 unsigned char
#define TT_IO_MAGIC 'l'
#define TT_GENQ_WAIT            _IOR(TT_IO_MAGIC, 0x1f, u32)

/*
 * Define BCD - INT - BCD macros.
 */
#define BCDTOI(a)       ((((a&0xf0) >> 4) * 10) + (a&0x0f))

				/* int32 handle			*/
typedef struct {
	int32 cnt;		/* number of elements in array	*/
	int32 num[1];		/* the array elements		*/
	} **LInt32Handle;
#define LInt32Buf(sp)	(&((sp))->num[0])
#define LInt32Len(sp)	(((sp))->cnt)
#define LInt32Size(sp)	(LInt32Len(sp)+1)

				/* int16 handle			*/
typedef struct {
	int32 cnt;		/* number of elements in array	*/
	int16 num[1];		/* the array elements		*/
	} **LInt16Handle;
#define LInt16Buf(sp)	(&((sp))->num[0])
#define LInt16Len(sp)	(((sp))->cnt)
#define LInt16Size(sp)	(LInt32Len(sp)+1)
	
				/* float64 handle		*/
typedef struct {
	int32 cnt;		/* number of elements in array	*/
	float64 num[1];		/* the array elements		*/
	} **LF64Handle;
#define LF64Buf(sp)	(&((sp))->num[0])
#define LF64Len(sp)	(((sp))->cnt)
#define LF64Size(sp)	(LInt32Len(sp)+1)
	
				/* float32 handle		*/
typedef struct {
	int32 cnt;		/* number of elements in array	*/
	float32 num[1];		/* the array elements		*/
	} **LF32Handle;
#define LF32Buf(sp)	(&((sp))->num[0])
#define LF32Len(sp)	(((sp))->cnt)
#define LF32Size(sp)	(LInt32Len(sp)+1)
	
	
#endif
