#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "GU_transformations.h"

#define PI		3.141598
#define OK		0

/**
* Calculates statictics over an image
* @param[in] image_buf image buffer pointer
* @param[in] image_buf nelems numbe rof elements (pixels) on image
* @param[out] *stats output stats array: Mean (0), Variance (1), Max (2) and Min (3)
**/
int GU_SHORT_stats_mean (unsigned long image_buf, unsigned long nelems, float *stats)
{
unsigned long i;
unsigned short *image_ushort;
long *image_long;
float *image_float;
float sum;
double  sum2;
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
        } else if (bpp == TLONG) {
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
        } else if (bpp == TFLOAT) {
                image_float = (float *)image_buf;
                for (i=0; i<nelems; i++) {
                        ele = *(image_float + i);
                        sum += ele;
                        sum2 += (double)(ele*ele);
                                                                                 45,0-1         6%
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

