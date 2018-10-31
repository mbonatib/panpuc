#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "../src/readLib.h"
#include <stdlib.h>

int main(void)
{

char timfile[90];
int fd;
int ret;
unsigned short *buf;
unsigned int exp_time=7000;
unsigned int ncols=2048, nrows=1024;
int nbytes;

	sprintf (timfile, "/home/sensei/fpas/_lskipper/config/DETECTOR/tim.lod");
	nbytes = ncols*nrows*2;
	buf = malloc (nbytes);
	if (buf == NULL) {
		printf ("malloc failed\n");
		goto bye;
	}

	ret = SDSU_InitLib ("/home/sensei/panview/PAN/Modules/SDSU/private/c/lib/libsdsugenIII_pcie.so");
	printf ("initi lib returned %d\n", ret);
	if ((fd = open_driver (0)) < 0){
		perror ("error opening the driver");
		printf ("errno %d %d\n", errno, fd);
		goto cls;
	}

	if ((ret = reset_controller (fd)) < 0){
		printf ("error resetting %d\n", ret);
		goto cls;
	}
	printf ("controller reseted\n");

        if ((ret = load_file (fd, timfile, 2))){
                printf ("error loading tim file %d\n", ret);
                goto cls;
        } else
                printf ("%s loaded\n", timfile);

        if ((ret = power (fd, 0)))
                printf ("error while doing power on %d\n", ret);

	if ((ret = dimensions (fd, ncols, nrows))){
		printf ("error while setting dims %d\n", ret);
		goto cls;
	} else
		printf ("dimensions set\n");

#if 1
	set_frames_per_buffer (fd, 1);
	set_ptr_val ((unsigned long) buf);
	set_bufs_per_frame (1);
	ret = start_exposure (fd);
	printf ("start_exp ret %d\n", ret);
	while (1) {
		ret = get_readout_progress (fd);
		printf ("%d\n", ret);
		usleep (100000);
	}
#endif
cls:	
	if (close_driver (fd) < 0){
		perror ("error closing driver ");
		return (-1);
	}
bye:
	free (buf); 

	return (0);
}

