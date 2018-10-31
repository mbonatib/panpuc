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
#include <sys/time.h>
#include <time.h>


#define OK		0
#define STATS		4
#define MEAN(val)	*(val + 0)
#define VAR(val)	*(val + 1)
#define MAX(val)	*(val + 2)
#define MIN(val)	*(val + 3)

/****************************************************************************
 * Look into a memory location and returns its value
 ***************************************************************************/
unsigned short mem_examine (unsigned long location, unsigned long off)
{
unsigned short value;

	if (location <= 0)
		return (-EFAULT);
	value = (unsigned short) *((short *)location + off);
	printf ("value = 0x%x\n", value);
	return (value);
}


int get_imstats (unsigned long image_buf, unsigned long nelems, int bpp, float *stats)
{
unsigned long i;
unsigned short *image_ushort;
long *image_long;
float *image_float;
float sum;
double	sum2;
float mean, var, max, min, ele;

	printf ("getting stats from %ld (bpp %d)\n", image_buf, bpp);
	if ((unsigned short *)image_buf == NULL)
		return (-EFAULT);
	max = -pow (2, 32) + 1;
	min = pow (2, 32) - 1; 
	sum = sum2 = 0;
	if (bpp == 16) {
		image_ushort = (unsigned short *)image_buf;
		for (i=0; i<nelems; i++) {
                	ele = (float) *(image_ushort + i);
                	sum += ele;
                	sum2 += (double)(ele*ele);
                	if (ele > max)
                       		max = ele;
                	if (ele < min)
                       		min = ele;
		}
	} else if (bpp == 32) {
                image_long = (long *)image_buf;
                for (i=0; i<nelems; i++) {
                        ele = (float) *(image_long + i);
                        sum += ele;
                        sum2 += (double)(ele*ele);
                        if (ele > max)
                                max = ele;
                        if (ele < min)
                                min = ele;
                }
	} else if (bpp == -32) {
                image_float = (float *)image_buf;
                for (i=0; i<nelems; i++) {
                        ele = *(image_float + i);
                        sum += ele;
                        sum2 += (double)(ele*ele);
                        if (ele > max)
                                max = ele;
                        if (ele < min)
                                min = ele;
                }
	} else
		return (-EINVAL);

	mean = (float)(sum/nelems);
	var = (float)((sum2/nelems) - (double)(mean*mean));
	MEAN (stats) = mean;
	VAR (stats) = var;
	MAX (stats) = max;
	MIN (stats) = min;
	printf ("mean %f var %f min %f max %f\n", mean, var, min, max);
	return (OK);
}	


int get_ut_time (int *year, int *month, int *day, int *hour, int *min, double *sec)
{
struct timeval tval;
struct tm *tm_time;
time_t startsecs;
//double startusecs;

        gettimeofday (&tval, NULL);
        startsecs = tval.tv_sec;
//        startusecs = tval.tv_usec / 1000000;
       	tm_time = gmtime (&startsecs);
	*year = 1900 + tm_time -> tm_year;
	*month = 1 + tm_time -> tm_mon;
	*day = tm_time -> tm_mday;
	*hour= tm_time -> tm_hour;
	*min = tm_time -> tm_min;
	*sec= (double) tm_time -> tm_sec+(double) tval.tv_usec/1000000;
	return (OK);
}
