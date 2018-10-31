/* Library for accesng the Leach PCI driver */
#include "sdsu_proto.h"
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
#include <dlfcn.h>

#ifdef _DEBUG_
#  define pdebug(fmt, args...) printf ("SDSU: " fmt, ## args)
#else
#  define pdebug(fmt, args...)
#endif
#define DSP_PRESCANS 0

void *FunctionLib;
int (*dhe_open_p)();
int (*dhe_close_p) ();
int (*dhe_read_reply_p)();
int (*dhe_get_progress_p)();
int (*dhe_get_frame_progress_p)();
int (*dhe_get_hctr_p)();
int (*dhe_get_hstr_p)();
int (*dhe_set_cmd_data_p)();
int (*dhe_read_reply_p)();
int (*dhe_do_command_p)();
int (*dhe_set_hctr_p)();
int (*dhe_set_hcvr_p)();
int (*dhe_reset_controller_p)();
int (*dhe_reset_pci_p)();
int (*dhe_load_pci_p)();
int (*dhe_set_size_p)();
int (*dhe_get_link_info_p)();
int (*dhe_get_cont_info_p)();
int (*dhe_set_pixtime_p)();
int (*dhe_get_pixtime_p)();
int (*dhe_artif_data_p)();



char opars[60];

const char *dlError;
static int master_ID = TIM_ID;
static int board_dest = TIM_ID;
static int tot_buf_size = 512 * 1024 * 1024; /* 120 MB default */
//static unsigned int tot_buf_size = 2 * 4200 * 4200; /* ?? */
static short opened=0; 
unsigned short *highmem=NULL;
int gbl_overscan=0;
int gbl_prescan=0;
int gbl_xdata=0;
int gbl_ydata=0;
int gbl_ncols=0;
int gbl_nrows=0;
int gbl_speed=-1;
int gbl_gain=-1;
int ncmd=0;
cmd_t   cmdArr[MAX_CMDS];

int SDSU_checkError (int iferr)
{
        dlError = dlerror ();
        if(dlError) {
                printf ("%s\n", dlError);
                return (-iferr);
        }
        return (DONE);
}


void SDSU_GetDlErr (char *err)
{
	strcpy (err, dlError);
}

int SDSU_InitLib (char *filepath)
{
int ret;

	pdebug ("libpath %s\n", filepath);
	printf ("hi opening\n");
	FunctionLib = dlopen (filepath, RTLD_LAZY);
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);
        dhe_open_p = dlsym (FunctionLib, "open_drv");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);
	printf ("hi 1\n");

        dhe_close_p = dlsym (FunctionLib, "close_drv");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_read_reply_p = dlsym (FunctionLib, "read_reply");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_get_progress_p = dlsym (FunctionLib, "get_read_progress");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);
	printf ("hi 2\n");

        dhe_get_frame_progress_p = dlsym (FunctionLib, "get_frames_progress");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_get_hctr_p = dlsym (FunctionLib, "get_hctr");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_get_hstr_p = dlsym (FunctionLib, "get_hstr");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_set_cmd_data_p = dlsym (FunctionLib, "set_cmd_data");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_do_command_p = dlsym (FunctionLib, "do_command");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_set_hcvr_p = dlsym (FunctionLib, "set_hcvr");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_reset_controller_p = dlsym (FunctionLib, "reset_ctrler");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_reset_pci_p = dlsym (FunctionLib, "reset_pci");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_load_pci_p = dlsym (FunctionLib, "load_pci");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_set_size_p = dlsym (FunctionLib, "set_size");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_get_link_info_p = dlsym (FunctionLib, "get_link_info");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_get_cont_info_p = dlsym (FunctionLib, "get_cont_info");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_set_pixtime_p = dlsym (FunctionLib, "set_pixtime");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_get_pixtime_p = dlsym (FunctionLib, "get_pixtime");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

        dhe_artif_data_p = dlsym (FunctionLib, "artif_pattern");
        if ((ret = SDSU_checkError (ELIBACC)))
                return (ret);

	return (DONE);
}

void SDSU_clean_table (void)
{
	ncmd = 0;
}

int SDSU_add_tab_entry (char *command, char *type, char *dsp, char *memtype, int address, int bit)
{
int etype=0;

        if ((command == (char *)NULL) || (dsp == (char *)NULL))
                return (-EINVAL);

        strcpy (cmdArr[ncmd].command, command);
	if (strcmp (type, "_add_") == 0)
		etype = ADDRESS;
	else if (strcmp (type, "_bit_") == 0)
		etype = BIT;
	else etype = DSPCMD;

	if (memtype != NULL) {
		if (memtype[0] == 'Y')
        		cmdArr[ncmd].memtype = Y_MEM;
		else if (memtype[0] == 'X')
        		cmdArr[ncmd].memtype = X_MEM;
		else if (memtype[0] == 'P')
        		cmdArr[ncmd].memtype = P_MEM;
		else if (memtype[0] == 'R')
        		cmdArr[ncmd].memtype = R_MEM;
		else
			cmdArr[ncmd].memtype = NONE;
	} else
		cmdArr[ncmd].memtype = NONE;
			
        strcpy (cmdArr[ncmd].dsp, dsp);
        cmdArr[ncmd].type = etype;
        cmdArr[ncmd].address = address;
        cmdArr[ncmd].bit = bit;
	if (cmdArr[ncmd].type == DSPCMD)
        	printf ("add_entry: dspcmd %s %s\n",cmdArr[ncmd].command, dsp);
	else if (cmdArr[ncmd].type == ADDRESS)
        	printf ("add_entry: address %s %s %s(%d) %d\n",cmdArr[ncmd].command, dsp, memtype, cmdArr[ncmd].memtype, address);
	else if (cmdArr[ncmd].type == BIT)
        	printf ("add_entry: bit %s %s %s(%d) %d %d\n",cmdArr[ncmd].command,  dsp, memtype, cmdArr[ncmd].memtype, address, bit);
        ncmd++;
        return (DONE);
}

