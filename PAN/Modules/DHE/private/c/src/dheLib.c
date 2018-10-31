#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define OK		0

#ifdef _DEBUG_
#  define pdebug(fmt, args...) printf ("dhe: " fmt, ## args)
#else
#  define pdebug(fmt, args...)
#endif
double dhe_var_elapsed=0;
short dhe_var_stop_integ=0;
short dhe_var_stopped=0;
short dhe_var_pause=0;
int dhe_var_resol_us=500;
time_t dhe_var_paused_time=0;

int dhe_get_elapsed (int *darktime)
{
	*darktime = dhe_var_paused_time + dhe_var_elapsed;
	return (dhe_var_elapsed);
}

int dhe_stop_integ(void)
{
int cnt=0;

	dhe_var_stop_integ = 1;
	while (!dhe_var_stopped){
		usleep (500);
		if (cnt++ > 4000)
			return (-ETIME);
	}
	return (dhe_var_elapsed);
}

void dhe_init_integ(void)
{
	dhe_var_stop_integ = 0;
	dhe_var_stopped = 0;
	dhe_var_elapsed = 0;
	dhe_var_pause = 0;
	dhe_var_paused_time = 0;
}

void dhe_set_resol (double resol_ms)
{
	if ((resol_ms <= 0) || (resol_ms > 100.0))
		dhe_var_resol_us = 500;
	else
		dhe_var_resol_us = resol_ms * 1000;
}

int dhe_timer (unsigned int msecs, int *darktime)
{
struct timeval tval;
time_t beg, end, ptime;

	dhe_init_integ();
	ptime = 0;
	gettimeofday (&tval, NULL);
	beg = 1000000*tval.tv_sec + tval.tv_usec;
	end = beg;
	while (dhe_var_elapsed < (double) msecs){
		usleep (dhe_var_resol_us);
		gettimeofday (&tval, NULL);
		if (dhe_var_pause){
			ptime = 1000000*tval.tv_sec + tval.tv_usec - end;
			dhe_var_paused_time = dhe_var_paused_time + ptime;
			end = end + ptime;
			beg = beg + ptime;
			printf ("Paused...");
		} else {
			end = 1000000*tval.tv_sec + tval.tv_usec;
		}
		dhe_var_elapsed = (double)(end - beg) / 1000.0;
		printf ("local timer: %d of %d\r", (int) dhe_var_elapsed, msecs);
		if (dhe_var_stop_integ)
			break;
	}
	dhe_var_stopped = 1;
	printf ("local timer: %d of %d\n", (int) dhe_var_elapsed, msecs);
	if (!dhe_var_stop_integ)
		dhe_var_elapsed = (double) msecs;

	*darktime = dhe_var_paused_time + dhe_var_elapsed;
        return ((int) dhe_var_elapsed);
}

int dhe_pause (void)
{
	dhe_var_pause = 1;
	return (OK);
}

int dhe_resume (void)
{
	dhe_var_pause = 0;
        return (OK);
}


