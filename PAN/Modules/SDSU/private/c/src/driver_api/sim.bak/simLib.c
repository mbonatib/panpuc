/* Library for accesng the Leach PCI driver */
#include "sdsu_defs.h"
#include "simLib.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/mman.h>

#ifdef _DEBUG_
#  define pdebug(fmt, args...) printf ("sim: " fmt, ## args)
#else
#  define pdebug(fmt, args...)
#endif
#define DSP_PRESCANS 0
#define MAXADDS 4096

static int alldone=0;
int fpb=1;
int framescntr=0;
int dettype=OPTICAL;
int cds=1;
int nsamp=1;
int sampcntr=0;
int ncoadd=1;
int coaddcntr=0;
int totexptime=0;
static int resetting=0;
static int reading=0;
static int clearing=0;
double gbl_pixtime=1.0;
double ronswing, sigswing;
char devname[30];
unsigned short *highmem=NULL;
unsigned short *currptr=NULL;
int mapcnt;
static int opened_mapsize=0;
static int maxmappix=0;
static int cmd_data[10];
int totpix;
struct timeval timest;
double beg, end;
add_t   adds[MAXADDS];
int nadds=0;

int find_sim_add (unsigned int board, unsigned int type, unsigned int add)
{
int i;

        for (i=0; i<nadds; i++){
                if (adds[i].board == board){
                	if (adds[i].type == type){
                		if (adds[i].address == add){
                       		 	return (i);
				}
			}
		}
	}

        i = nadds;
	if (nadds == MAXADDS) {
		i = MAXADDS-1;
		printf ("WARNING: maxadds exceeded (%d)\n", nadds);
	} else
        	nadds++;

        adds[i].board = board;
        adds[i].address = add;
        adds[i].type = type;
        adds[i].value = 0;
        return (i);
}

void set_sim_add (unsigned int board, unsigned int type, unsigned int add, int value)
{
        adds[find_sim_add (board, type, add)].value = value;
}

int get_sim_add (unsigned int board, unsigned int type, int add)
{
unsigned int ret;

        ret = adds[find_sim_add (board, type, add)].value;

        return (ret);
}

/*************************************************************************
 * Function for reading the reply from the Leach Controller (to a command)
 *************************************************************************/
int read_reply (int fd, int expected, int timeout)
{
int val;

	val = cmd_data[0];
	pdebug ("cmd_data[0]=0x%x cmd_data[1]=0x%x\n", cmd_data[0], cmd_data[1]);
	if (val != expected) {
		if (val == ERR)
			val = -EIO;
		else if (val == TOUT)
			val = -ETIME;
		else if (expected != 0)
			val = -EIO;
	} else
		val = DONE;

	return (val);		
}


int set_cmd_data (int fd, int data)
{
	return (DONE);
}


//***********************************************************************
int do_command (int fd, int board_id, int command, int arg1, int arg2, int arg3, int arg4, int expected_reply, int timeout)
{
int ret=DONE;
unsigned int num_type, address;
int gain=1;

		switch (command){
			case SEX: 
				gettimeofday (&timest, NULL);
				beg = (double)1000*timest.tv_sec + (double)timest.tv_usec/1000;
				reset_buffer();
				framescntr=0;
				if (fpb <= 0)
					fpb = 1;
				gain = 1;
       				if (ron<0)
                			ron = DEFRON;
				if (biaslevel < 0)
					biaslevel = DEFBIASLEVEL;
				ronswing = 6*sqrt (ron) / gain;
				clearing=1;
				resetting=0;
				reading=0;
				alldone=0;
				totexptime=0;
				printf ("exposure started\n"); 
				break;
			case FPB:
				fpb=arg1;
				printf ("setting frames per buffer to %d\n", fpb); 
				ret=DONE;
				break;
			case SET:
				printf ("setting exptime to %d\n", arg1); 
				break;
			case RET: 
				gettimeofday (&timest, NULL);
				end = (double) 1000*timest.tv_sec + (double)timest.tv_usec/1000;
				totexptime = (int) (end-beg);
				if (totexptime <0)
					totexptime=0;
				ret = totexptime;
				
//				printf ("returning exptime %d\n", ret); 
				break;
			case WRM: 
				num_type = arg1 & 0xF00000;
				address = arg1 & 0x0FFFFF;
				set_sim_add (board_id, num_type, address, arg2);
//				pdebug ("write_mem  %d 0x%x 0x%x %d\n",board_id, num_type, address, arg2);
				ret = DONE;
				break;
			case RDM:
				num_type = arg1 & 0xF00000;
				address = arg1 & 0x0FFFFF;
				ret = get_sim_add (board_id, num_type, address);
//				pdebug ("read_mem %d 0x%x 0x%x: %d\n", board_id, num_type, address, ret);
				break;
			case RCC:
				if (dettype == NIR)
					ret = TIMREV5+UTILREV3+IR8;
				else
					ret = TIMREV5+UTILREV3+VIDREV5;
				break;
			case SNR:
			case SFS:
				if (dettype == NIR){
					nsamp = arg1;
					printf ("nsamples %d\n", nsamp);
					ret = DONE;
				} else
					ret = -EIO;
				break;
			case SNC:
				if (dettype == NIR){
					ncoadd = arg1;
					printf ("ncoadds %d\n", ncoadd);
					ret = DONE;
				} else
					ret = -EIO;
				break;
                       case CDS:
                                if (dettype == NIR){
					cds=arg1;
					printf ("sampling cds %d\n", cds);
                                        ret = DONE;
                                } else
                                        ret = -EIO;
                                break;
			default:
				break;
		}

		return (ret);
}