int SDSU_get_board_from_tab (char *board)
{
int ret = TIM_ID;

	if (strcmp (board, "util") == 0)
		ret = UTIL_ID;
	else if (strcmp (board, "pci") == 0)
		ret = PCI_ID;

	return (ret);
}

int SDSU_get_intcmd (char *cmd)
{
int val, i;
char num;

	if (cmd == NULL)
		return (NOT_IN_TABLE);
	if (strlen(cmd) != 3)
		return (NOT_IN_TABLE);

	val = 0;
	for (i=0; i<3; i++){
		num = cmd[i];
		val = num | (val << 8);
	}  
	return (val);
}	


int SDSU_get_tab_dspcmd (char *command, char *dspcmd)
{
int i;

        for (i=0; i<ncmd; i++){
                if (strcmp (cmdArr[i].command, command) == 0){
			/*does this entry  corresponds to a dsp command?*/
			if (cmdArr[i].type == DSPCMD){
				if (dspcmd != NULL) /*provide the ascii version too*/
					strcpy (dspcmd, cmdArr[i].dsp);
				/*return the numeric value of the dsp cmd*/
				return (SDSU_get_intcmd (cmdArr[i].dsp));
			} 
		}
        }
        return (NOT_IN_TABLE);
}

int SDSU_get_tab_address (char *command, int *board, int *memtype)
{
int i;

        for (i=0; i<ncmd; i++){
                if (strcmp (cmdArr[i].command, command) == 0){
                        /*does this entry  corresponds to a dsp command?*/
                        if (cmdArr[i].type == ADDRESS){
				*board = SDSU_get_board_from_tab (cmdArr[i].dsp);
				*memtype = cmdArr[i].memtype;
                                return (cmdArr[i].address);
                        }
                }
        }
        return (NOT_IN_TABLE);
}
	
int SDSU_get_tab_bit (char *command, int *board, int *memtype, int *address)
{
int i;

        for (i=0; i<ncmd; i++){
                if (strcmp (cmdArr[i].command, command) == 0){
                        /*does this entry  corresponds to a dsp command?*/
                        if (cmdArr[i].type == BIT){
                                *board = SDSU_get_board_from_tab (cmdArr[i].dsp);
                                *memtype = cmdArr[i].memtype;
                                *address = cmdArr[i].address;
                                return (cmdArr[i].bit);
                        }
                }
        }
        return (NOT_IN_TABLE);
}


int set_mapsize (int size)
{
	if (size <= 0)
		return (-EINVAL);
	
	tot_buf_size = size * 1024 * 1024;
	printf ("mapping size is now %d MB. close and init to take effect\n", tot_buf_size);
	printf ("Be sure the <mapsize> key in the config file (under [Device] entry) is not present or is <=0, or the value ");
	printf ("you just set will be overriden\n");
	return (DONE);
}

int get_mapsize (void)
{
	return ((int) ((float)tot_buf_size / (1024*1024)));
}

/*************************************************************************
 * Function for reading the reply from the Leach Controller (to a command)
 *************************************************************************/
int read_reply (int fd, int expected, int timeout)
{
	return ((*dhe_read_reply_p)(fd, expected, timeout));		
}


int set_cmd_data (int fd, int data)
{
	return ((*dhe_set_cmd_data_p)(fd, data));
}


//***********************************************************************
int do_command (int fd, int board_id, int command, int arg1, int arg2, int arg3, int arg4, int expected_reply, int timeout)
{
	return ((*dhe_do_command_p)(fd, board_id, command, arg1, arg2, arg3, arg4, expected_reply, timeout));
}


/**********************************************************************
 * Get the status from the HCTR register
 *********************************************************************/
int get_hctr (int fd)
{
	return ((*dhe_get_hctr_p)(fd));
}


/**********************************************************************
 * Get the status from the HSTR register
 *********************************************************************/
int get_hstr (int fd)
{
	return ((*dhe_get_hstr_p)(fd));
}


int get_frames_progress (int fd)
{
	return ((*dhe_get_frame_progress_p)(fd));
}

/**********************************************************************
 * Get the readout progress (how many buffers has been read till now)
 *********************************************************************/
int get_read_progress (int fd)
{
	return ((*dhe_get_progress_p)(fd));
}


/**********************************************************************
 *  * Closes the driver
 *   *********************************************************************/
int close_drv (int fd)
{
int ret;

	pdebug ("cloding driver\n");

        if ((*dhe_close_p) == NULL)
                return (-ELIBACC);

        if ((ret = (*dhe_close_p)(fd)) < 0){
		pdebug ("close returned %d\n", ret);
                return (ret);
	}

	opened = 0;
	pdebug ("driver closed\n");
	
        return (DONE);
}

void set_openparams (char *params)
{
	strcpy (opars, params);
}	
/***************************************************************************
 * Opens the driver. The "device" parameter says which card (starts from 0)
 ***************************************************************************/
int open_drv(int device, unsigned short **highmemPtr)
{
int fd;

        if ((*dhe_open_p) == NULL)
                return (-ELIBACC);

	if (opened)
		return (-EISCONN);

        pdebug ("sdsuLib: opening driver ...\n");
        if ((fd = (*dhe_open_p)(device, highmemPtr, tot_buf_size, opars)) < 0){
                return (fd);
        }
	opened=1;
        pdebug ("sdsuLib: driver opened (%d, fd %d) ...\n", opened, fd);
	return (fd);
}



