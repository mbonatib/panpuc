/* Library for accesng the Leach PCI driver */
#include "sdsu_defs.h"
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
static short opened=0; 
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
	if (ioctl (fd, ASTROPCI_HCVR_DATA, &data))
		return (-errno);
	return (DONE);
}


//***********************************************************************
int do_command (int fd, int board_id, int command, int arg1, int arg2, int arg3, int arg4, int expected_reply, int timeout)
{
	        int header, args;
		int ret;

		if (arg4 != NONE)
			args = 6;
		else if (arg3 != NONE)
		       args = 5;
		else if (arg2 != NONE)
			args = 4;
		else if (arg1 != NONE)
			args = 3;
		else
			args = 2;	

//		printf ("do_com: args = %d, comm=%d, arg1=%d, arg2=%d, board_id = %d\n", args, command, arg1, arg2, board_id);
		pdebug ("do_com: args = %d, comm=%d, arg1=%d, arg2=%d, board_id = %d\n", args, command, arg1, arg2, board_id);
	        cmd_data[0] = header = ((board_id << 8) | args);
	        cmd_data[1] = command;
	        cmd_data[2] = arg1;
	        cmd_data[3] = arg2;
	        cmd_data[4] = arg3;
	        cmd_data[5] = arg4;

		if (ioctl (fd, ASTROPCI_COMMAND, &cmd_data) < 0)
			return (-errno);
		ret = read_reply (fd, expected_reply, timeout);
//		return (read_reply (fd, expected_reply, timeout));
		return (ret);
}


/**********************************************************************
 * Get the status from the HCTR register
 *********************************************************************/
int get_hctr (int fd)
{
int val=0;

        if (ioctl (fd, ASTROPCI_GET_HCTR, &val))
                return (-errno);

	return (val);
}


/**********************************************************************
 * Get the status from the HSTR register
 *********************************************************************/
int get_hstr (int fd)
{
int val=0;

        if (ioctl (fd, ASTROPCI_GET_HSTR, &val))
                return (-errno);
        return (val);
}


int get_frames_progress (int fd)
{
int frames;

	if (ioctl (fd, ASTROPCI_GET_FRAMES_READ, &frames))
		return (-errno);
        printf ("frames %d\n", frames);
	return (frames);
}

/**********************************************************************
 * Get the readout progress (how many buffers has been read till now)
 *********************************************************************/
int get_read_progress (int fd)
{
int pix=0;

//	printf ("calling read progress %d\n",ASTROPCI_GET_PROGRESS);
	if (ioctl (fd, ASTROPCI_GET_PROGRESS, &pix)){
		printf ("ioctl progress error %d (pix %d)\n", -errno, pix);
		return (-errno);
	}
//	printf ("pr %d\n", pix);
	if (pix == ROUT)
		pix = -1;
	else
		if (pix == ERR)
			pix = -EIO;
	else
		if (pix == TOUT)
			pix = -ETIME;
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
        if (close(fd) != 0)
                return (-errno);
	opened=0;
	opened_mapsize = 0;
        return (DONE);
}


/***************************************************************************
 * Opens the driver. The "device" parameter says which card (starts from 0)
 ***************************************************************************/
int open_drv(int device, unsigned short **highmemPtr, int mapsize, char *params)
{
int fd;
char devname[20];
int count=0;

	printf ("openeds %d\n", opened);
	if (opened)
		return (-EBUSY);
//	sprintf (devname, "/dev/astropci%d", device);
	sprintf (devname, "/dev/Arc64PCI%d", device);
open_again:
	fd = open (devname, O_RDWR);
	if (fd == -1) {
		printf ("error openind %s (%d)\n", devname, errno);
		return (-errno);
	}
	*highmemPtr = (unsigned short*) mmap (0, mapsize, (PROT_READ | PROT_WRITE), MAP_SHARED, (int) fd, 0);
	highmem = *highmemPtr;
	if (highmem == NULL) {
		close_drv (fd);
		return (-errno);
	}
//	pdebug ("high_mem %ld\n", (unsigned long)highmem);
	printf ("high_mem %ld\n", (unsigned long)highmem);

	/*for some reason, normally the first time the driver is opened after
	 * the controller has been powered up, the PCI card fails to get the
	 * highmem, getting just 0xffffff. If this happens, closing the
	 * driver and opening it again fixes it ...(??) 
	*/
#if 0
	if ((unsigned long)highmem >= 0xffffffff) {
		if (count++ <= 3) {
			close_drv(fd);
			goto open_again;
		}
		else
			return (-ENOMEM);
	}
#endif
	opened=1;
	opened_mapsize = mapsize;
	return (fd);
}

void get_link_info (int fd, char *info)
{
        strcpy (info, "pcix");
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
int val= RESET_CONTROLLER;

	if (ioctl (fd, ASTROPCI_SET_HCVR, &val))
		return (-errno);
	cmd_data[0] = val;
	return (read_reply (fd, SYR, -1));
}



/**********************************************************************
 * Resets the PCI card
 *********************************************************************/
int reset_pci (int fd)
{
int val= PCI_PC_RESET;

        if (ioctl (fd, ASTROPCI_SET_HCVR, &val))
                return (-errno);
	cmd_data[0] = val;
        return (read_reply (fd, DON, -1));
}