int set_size (int fd, int ncols, int nrows)
{
	totpix = ncols*nrows;
	printf ("SIM: totpix %d\n", totpix);

	return (DONE);
}


/**********************************************************************
 * Get the status from the HCTR register
 *********************************************************************/
int get_hctr (int fd)
{
	return (DONE);
}


/**********************************************************************
 * Get the status from the HSTR register
 *********************************************************************/
int get_hstr (int fd)
{
int val=0;

        return (val);
}

int set_pixtime (int fd, double pixtime)
{
	printf ("pixel time in %6.2f\n", pixtime);
	if (pixtime > 0)
		gbl_pixtime = pixtime;

	return (DONE);
}

double get_pixtime (int fd)
{
	printf ("pixel time is %6.2f\n", gbl_pixtime);
        return (gbl_pixtime);
}

void reset_buffer (void)
{
	printf ("SIM: resetting buffer to higmem\n");
	currptr = highmem;
	mapcnt = 0;
}

int get_frames_progress (int fd)
{
	return (1);
}

/**********************************************************************
 * Get the readout progress (how many buffers has been read till now)
 *********************************************************************/
int get_read_progress (int fd)
{
int pix=0;
#if 1
	if (alldone){
		printf ("SIM: all is done\n");
		return (mapcnt);
	}
#endif
//	printf ("claring %d restting %d reading %d\n", clearing, resetting, reading);
	if (clearing){
		printf ("SIM: clearing\n");
		usleep (500000);
		clearing=0;
		if (dettype == NIR){
			if (cds==1)
				resetting=1;
			else
				reading=1;
			if (nsamp == 0)
				nsamp = 1;
		} else {
			reading=1;
			nsamp = 1;
		}
		pix = -1;
		return (pix);
	}
	if (resetting){
		pix = get_chunk(0);
		if (pix >= totpix){
			pix = totpix;
			sampcntr++;
			framescntr++;
		        printf ("SIM: reset frame %d done\n", sampcntr);
			if (framescntr >= fpb){
				reset_buffer();
				framescntr=0;
			}
			if (sampcntr >= nsamp){
				sampcntr=0;
				resetting=0;
				reading=1;
		               gettimeofday (&timest, NULL);
   			       beg = (double)1000*timest.tv_sec + (double)timest.tv_usec/1000;
		               printf ("SIM: start integration\n");
			}
		}
		return (pix);
	}
	if (reading){
		pix = get_chunk(1);
		if (pix >= totpix){
			pix = totpix;
			sampcntr++;
			framescntr++;
		        printf ("SIM: read frame %d done\n", sampcntr);
			if (framescntr >= fpb){
				reset_buffer();
				framescntr=0;
			}
			if (sampcntr >= nsamp){
				sampcntr=0;
				clearing=0;
				resetting=0;
				reading=0;
				reset_buffer();
				alldone=1;
		               printf ("SIM: finished block\n");
			}
		}
		return (pix);
	}
	return (pix);
}
				

/**********************************************************************
 * Get the readout progress (how many buffers has been read till now)
 *********************************************************************/