/**********************************************************************
 * Resets the Leach Controller
 *********************************************************************/
int reset_ctrler (int fd)
{
	return ((*dhe_reset_controller_p)(fd));
}



/**********************************************************************
 * Resets the PCI card
 *********************************************************************/
int reset_pci (int fd)
{
	return ((*dhe_reset_pci_p)(fd));
}


//************************************************************************
int get_masterID (void)
{
	return (master_ID);
}

void set_masterID (int id)
{
	master_ID = id;
}


/***********************************************************************/
int set_destination (int fd, int num_of_args,  int bd)
{
	board_dest = bd;
	return (DONE);
}


//***********************************************************************
void set_hctr (int fd, int value)
{
	(*dhe_set_hctr_p)(fd);
}


//************************************************************************	
int set_hcvr (int fd, int command, int expected_reply, int timeout)
{
	return ((*dhe_set_hcvr_p)(fd, command, expected_reply, timeout));
}


//********************************************************************
int set_size (int fd, unsigned int ncols, unsigned int nrows)
{
int ret=DONE;
int add, board, memtype;
unsigned int imsize;

	imsize = ncols * nrows * 2;
	if (imsize >= tot_buf_size){
		printf ("ERROR image (%d MB) is bigger than current mapped buffer in kernel (%d MB). Please run the command 'set mapsize', then close and init, then resize the image\n", imsize/(1024*1024), tot_buf_size/(1024*1024));
		return (-ENOMEM);
	}

        if ((add =  SDSU_get_tab_address (COLSLOC, &board, &memtype)) == NOT_IN_TABLE){
                add = 0x01;
                board = TIM_ID;
                memtype = Y_MEM;
        }
	if ((ret=do_command (fd, board, WRM, (memtype | add), ncols, NONE, NONE ,DON, DEF_TIMEOUT)) < 0)
		return (ret);

        if ((add =  SDSU_get_tab_address (ROWSLOC, &board, &memtype)) == NOT_IN_TABLE){
                add = 0x02;
                board = TIM_ID;
                memtype = Y_MEM;
        }
	ret = do_command (fd, board, WRM, (memtype | add), nrows, NONE, NONE ,DON, DEF_TIMEOUT);
#if 0
	/*if all is OK, ask the DSP to calc. the per-amp. size (for waveforms)*/
	if (ret>=0)
        	manual_command (fd, "", "", "", "", "", "SAM", TIM_ID);
#endif
	gbl_ncols = ncols;
	gbl_nrows = nrows;
	(*dhe_set_size_p)(fd, ncols, nrows);
	return (ret);
}


//**********************************************************************
int get_controller_info (int fd)
{
int info=0;
	info = do_command (fd, TIM_ID, RCC, NONE, NONE, NONE, NONE, 0, DEF_TIMEOUT);

	return (info);
}

void get_cont_type (int fd, char *strinfo)
{
	(*dhe_get_cont_info_p)(fd, strinfo);
}	


int get_info (int fd, unsigned short *info)  /*by now, return 6 16-bit values*/
{

	return (get_controller_info (fd));
}

void get_link_info (int fd, char *info) 
{
	(*dhe_get_link_info_p)(fd, info);
}




/***********************************************************************/
int get_cont_status (int fd)
{
	return ((get_hstr(fd) & HTF_BITS) >> 3);
}
	

//************************************************************************
int power (int fd, short flag)
{
int command;

	if (flag)
		command = PON;
	else
		command = POF;

	return (do_command (fd, get_masterID(), command, NONE, NONE, NONE, NONE, DON, DEF_TIMEOUT));
}


/*************************************************************************/
int move_shutter (int fd, short flag)
{
int command;

	if (flag)
		command = OSH;
	else
		command = CSH;

	return (do_command (fd, get_masterID(), command, NONE, NONE, NONE, NONE, DON, DEF_TIMEOUT));
}


//**************************************************************************
int abort_exposure (int fd)
{
	printf ("sending AEX\n");
	return (do_command (fd, get_masterID(), AEX, NONE, NONE, NONE, NONE, DON, DEF_TIMEOUT));
}


//*************************************************************************
int pause_exp (int fd)
{
int ret;

	ret = do_command (fd, get_masterID(), PEX, NONE, NONE, NONE, NONE, DON, DEF_TIMEOUT);
	return (ret);
}


//***********************************************************************
int resume_exp (int fd)
{
int ret;
	 ret = do_command (fd, get_masterID(), REX, NONE, NONE, NONE, NONE, DON,DEF_TIMEOUT);
	 return (ret);
}


/************************************************************************/
int abort_readout (int fd)
{
	return (set_hcvr (fd, ABORT_READOUT, DON, DEF_TIMEOUT));
}


/**********************************************************************
 * Aborts the current image (prev and post expose frames)
 *********************************************************************/
int abort_current_image (int fd)
{
	pdebug ("abort_current_image\n");
	return (abort_readout (fd));
}

int set_guider_nimages (int fd, int val)
{
char gec[5];
char arg1[6];

  if (SDSU_get_tab_dspcmd (GNIM, gec) == NOT_IN_TABLE)
         strcpy (gec, DEFGNIM);

  sprintf (arg1, "%d", val);
  return (manual_command (fd, arg1, "", "", "", "", gec, TIM_ID));
}