//***********************************************************************
void set_hctr (int fd, int value)
{
	ioctl (fd, ASTROPCI_SET_HCTR, &value);
}


//************************************************************************	
int set_hcvr (int fd, int command, int expected_reply, int timeout)
{
int ret=0;

	if ((ret=ioctl (fd, ASTROPCI_SET_HCVR, &command)) < 0){
		pdebug ("set hcvr returned errno %d ret %d\n", errno, ret);
		return (-errno);
	}
	pdebug ("set_hcvr: ioctl returned %d\n", ret);
	cmd_data[0] = command;
	return (read_reply (fd, expected_reply, timeout));
}


/***************************************************************************
 * Loads the pci card (DSP) with the specified ".lod" file from the host
 * This function shouldn't be very used
 ***************************************************************************/
int load_pci (int fd, char *filename)
{
	char input_line[81];
	const char *words_string = " ";
	const char *addr_string = " ";
	const char *fileType = " ";
	const char *tokens[20];
	int data = 0;
	int addr = 0;
	int done_reading_data = 0;
	int reply = 0;
	int word_total = 0;
	int word_count = 0;
	int count = 0;
	int i = 0;
	FILE *inFile;

	/* Set the PCI board to slave mode. */
	ioctl(fd, ASTROPCI_GET_HCTR, &reply);

	/* Clear the HTF bits and bit 3. */
	reply = reply & HTF_CLEAR_MASK & BIT3_CLEAR_MASK;
		
	/* Set the HTF bits. */
	reply = reply | HTF_MASK;

	ioctl(fd, ASTROPCI_SET_HCTR, &reply);

	/* Inform the DSP that new pci boot code will be downloaded. */
	ioctl(fd, ASTROPCI_PCI_DOWNLOAD, 0);

	/* Set the magic value that says this is a PCI download. */
	reply = 0x00555AAA;

	ioctl(fd, ASTROPCI_HCVR_DATA, &reply);
	/* Open the file for reading. */
	if ((inFile = fopen(filename, "r")) == NULL) {
		printf("Error: Cannot open file: %s \n", filename);
		return (-errno);
	}
		
	while(!done_reading_data) {
		fgets(input_line, 80, inFile);

		if (strstr(input_line, SEARCH_STRING) != NULL) {
			/* Get the next line. */
			fgets(input_line, 80, inFile);

			/* Get the number of words and starting address. */
			words_string = strtok(input_line, " ");
			addr_string = strtok(NULL, " ");

			sscanf(words_string, "%X", &word_total);
			sscanf(addr_string, "%X", &addr);

			/* Check that the number of words is less that 4096 (0x1000). */
			if (word_total > 0x1000) {
				printf("Error: Number of words to write exceeds DSP memory range.");
				return (-EILSEQ);
			}
			else {
				ioctl(fd, ASTROPCI_HCVR_DATA, &word_total);
			}

			/* Check that the address is equal to 0. */
			if (addr != 0) {
				printf("\nError: Address not equal to zero.");
				exit(1);
			}
			else {
				ioctl(fd, ASTROPCI_HCVR_DATA, &addr);
				addr = 0;
			}

			/* Throw away the next line (example: _DATA P 000002). */
			fgets(input_line, 80, inFile);

			/* Load the data. */
			while (word_count < (word_total - 2)) {
				/* Get the next line, this is the data start. */
				fgets(input_line, 80, inFile);

				/* Check for "_DATA" strings and discard them by     */
				/* reading the next data line, which should be data. */
				if (strstr(input_line, SEARCH_STRING) != NULL)
					fgets(input_line, 80, inFile);

				count = 0;
				tokens[count] = strtok(input_line, " ");
					
				if (strstr(tokens[0], "_") != NULL) {
					done_reading_data = 1;
					fseek (inFile, -15, SEEK_CUR);
				}

				else {
					while (tokens[count] != NULL) {
						count++;
						tokens[count] = strtok(NULL, " ");
					}

					for (i=0; i<count-1; i++) {
						sscanf(tokens[i], "%X", &data);
						ioctl(fd, ASTROPCI_HCVR_DATA, &data);
						addr++;
						word_count++;
					}
				}
			}
			done_reading_data = 1;
		}

		/* Check the file validity. */
		else if (strspn(input_line, "_START") > 0) {
			fileType = strtok(input_line, " ");
			fileType = strtok(NULL, " ");

			if (strspn(fileType, "PCI") > 0)
				pdebug("PCI file ... OK.\n");
			else {
				pdebug("Error: Invalid file type.\n");
				return (-EILSEQ);
			}
		}
	}
	/* Set the PCI board data transfer format (Set HTF bits to 00). */
	ioctl(fd, ASTROPCI_GET_HCTR, &reply);
	reply = (reply & HTF_CLEAR_MASK) | 0x900;
	ioctl(fd, ASTROPCI_SET_HCTR, &reply);

	fclose(inFile);

	/* Wait for the PCI DSP to finish initialization. */
	ioctl(fd, ASTROPCI_PCI_DOWNLOAD_WAIT, &reply);

	/* Make sure a DON is received. */
	if (reply != DON) {
		printf("ERROR: Bad reply, expected: DON (0x444F4E), got: 0x%X\n", reply);
		return (reply);
	}
	return (DONE);
}

int set_size (int fd, int ncols, int nrows)
{
        return (DONE);
}

