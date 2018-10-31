int get_hctr (int fd);
int get_hstr (int fd);
int get_frames_progress (int fd);
int get_read_progress (int fd);
int close_drv (int fd);
int open_drv(int device, unsigned short **highmemPtr);
int reset_ctrler (int fd);
int reset_pci (int fd);
void set_hctr (int fd, int value);
int set_hcvr (int fd, int command, int expected_reply, int timeout);
int set_size (int fd, unsigned int ncols, unsigned int nrows);
int get_controller_info (int fd);
int get_info (int fd, unsigned short *info);  
int get_cont_status (int fd);
int power (int fd, short flag);
int move_shutter (int fd, short flag);
int abort_exposure (int fd);
int pause_exp (int fd);
int resume_exp (int fd);
int abort_readout (int fd);
int abort_current_image (int fd);
int set_exposure_time (int fd, unsigned int val);
int start_exposure (int fd);
int read_exposure_time (int fd);
int test_data_link (int fd, short dest, int data);
int read_memory (int fd, char *type, int address);
int write_memory (int fd, char *type, int address, int value);
int autoshutter (int fd, int flag);
int load_application (int fd, int app, short dest);
int load_file (int fd, char *filename, short board_id);
int load_pci (int fd, char *filename);
int manual_command (int fd, char *arg1, char *arg2, char *arg3, char *arg4, char *arg5, char *command, short board_dest);
int set_num_coadds (int fd, int coadds);
int set_num_reads (int fd, int reads);
int set_num_sur (int fd, int sur);
int set_fpb (int fd, int fpb);
int set_gain_and_speed (int fd, int gain, int speed);
int res_idl (int fd);
int stp_idl (int fd);
int clear_array (int fd);
int set_temperature (int fd, int value);
int read_temperature (int fd);
int set_binning (int fd, int xbin, int ybin);
int set_mpp (int fd, short flag);
int artif_data (int fd, short flag);
int set_destination (int fd, int num_of_args,  int bd);
int read_actual_inttime (int fd);
int set_pixtime (int fd, double pixtime);
int get_pixtime (int fd);
int set_guider_exptime (int fd, int exptime);
int set_window_mode (int fd, int mode, int cols, int rows);
int set_guider_nimages (int fd, int val);