int set_window_mode (int fd, int mode, int cols, int rows)
{
char wm[5];
char arg1[5];
char arg2[5];
int ret;

  if (SDSU_get_tab_dspcmd (GMODE, wm) == NOT_IN_TABLE)
         strcpy (wm, DEFGMODE);
  if (mode <= 0)
	mode = 0;
  else
	mode = 1;
  sprintf (arg1, "%d", mode);
  if ((ret = manual_command (fd, arg1, "", "", "", "", wm, TIM_ID)) < 0)
	return (ret);

  if (SDSU_get_tab_dspcmd (GLEN, wm) == NOT_IN_TABLE)
         strcpy (wm, DEFGLEN);
  sprintf (arg1, "%d", cols);
  sprintf (arg2, "%d", rows);
  return (manual_command (fd, arg1, arg2, "", "", "", wm, TIM_ID));
}


int set_guider_exptime (int fd, int val)
{
char gec[5];
char arg1[6];

  if (SDSU_get_tab_dspcmd (GEXPT, gec) == NOT_IN_TABLE)
         strcpy (gec, DEFGEXPT);

  sprintf (arg1, "%d", val);
  printf ("sending %s %s\n", gec, arg1);
  return (manual_command (fd, arg1, "", "", "", "", gec, TIM_ID));
}

/********************************************************************/
int set_exposure_time (int fd, unsigned int val)
{
int m_id, ret;
	
	m_id = get_masterID();
	if (m_id == TIM_ID)
		ret = do_command (fd, m_id, SET, val, NONE, NONE, NONE, DON,DEF_TIMEOUT);
	else
		ret = do_command (fd, m_id, WRM, (Y_MEM | 0x18), val, NONE, NONE, DON, DEF_TIMEOUT);

	return (ret);
}	


/**************************************************************************/
int start_exposure (int fd)
{
	return (do_command (fd, get_masterID(), SEX, NONE, NONE, NONE, NONE, DON,DEF_TIMEOUT));
}


/**************************************************************************/
int read_exposure_time (fd)
{
int m_id;
int ret;
int cmd;

	m_id = get_masterID ();
	if (m_id == TIM_ID) {
//		if ((cmd = SDSU_get_tab_dspcmd (ELAPSEDTIMECMD, NULL)) == NOT_IN_TABLE)
			cmd = RET;
		ret = do_command (fd, m_id, cmd, NONE, NONE, NONE, NONE, 0,DEF_TIMEOUT);
//		if (ret == ROUT)
//			ret = -TOUT ;
		if (ret == DON)
			ret = 0;					
		return (ret);
	} else
		return (do_command (fd, m_id, RDM, (Y_MEM | 0x17), NONE, NONE, NONE, 0 ,DEF_TIMEOUT));
}

/**************************************************************************/
int read_frame_time (fd)
{
char rft[5];

	if (SDSU_get_tab_dspcmd (FRAMETIMECMD, rft) == NOT_IN_TABLE)
		strcpy (rft, DEFFRAMETIMECMD);
        return (manual_command (fd, "", "", "", "", "", rft, TIM_ID));
}

/**************************************************************************/
int read_actual_inttime (fd)
{
char itt[5];

	if (SDSU_get_tab_dspcmd (INTEGTIMECMD, itt) == NOT_IN_TABLE)
		strcpy (itt, DEFINTEGTIMECMD);
        return (manual_command (fd, "", "", "", "", "", itt, TIM_ID));
}


/*************************************************************************/
int test_data_link (int fd, short dest, int data)
{

	if (data > 0xFFFFFF)    /*24 bits maximum */
	                return (-EINVAL);

	 return (do_command (fd, dest, TDL, data, data, NONE, NONE, 0 ,DEF_TIMEOUT));
}


/**************************************************************************/
int read_memory (int fd, char *type, int address)
{
int num_type;

	if (!strcmp (type, "Y"))
		num_type = Y_MEM;
	else if (!strcmp (type, "X"))
			num_type = X_MEM;
	else if (!strcmp (type, "P"))
			num_type = P_MEM;
	else if (!strcmp (type, "R"))
			num_type = R_MEM;
	else
		return (-EINVAL);

	return (do_command (fd, board_dest, RDM, (num_type | address), NONE, NONE, NONE, 0, DEF_TIMEOUT));

}

/**************************************************************************/

int write_memory (int fd, char *type, int address, int value)
{
int num_type;

        if (!strcmp (type, "Y"))
                num_type = Y_MEM;
        else if (!strcmp (type, "X"))
                num_type = X_MEM;
        else if (!strcmp (type, "P"))
                num_type = P_MEM;
        else if (!strcmp (type, "R"))
                num_type = R_MEM;
        else
               return (-EINVAL);
//	printf ("write_memory: board %d, mem %d, add %d value %d\n", board_dest,num_type, address, value);
	return (do_command (fd, board_dest, WRM, (num_type | address), value, NONE, NONE, DON, DEF_TIMEOUT));
}


/***************************************************************************
 * The autoshutter mode indicates if the controller is going to open the
 * shutter automatically when the exposure starts. If sets to "no", then
 * the caller needs to take care of sending a "shutter open" command manually
 ***************************************************************************/
int autoshutter (int fd, int flag)
{
int ret, add, m_id, val;
unsigned int status;

        m_id = get_masterID();
        if (m_id == TIM_ID)
                add = 0;
        else
                add = 1;

        set_destination (fd, 0, m_id);
        if ((status = read_memory (fd, "X", add)) < 0)
                return (status);

        if (m_id == TIM_ID){
                if (flag)
                        val = (status | _OPEN_SHUTTER_);
                else
                        val = (status & _CLOSE_SHUTTER_);
        } else {
                if (flag)
                        val = (status | 0x00000001);
                else
                        val = (status & 0xFFFFFFFE);
        }

        if ((ret = write_memory (fd, "X", 0, val)) < 0)
                return (ret);

        pdebug ("autotter: board %d address %d value 0x%x\n", m_id, add, val);
        return (DONE);
}