int get_chunk (int expo)
{
int chunk= 65535;
int i,a,p;
unsigned int delay;
double rnb, rns;
int totsignal;

	delay = chunk*gbl_pixtime;
	usleep (delay);
	if (expo)
		totsignal = siglevel * totexptime;
	else
		totsignal = 0;
	sigswing = 3*sqrt(totsignal);

	if (artifdata == ARTIFDET){
		for (i=0; i<chunk; i++){
                        rnb = 0.5 - (double)random ()/ RAND_MAX;
                        rns = 0.5 - (double)random ()/ RAND_MAX;
			*currptr = biaslevel + totsignal + sigswing*rns + ron*rnb;
			currptr++;mapcnt++;
			if (mapcnt >= maxmappix){
				reset_buffer();
			}
		}
	} else if (artifdata == COLRAMP){
		for (i=0; i<(int)(double)chunk/namps; i++){
			p=i;
			for (a=0; a<namps; a++){
				*currptr = p;
                        	currptr++;mapcnt++;
                 	       if (mapcnt >= maxmappix){
					reset_buffer();
                        	}
			}
		}
	}
	return (mapcnt);
}

int artif_pattern (int fd, int type, char *args)
{
	if (type == COLRAMP){
		artifdata = COLRAMP;
                sscanf (args, "%d", &namps);
		if (namps <=0)
			namps = 1;
		printf ("artifdata is colramp namps %d\n", namps);
	}
        if (type == ARTIFDET) {
                artifdata = ARTIFDET;
                sscanf (args, "%d %d %d", &biaslevel, &siglevel, &ron);
                if (biaslevel <=0)
                        biaslevel = DEFBIASLEVEL;
                if (siglevel <=0)
                        biaslevel = DEFSIGLEVEL;
                if (ron < 0)
                        ron = DEFRON;
                printf ("artifdata is artifdet bias %d signal %d ron %d\n", biaslevel, siglevel, ron);
                return (DONE);
        }
	return (DONE);
}

void get_link_info (int fd, char *info)
{
	strcpy (info, "simulated");
}

void get_cont_info (int fd, char *info)
{
        strcpy (info, "SDSU GenIII (simulated)");
}


/**********************************************************************
 *  * Closes the driver
 *   *********************************************************************/
int close_drv (int fd)
{
        if (highmem != NULL){
                munmap ((char *)highmem, opened_mapsize);
		unlink (devname);
	}
	printf ("simulated driver closed\n");
        return (DONE);
}


/***************************************************************************
 * Opens the driver. The "device" parameter says which card (starts from 0)
 ***************************************************************************/
int open_drv(int device, unsigned short **highmemPtr, int mapsize, char *params)
{
int fd;
int result;

	sprintf (devname, "/tmp/astropci_sim%d", device);
	printf ("sim: driver opened\n");
//let's create the map
	fd = open(devname, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    	if (fd == -1) {
		perror("Error creating driver file");
		return (-errno);
	}
	result = lseek(fd, mapsize-1, SEEK_SET);
	if (result == -1) {
		close (fd);
		perror("Error expanding driver file");
		return (-errno);
	}
	result = write(fd, "", 1);
        if (result == -1) {
                close (fd);
                perror("Error writing to driver file");
                return (-errno);
        }
		
	*highmemPtr = (unsigned short *)mmap(0,mapsize, (PROT_READ | PROT_WRITE), MAP_SHARED, (int) fd, 0);
	highmem = *highmemPtr;
        if (highmem == NULL) {
                close (fd);
                return (-errno);
        }
	printf ("high_mem %ld\n", (unsigned long)highmem);
	opened_mapsize = mapsize;
	maxmappix = (int) (double)mapsize/2;
	if (strcmp (params, "NIR") == 0)
		dettype = NIR;
	else
		dettype = OPTICAL;

	return (fd);
}



/**********************************************************************
 * Resets the Leach Controller
 *********************************************************************/
int reset_ctrler (int fd)
{

	return (DONE);
}



/**********************************************************************
 * Resets the PCI card
 *********************************************************************/
int reset_pci (int fd)
{
	return (DONE);
}


//***********************************************************************
void set_hctr (int fd, int value)
{
}


//************************************************************************	
int set_hcvr (int fd, int command, int expected_reply, int timeout)
{

	return (DONE);
}


/***************************************************************************
 * Loads the pci card (DSP) with the specified ".lod" file from the host
 * This function shouldn't be very used
 ***************************************************************************/
int load_pci (int fd, char *filename)
{
	return (DONE);
}
