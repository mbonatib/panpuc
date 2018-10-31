#include "driver_funcs.h"
#include "GTypeDefs.h"

#define MAX_MEM_POINTS	10
#define MINFASTBUF	1024*1024
#define BUF_BYTES_SIZE	2*65536
#define INI_POINTS	0
#define END_POINTS	1
#define READ_MEM	0
#define WRITE_MEM	1
#define EXP_TIME_BREAK	2000
#define CIRC_BUF_SIZE   1024
#define LOAD_FILE_ARGS	0x00030000
#define LOAD_APP_ARGS	0x00010000
#define TDL_ARGS	0x00010000
#define OK		0
#define reply_timeout	6		/* secs */
#define ST_IDLE		0
#define ST_RESET	1
#define ST_EXPO		2
#define ST_READ		3
#define ST_PAUSED	4
#define FM_IDLE		0
#define FM_READING	1
#define FM_ABORTED	2
#define FM_OK		3
#define FM_ERROR	4
#define FM_RDENDED	-1
#define CO_ABORTED	5
#define CHK_B		1
#define cont_READOUT	0x5
#define STOP_TIME	2000

typedef
struct frame_struc {
	ptr_t ptr;
	int buf_read;
	short status;
} frame_t;


typedef
struct mem_struct {
        short id;		/*numeric id for the testponint*/
        short board;		/*timing=2, util=3 or pci=1*/
        short mem_type;		/*D=0, P=1, R=2, X=3, Y=4, etc*/
        ptr_t address;	/*address in memory type (in board)*/
        dptr_t value;		/*value to write or value read*/
        short action;		/*read or write*/
	short status;		/*DON, ERR ot TIMEOUT*/
} memory_t;


typedef
struct img_struct {
	unsigned int exp_prog;
	short detstatus;
	short imgstatus;
	char UTShut[25];
	memory_t *IniMemVals;
	memory_t *EndMemVals;
	int n_inivals;
	int n_endvals;
} image_t;


int free_pointers();
ptr_t return_data_pointer (u64 mbuff,  u64 buf_read, u64 cbuff_size, u64 dbuf_size, u64 *elems_to_write, u64 old_bufs);
//memory_t *read_mem_points (int fd, int where, int num_points, memory_t *mem_array);
void init_cont_read();
void clear_discards();
void clear_counters ();
void clear_exp_vals ();
int clear_mem_points();
int clear_all (int err);
void clear_old_bufs ();
int chk_buffer (void);
ptr_t get_ptr_val ();
ptr_t get_free_buf (int index);
ptr_t get_used_buf ();
void set_num_bufs (int n_ptrs);
void set_ptr_vals (ptr_t *ptrs, int n_ptrs);
ptr_t set_new_buf (ptr_t ptr, int index);
void set_ptr_val (ptr_t val);
void set_bufs_per_frame (unsigned int val);
int get_imstatus (int fd, ptr_t *curr_ptr, short *detstat, int *exp, int *imread, int *imdones, int *coaddsdone, int *fmstat, int *coad_disc, int *img_disc, int *actual_buf_read);
//char* get_KEYS (u64 *vals, short *ids);
//void add_mem_point (void *m_array, int where);
int abort_read (int fd);
int stop_hard_coadd (int fd);
int stop_hard_sur (int fd);
int readNframes (int fd, int num_frames, int bytes_to_receive);
int readNFastframes (int fd, int num_frames, int bytes_to_receive);
int readUltraFastframes (int fd, int num_frames, int bytes_per_frame);
void reset_fastptr_val(void);
int read_exp_time (int fd, int exp_time);
void stop_seq ();
int stop_coadd (int fd);
int stop_exp (int fd);
int set_mapsize (int size);
int set_pixeltime (int fd, double pixtime);
ptr_t get_fastptr_val(void);
int stop_sample_up_the_ramp (int fd);