/***************************************************************************
 * The autoclear mode indicates if the controller is going to clear the
 * array automatically when the exposure starts. If sets to "no", then
 * the caller needs to take care of sending a "clear array" command manually
 ***************************************************************************/
int autoclear (int fd, int flag)
{
int ret, add, m_id, val;
unsigned int status;

        m_id = get_masterID();
        if (m_id == TIM_ID)
                add = 0;
        else
                add = 1;

        set_destination (fd, 0, m_id);
        if ((status = read_memory (fd, "X", add)) < 0)
                return (status);

        if (m_id == TIM_ID){
                if (flag)
                        val = (status & _AUTO_CLR_ON_);
                else
                        val = (status | _AUTO_CLR_OFF_);
        } else {
                if (flag)
                        val = (status | 0x00000001);
                else
                        val = (status & 0xFFFFFFFE);
        }

        if ((ret = write_memory (fd, "X", 0, val)) < 0)
                return (ret);

        pdebug ("autoclr: board %d address %d value 0x%x\n", m_id, add, val);
        return (DONE);
}



/********************************************************************************/
int load_application (int fd, int app, short dest)
{
	return (do_command (fd, dest, LDA, app, NONE, NONE, NONE, DON, DEF_TIMEOUT));
}


/*********************************************************************************/
int load_file (int fd, char *filename, short board_id)
{
	int ret=DONE;
	const char * input_value = " ";
	char input_line[81];
	const char * type_string = "__";
	const char * addr_string = " ";
	const char * file_type = " ";
	int end_of_file = 0;
	int done_reading_data = 0;
	int type = 0;
	int addr = 0;
	int data = 0;
	FILE *inFile;
	int count = 0;
	int i = 0;
	const char *tokens[20];
	
	if (board_id == PCI_ID) 
		return (load_pci (fd, filename));

	printf ("loading file %s to %d\n", filename, board_id);
	/*---------------------------
	  Open the file for reading.
	  ---------------------------*/
	if ((inFile = fopen(filename, "r")) == NULL)
	{
		printf ("ERROR: Cannot open file: %s \n", filename);
		return (-errno);
	}
	/*Check file*/
	fgets(input_line, 80, inFile);
	if (strlen (input_line) < 6)
		goto error;

	input_value = strtok(input_line, " ");
	if (strcmp(input_value, "_START") == 0) {
		file_type = strtok(NULL, " ");
		if ((strspn(file_type, "TIM") > 0) && (board_id == TIM_ID)) 
				goto ok_continue;

		if ((strspn(file_type, "UTIL") > 0) && (board_id == UTIL_ID)) 
				goto ok_continue;	
	}
error:
	printf("ERROR: Unexpected file type (%s %d) ", file_type, board_id);
	fclose (inFile);
	return (-EILSEQ);

ok_continue:
	/*--------------------------------------
	  Read in the file one line at a time.
	  --------------------------------------*/
	while (end_of_file == 0)
	{
		fgets(input_line, 80, inFile);
		input_value = strtok(input_line, " ");
		/* Check for the start of valid data */
		if (strstr(input_value, "DATA") != NULL) {
			type_string = strtok(NULL, " ");
			addr_string = strtok(NULL, " ");

			/* Convert the "address" string to the correct data type */
			sscanf(addr_string, "%X", &addr);

			/* A valid start address must be less than MAX_DSP_START_LOAD_ADDR */
			if (addr < MAX_LOAD_ADDR) {
				/* Set the "type" string to the correct ASCII sequence */
				if (strcmp(type_string, "X") == 0)
					type = X;
				else if (strcmp(type_string, "Y") == 0)
					type = Y;
				else if (strcmp(type_string, "P") == 0)
					type = P;
				else if (strcmp(type_string, "R") == 0)
					type = R;

				/* Read in the data block */
				while (!done_reading_data)
				{
					fgets(input_line, 80, inFile);
					count = 0;
					tokens[count] = strtok(input_line, " ");
						
					if (strstr(tokens[0], "_") != NULL)
					{
						done_reading_data = 1;
						fseek (inFile, -15, SEEK_CUR);
					}
						
					else
					{
						while (tokens[count] != NULL)
						{
							count++;
							tokens[count] = 
							     strtok(NULL, " ");
						}

						for (i=0; i<count-1; i++) 
						{
							sscanf(tokens[i], "%X", &data);

							if ((ret=do_command(fd, board_id, WRM, (type | addr), data, NONE, NONE, DON, DEF_TIMEOUT)) <0) {
								printf("\nERROR: Could not do write memory command (%d).\n", ret);
								return (ret);
							}
							
							addr++;
						}
					}
				}
			}
		}
	
		/* Check the file validity and set the board destination */
		if (strcmp(input_value, "_START") == 0) {
			file_type = strtok(NULL, " ");

			if ((strspn(file_type, "TIM") > 0) && (board_id == TIM_ID)) 
					goto label_ok;

			if ((strspn(file_type, "UTIL") > 0) && (board_id == UTIL_ID)) 
						goto label_ok;	

			printf("ERROR: Unexpected file type (%s %d) ", file_type, board_id);
			return (-EILSEQ);
		}
		/* Check for the end of file */
		else if (strcmp(input_value, "_END") == 0)
			end_of_file = 1;

		/* Check for the end of file */
		else if (strcmp(input_value, "_SYMBOL") == 0)
			end_of_file = 1;
label_ok:
		/* Re-initialize variables */
		done_reading_data = 0;
		type_string = "__";
		input_value = NULL;
	}

	/* Close the file */
	fclose(inFile);
	pdebug ("%s file loaded OK\n", filename);
	return (DONE);
}


