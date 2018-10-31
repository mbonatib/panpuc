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

int load_pci (int fd, char* filename);
int manual_command (int fd, char *arg1, char *arg2, char *arg3, char *arg4, char *arg5, char *command, short board_dest);
int set_mapsize (int size);
int get_mapsize (void);
int read_reply (int fd, int expected, int timeout);
int set_cmd_data (int fd, int data);
int do_command (int fd, int board_id, int command, int arg1, int arg2, int arg3, int arg4, int expected_reply, int timeout);
int get_hctr (int fd);
int get_hstr (int fd);
int get_frames_progress (int fd);
int get_read_progress (int fd);
int close_drv (int fd);
int open_drv(int device, unsigned short **highmemPtr);
int reset_ctrler (int fd);
int reset_pci (int fd);
int get_masterID (void);
void set_masterID (int id);
int set_destination (int fd, int num_of_args,  int bd);
void set_hctr (int fd, int value);
int set_hcvr (int fd, int command, int expected_reply, int timeout);
int set_size (int fd, unsigned int ncols, unsigned int nrows);
int get_controller_info (int fd);
void get_link_info (int fd, char *info);
void get_cont_info (int fd, char *info);
int get_info (int fd, unsigned short *info);  
int get_cont_status (int fd);
int power (int fd, short flag);
int move_shutter (int fd, short flag);
int abort_exposure (int fd);
int abort_sur (int fd);
int pause_exp (int fd);
int resume_exp (int fd);
int abort_readout (int fd);
int abort_current_image (int fd);
int set_exposure_time (int fd, unsigned int val);
int start_exposure (int fd);
int read_exposure_time (int fd);
int read_frame_time (int fd);
int read_actual_inttime (int fd);
int test_data_link (int fd, short dest, int data);
int read_memory (int fd, char *type, int address);
int write_memory (int fd, char *type, int address, int value);
int autoshutter (int fd, int flag);
int autoclear (int fd, int flag);
int load_application (int fd, int app, short dest);
int load_file (int fd, char *filename, short board_id);
int load_pci (int fd, char *filename);
int manual_command (int fd, char *arg1, char *arg2, char *arg3, char *arg4, char *arg5, char *command, short board_dest);
int set_num_coadds (int fd, int coadds);
int set_num_reads (int fd, int reads);
int set_num_sur (int fd, int sur);
int set_fpb (int fd, int fpb);
int set_gain_and_speed (int fd, int gain, int speed);
int resume_idle (int fd);
int stop_idle (int fd);
int clear_array (int fd);
int set_temperature (int fd, int value);
int read_temperature (int fd);
int set_binning (int fd, int xbin, int ybin);
int set_roi (int fd, int xstart, int ystart, int xlen, int  ylen);
int set_geom (int fd, int xprescan, int xdata, int xoverscan, int yprescan, int ydata, int yoverscan);
int set_mpp (int fd, short flag);
int artif_data (int fd, short flag);
int set_sync (int fd, short flag);
