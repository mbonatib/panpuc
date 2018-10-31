/* Library for accesng the Leach PCI driver */
#include "sdsu_defs.h"
#include "ArcDeviceCAPI.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/mman.h>

#ifdef _DEBUG_
#  define pdebug(fmt, args...) printf ("SDSU: " fmt, ## args)
#else
#  define pdebug(fmt, args...)
#endif
#define DSP_PRESCANS 0

static int cmd_data[10];
static int opened_mapsize=0;
static int opened=0;
unsigned short *highmem=NULL;

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
		int stat;
		int drep;
		int ret;

		if (arg4 != NONE)
			drep = ArcDevice_Command_IIII (board_id, command, arg1, arg2, arg3, arg4, &stat);
		else if (arg3 != NONE)
			drep = ArcDevice_Command_III (board_id, command, arg1, arg2, arg3, &stat);
		else if (arg2 != NONE)
			drep = ArcDevice_Command_II (board_id, command, arg1, arg2, &stat);
		else if (arg1 != NONE)
			drep = ArcDevice_Command_I (board_id, command, arg1, &stat);
		else
			drep = ArcDevice_Command (board_id, command, &stat);

        	if (stat != ARC_STATUS_OK)
                	return (-stat);

		cmd_data[0] = drep;
		ret = read_reply (fd, expected_reply, timeout);	
		return (ret);
}


/**********************************************************************
 * Get the status from the HCTR register
 *********************************************************************/
int get_hctr (int fd)
{
int val=0;

	return (val);
}


/**********************************************************************
 * Get the status from the HSTR register
 *********************************************************************/
int get_hstr (int fd)
{
int val=0;

        return (val);
}


int get_frames_progress (int fd)
{
int frames;
int stat;

        frames = ArcDevice_GetFrameCount(&stat);
        if (stat != ARC_STATUS_OK)
                return (-frames);

        printf ("frames %d\n", frames);
	return (frames);
}

/**********************************************************************
 * Get the readout progress (how many buffers has been read till now)
 *********************************************************************/
int get_read_progress (int fd)
{
int stat;
int pix;

	pix = ArcDevice_GetPixelCount(&stat);
        if (stat != ARC_STATUS_OK)
                return (-pix);
        return (pix);
}


int artif_pattern (int fd, int type, char *args)
{
        return (DONE);
}


int set_pixtime (int fd, double pixtime)
{
        return (DONE);
}

int get_pixtime (int fd, double *pixtime)
{
	*pixtime = -1.0;
	printf ("pixtime returning -1\n");
        return (0);
}


/**********************************************************************
 *  * Closes the driver
 *   *********************************************************************/
int close_drv (int fd)
{
        if (highmem != NULL)
                munmap ((char *)highmem, opened_mapsize);
	        pdebug ("closing driver, unmapping %d bytes\n", opened_mapsize);
	ArcDevice_Close();
//	ArcDevice_UnMapCommonBuffer();
	ArcDevice_IsOpen(&opened);
	opened_mapsize = 0;
	opened = 0;
        return (DONE);
}


/***************************************************************************
 * Opens the driver. The "device" parameter says which card (starts from 0)
 ***************************************************************************/
int open_drv(int device, unsigned short **highmemPtr, int mapsize, char *params)
{
int fd;
char devname[20];
int dStatus;
const char** pszDevList;
void *pBuf;
//	ArcDevice_IsOpen (&opened);
	printf ("openeds %d\n", opened);
	if (opened)
		return (-EBUSY);
//	sprintf (devname, "/dev/astropci%d", device);
	sprintf (devname, "/dev/Arc64PCI%d", device);
	ArcDevice_FindDevices( &dStatus );
 	printf( "done! Found %d devices!\n", ArcDevice_DeviceCount() );
        if ( dStatus == ARC_STATUS_ERROR ){
		printf( "FindDevices failed!\n" );
		return (0);
	}
	printf( "[] Reading device list ... " );
	pszDevList = ArcDevice_GetDeviceStringList(&dStatus);
	if ( dStatus == ARC_STATUS_ERROR ){
		printf( "GetDeviceStringList failed!\n" );
		return (0);
	}
	printf( "[] Opening device #%d ( %s ) ... ", device, pszDevList[device] );
	ArcDevice_Open_I( device, mapsize, &dStatus ); 
	ArcDevice_FreeDeviceStringList();
	if ( dStatus == ARC_STATUS_ERROR ){
		printf( "Open failed!\n" );
		return (0);
	}
	pBuf = ArcDevice_CommonBufferVA( &dStatus );
	if ( dStatus == ARC_STATUS_OK ){
		printf( "\tBuffer VA ..... 0x%lX\n", ( unsigned long )pBuf );
		printf( "\tBuffer PA ..... 0x%lX\n", ArcDevice_CommonBufferPA( &dStatus ) );
		printf( "\tBuffer Size ... %d bytes\n", ArcDevice_CommonBufferSize( &dStatus ) );
	} else {
		printf ("Buffer init failed\n");
		return (0);
	}

	*highmemPtr = pBuf;
	highmem = *highmemPtr;
	if (highmem == NULL) {
		fd = 0;
		close_drv (fd);
		return (-errno);
	}
//	pdebug ("high_mem %ld\n", (unsigned long)highmem);
	printf ("high_mem %ld\n", (unsigned long)highmem);

//	ArcDevice_IsOpen (&opened);
	opened = 1;
	if (opened)
		fd = 250;
	return (fd);
}

void get_link_info (int fd, char *info)
{
        strcpy (info, "pcie");
}

void get_cont_info (int fd, char *info)
{
        strcpy (info, "SDSU GenIII");

}

/**********************************************************************
 * Resets the Leach Controller
 *********************************************************************/
int reset_ctrler (int fd)
{
int stat;

        ArcDevice_ResetController (&stat);
        if (stat != ARC_STATUS_OK)
                return (-stat);
        return (stat);
}



/**********************************************************************
 * Resets the PCI card
 *********************************************************************/
int reset_pci (int fd)
{
int stat;

	ArcDevice_Reset (&stat);
        if (stat != ARC_STATUS_OK)
                return (-stat);
        return (stat);
}


//***********************************************************************
void set_hctr (int fd, int value)
{
}


//************************************************************************	
int set_hcvr (int fd, int command, int expected_reply, int timeout)
{
int ret=0;

	return (ret);
}


/***************************************************************************
 * Loads the pci card (DSP) with the specified ".lod" file from the host
 * This function shouldn't be very used
 ***************************************************************************/
int load_pci (int fd, char *filename)
{
int stat=0;

	ArcDevice_LoadDeviceFile(filename, &stat);
        if (stat != ARC_STATUS_OK)
                return (-stat);

	return (DONE);
}

int set_size (int fd, int ncols, int nrows)
{
        return (DONE);
}