/***************************************************************************
 * Loads the pci card (DSP) with the specified ".lod" file from the host
 * This function shouldn't be very used
 ***************************************************************************/
int load_pci (int fd, char *filename)
{
	return ((*dhe_load_pci_p)(fd, filename));
}

/***************************************************************************
 * Send low level commands to the controller (like setting muxes, dacs, etc)
 ***************************************************************************/
int manual_command (int fd, char *arg1, char *arg2, char *arg3, char *arg4, char *arg5, char *command, short board_dest)
{
short i, base, len, num_of_args=0;
int ret=0, num_arg1=-1, num_arg2=-1, num_arg3=-1, num_arg4=-1;
int num_command=0;
char invalidstr[10];
char *invalid;

	invalid = &invalidstr[0];
	errno = 0;
	len = strlen(arg1);
	if (len != 0){
		base = 10;
		if (len > 1)
		   if ((arg1[0] == '0') && (arg1[1] == 'x'))
			base = 16;
		if ((num_arg1=strtol (arg1, &invalid, base)) == 0)
			if (*invalid != '\0')
				for (i=0; i<len; i++)
					num_arg1 = (num_arg1 << 8) | (unsigned short)arg1[i];
		num_of_args++;
	}

	len = strlen (arg2);
        if (len != 0){
		base = 10;
		if (len > 1)
		   if ((arg2[0] == '0') && (arg2[1] == 'x'))                                                base = 16; 
                if ((num_arg2=strtol (arg2, &invalid, base)) == 0)
			if (*invalid != '\0') 
                        	for (i=0; i<len; i++)
                                	num_arg2 = (num_arg2 << 8) | (unsigned short)arg2[i];
                num_of_args++;
         }

	len = strlen(arg3);
        if (len != 0){
		base = 10;
		if (len > 1)
                   if ((arg3[0] == '0') && (arg3[1] == 'x'))                                                base = 16;  
                if ((num_arg3=strtol (arg3, &invalid, base)) == 0)
			if (*invalid != '\0') 
                        	for (i=0; i<len; i++)
                                	num_arg3 = (num_arg3 << 8) | (unsigned short)arg3[i];
                num_of_args++;
	}
	
	len = strlen (arg4);
        if (len != 0){
		base = 10;
		if (len > 1)
                   if ((arg4[0] == '0') && (arg4[1] == 'x'))
                         base = 16;
                if ((num_arg4=strtol (arg4, &invalid, base)) == 0)
			if (*invalid != '\0') 
                        	for (i=0; i<len; i++)
                                	num_arg4 = (num_arg4 << 8) | (unsigned short)arg4[i];
                num_of_args++;
        }


	for (i=0; i<3; i++){
		if (((unsigned short)command[i] < 0x41) || ((unsigned short)command[i] > 0x5A))
			return (-EINVAL);
		num_command = (num_command << 8) | (unsigned short)command[i];
	}

	printf ("command 0x%x arg1 %d arg2 %d arg3 %d arg4 %d\n", num_command, num_arg1, num_arg2, num_arg3, num_arg4);
	ret = do_command (fd, board_dest, num_command, num_arg1, num_arg2, num_arg3, num_arg4, 0, DEF_TIMEOUT);
        printf ("cmd ret %d\n", ret);
	return (ret);

}


/***************************************************************************/
int set_num_coadds (int fd, int coadds)
{
char arg1[4];
char cmd[4];

        if (SDSU_get_tab_dspcmd (COADDSCMD, cmd) == NOT_IN_TABLE)
                strcpy (cmd, DEFCOADDSCMD);
	
        sprintf (arg1, "%d", coadds);
        return (manual_command (fd, arg1, "", "", "", "", cmd, TIM_ID));
}

int set_num_reads (int fd, int reads)
{
char arg1[4];
char cmd[4];

        if (SDSU_get_tab_dspcmd (NREADSCMD, cmd) == NOT_IN_TABLE)
                strcpy (cmd, DEFNREADSCMD);
	
        sprintf (arg1, "%d", reads);
	return (manual_command (fd, arg1, "", "", "", "", cmd, TIM_ID));
}

int abort_sur (int fd)
{
char cmd[4];

        if (SDSU_get_tab_dspcmd (ABSCMD, cmd) == NOT_IN_TABLE)
                strcpy (cmd, DEFABSCMD);

        return (manual_command (fd, "", "", "", "", "", cmd, TIM_ID));
}


int set_num_sur (int fd, int sur)
{
char arg1[4];
char cmd[4];

	if (sur <= 0)
		return (abort_sur(fd));

        if (SDSU_get_tab_dspcmd (SURCMD, cmd) == NOT_IN_TABLE)
                strcpy (cmd, DEFSURCMD);

        sprintf (arg1, "%d", sur);
	return (manual_command (fd, arg1, "", "", "", "", cmd, TIM_ID));
}



int set_fpb (int fd, int fpb)
{
char arg1[4];
int ret=0;
char cmd[4];

        if (SDSU_get_tab_dspcmd (FRAMESPERBUFCMD, cmd) == NOT_IN_TABLE)
                strcpy (cmd, DEFFRAMESPERBUFCMD);

        sprintf (arg1, "%d", fpb);
	if ((ret=manual_command (fd, arg1, "", "", "", "", cmd, TIM_ID))<0)
		return (ret);
	return (DONE);
}




/***************************************************************************
 * Sets gain and speed values
 ***************************************************************************/
int set_gain_and_speed (int fd, int gain, int speed)
{
char arg1[4], arg2[3], arg3[3];
char cmd[4];
int ret;
int bnum = 0;

        if (SDSU_get_tab_dspcmd (GAINSPEEDCMD, cmd) == NOT_IN_TABLE)
                strcpy (cmd, DEFGAINSPEEDCMD);


	sprintf (arg1, "%d", bnum);
	sprintf (arg2, "%d", gain);
	sprintf (arg3, "%d", speed);
	ret = manual_command (fd, arg1, arg2, "8", "", "", cmd, TIM_ID);
	ret = manual_command (fd, arg1, arg2, arg3, "", "", cmd, TIM_ID);
//	usleep (50000);
//	ret = manual_command (fd, arg1, arg2, arg3, "", "", cmd, TIM_ID);
	printf ("ret %d\n", ret);
	if (ret == DON){
		gbl_gain = gain;
		gbl_speed = speed;
	}
	printf ("%d: speed %d, gain %d\n", bnum, gbl_speed, gbl_gain);
	return (ret);
}

void get_gain_and_speed (int fd, int *gain, int *speed)
{
	*gain = gbl_gain;
	*speed = gbl_speed;
}

int set_pixtime (int fd, double pixtime)
{
	printf ("sdsu %g\n", pixtime);
	return ((*dhe_set_pixtime_p)(fd, pixtime));
//	return (DONE);
}

float get_pixtime (int fd)
{
double pixtime=0;

        (*dhe_get_pixtime_p)(fd, &pixtime);
        return ((float)pixtime);
}

/***************************************************************************
 * Resumes the Idle Mode for the CCD
 ***************************************************************************/
int res_idl (int fd)
{
	return (do_command (fd, TIM_ID, IDL, NONE, NONE, NONE, NONE, DON, DEF_TIMEOUT));
}

/***************************************************************************
 * Stops the Idle Mode for the CCD
 ***************************************************************************/
int stp_idl (int fd)
{
	return (do_command (fd, TIM_ID, STP, NONE, NONE, NONE, NONE, DON, DEF_TIMEOUT));
}


int clear_array (fd)
{
	return (do_command (fd, TIM_ID, CLR, NONE, NONE, NONE, NONE, DON, DEF_TIMEOUT));
}


/***************************************************************************
 * Sets the temperature setpoint for the CCD array. This must be supported
 * by the loaded DSP code too
 ***************************************************************************/
int set_temperature (int fd, int value)
{

	return (do_command (fd, UTIL_ID, WRM, (Y_MEM | 0x1C), value, NONE, NONE, 0, DEF_TIMEOUT));
}


/***************************************************************************
 * Reads the temperature fro the CCD array. This must be supported by the
 * loaded DSP code too
 ***************************************************************************/
int read_temperature (int fd)
{
	return (do_command (fd, UTIL_ID, RDM, (Y_MEM | 0x0C), NONE, NONE, NONE, 0, DEF_TIMEOUT));
}


/***************************************************************************
 * Sets the binning factor. This must be supported by the loaded DSP code too
 ***************************************************************************/
int set_binning (int fd, int xbin, int ybin)
{
char xbinning[3], ybinning[3];
int add, board, memtype;
/*
int ret;
	if ((ret=do_command (fd ,TIM_ID, WRM, (Y_MEM | 0x5), xbin, NONE, NONE, DON, DEF_TIMEOUT)) < 0)
		return (ret);
	return (do_command (fd, TIM_ID, WRM, (Y_MEM | 0x6), ybin, NONE, NONE, DON, DEF_TIMEOUT));
*/
        sprintf (xbinning, "%d", xbin);
        sprintf (ybinning, "%d", ybin);
	if ((add =  SDSU_get_tab_address (XBINLOC, &board, &memtype)) == NOT_IN_TABLE){
		add = 0x05;
		board = TIM_ID;
		memtype = Y_MEM;
	}
	do_command (fd, board, WRM, (memtype | add), xbin, NONE, NONE, 0, DEF_TIMEOUT);
	if ((add =  SDSU_get_tab_address (XBINLOC, &board, &memtype)) == NOT_IN_TABLE){
		add = 0x06;
		board = TIM_ID;
		memtype = Y_MEM;
	}
	return (do_command (fd, board, WRM, (memtype | add), ybin, NONE, NONE, 0, DEF_TIMEOUT));
 //       return (manual_command (fd, xbinning, ybinning, "", "", "", "BIN", TIM_ID));
}


int set_roi (int fd, int xstart, int ystart, int xlen, int  ylen)
{
char xs[8], ys[8], xl[8], yl[8], ov[8], pr[8], rem[8];
short nb=0;
char roiloc[4];
char roilen[4];

        if (SDSU_get_tab_dspcmd (ROILOCCMD, roiloc) == NOT_IN_TABLE)
                strcpy (roiloc, DEFROILOCCMD);
        if (SDSU_get_tab_dspcmd (ROILENCMD, roilen) == NOT_IN_TABLE)
                strcpy (roilen, DEFROILENCMD);

	if (xstart <= 0)
		xstart = 1;
	if (ystart <= 0)
		ystart = 1;
	if (xlen <= 0)
		xlen = 1;
	if (ylen <= 0)
		ylen = 1;

        sprintf (xs, "%d", xstart-1);
        sprintf (ys, "%d", ystart-1);
#if DSP_PRESCAN
        sprintf (xl, "%d", xlen);
#else
        sprintf (xl, "%d", xlen + gbl_prescan);
#endif
        sprintf (yl, "%d", ylen);
//        sprintf (rem, "%d", gbl_prescan + gbl_xdata - (xstart + xlen - 1));
        sprintf (rem, "%d", gbl_xdata - (xstart + xlen - 1));
        sprintf (ov, "%d", gbl_overscan);
        sprintf (pr, "%d", gbl_prescan);
	if ((xlen < gbl_xdata) || (ylen < gbl_ydata))
        	nb=1;
	else
        	nb=0;
	if (nb > 0){
		printf ("SDSU, xstart %d ystart %d xlen %d ylen %d gbl_xdata %d gbl_overscan %d gbl_prescan %d\n", xstart, ystart, xlen, ylen, gbl_xdata, gbl_overscan, gbl_prescan);
		printf ("SDSU, sending SSS: %s %s %s\n", ov, xl, yl);
//        	manual_command (fd, ov, xl, yl, "", "", "SSS", TIM_ID);
        	manual_command (fd, ov, xl, yl, pr, "", roilen, TIM_ID);
		printf ("SDSU, sending SSP: %s %s %s\n", ys, xs, rem);
        	return (manual_command (fd, ys, xs, rem, "", "",roiloc , TIM_ID));
	} else {
		printf ("SDSU, sending SSS: %s %s %s\n", ov, xl, yl);
        	manual_command (fd, ov, xl, yl, "", "", roilen, TIM_ID);
	}
	return (DONE);
}


int set_geom (int fd, int xprescan, int xdata, int xoverscan, int yprescan, int ydata, int yoverscan)
{
char xp[4], yp[4], xo[4], yo[4], xd[4], yd[4];
unsigned int amprows, ampcols;
int add, board, memtype, ret;

	if (xprescan < 0)
		xprescan = 0;
	if (xdata < 0)
		xdata = 0;
	if (xoverscan < 0)
		xoverscan = 0;
	if (yprescan < 0)
		yprescan = 0;
	if (xdata < 0)
		ydata = 0;
	if (yoverscan < 0)
		yoverscan = 0;

        sprintf (xp, "%d", xprescan);
        sprintf (xo, "%d", xoverscan);
        sprintf (yp, "%d", yprescan);
        sprintf (yo, "%d", yoverscan);
        sprintf (xd, "%d", xdata);
        sprintf (yd, "%d", ydata);
	gbl_overscan = xoverscan;
	gbl_prescan = xprescan;
	gbl_xdata = xdata;
	gbl_ydata = ydata;
	if ((add =  SDSU_get_tab_address (AMPCOLSLOC, &board, &memtype)) != NOT_IN_TABLE){
		ampcols = (unsigned int) (xprescan + xdata + xoverscan);
		printf ("writing ampcols %d to dsp\n", ampcols);
		if ((ret = do_command (fd, board, WRM, (memtype | add), ampcols, NONE, NONE ,DON, DEF_TIMEOUT) < 0))
			return (ret);
	}
	if ((add =  SDSU_get_tab_address (AMPROWSLOC, &board, &memtype)) != NOT_IN_TABLE){
		amprows = (unsigned int) (yprescan + ydata + yoverscan);
		printf ("writing amprows %d to dsp\n", amprows);
		if ((ret = do_command (fd, board, WRM, (memtype | add), amprows, NONE, NONE ,DON, DEF_TIMEOUT)) < 0)
			return (ret);
	}
	printf ("SDSU GEOM, gbl_xdata %d gbl_ydata %d  gbl_overscan %d gbl_prescan %d\n", gbl_xdata, gbl_ydata, gbl_overscan, gbl_prescan);
	return (DONE);
}


/***************************************************************************
 * Sets the CCD mode to mpp or not. THis must be supported by the DSP code too
 ***************************************************************************/
int set_mpp (int fd, short flag)
{
	return (do_command (fd, TIM_ID, MPP, flag, NONE, NONE, NONE, DON, DEF_TIMEOUT));
}

int artif_pattern (int fd, int type, char *args)
{
	return ((*dhe_artif_data_p)(fd, type, args));
}

/**********************************************************************
 * Enables or disables the synthetic image mode on the timing board
 **********************************************************************/
int artif_data (int fd, short flag)
{
int ret, add, m_id, val;
unsigned int status;

	m_id = get_masterID();
	add = 0;
	if (m_id != TIM_ID)
		return (-EPERM);

	set_destination (fd, 0, m_id);
	if ((status = read_memory (fd, "X", add)) < 0)
		return (status);
	if (flag) 
		val = (status | _ARTIF_DATA_ON_);
	else
		val = (status & _ARTIF_DATA_OFF_);

	if ((ret = write_memory (fd, "X", 0, val)) < 0)
		return (ret);
	return (DONE);
}

int set_sync (int fd, short flag)
{
int ret, m_id;
char cmd[4];

        if (SDSU_get_tab_dspcmd (SYNCCMD, cmd) == NOT_IN_TABLE)
                strcpy (cmd, DEFSYNCCMD);

	m_id = get_masterID();
	if (m_id != TIM_ID)
		return (-EPERM);

	if (flag)
       		ret = manual_command (fd, "1", "", "", "", "", cmd, TIM_ID);
	else
       		ret = manual_command (fd, "0", "", "", "", "", cmd, TIM_ID);
	
	return (ret);
}
