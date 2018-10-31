/* Library for accesng the Leach PCI driver */
#include "driver_funcs.h"
#include "readLib.h"
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
#ifdef _SHMEM_
#include "SDSU_shmem.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif
//static long old_bufs=0;
static int n_inipoints=0;
static int bufs_read=0;
static int n_endpoints=0;
static char *emptyTime = "EMPTY";
static unsigned long next_imptr=0;
static short detstatus=0;
static int exp_prog=0;
static short stop_sequence=0;
static short stop_exposure=0;
static short stop_coadding=0;
static short stop_sur=0;
static short try_stop_coadding=0;
static short try_stop_sur=0;
static short try_stop_next_exp=0;
static short discard_coadd=0;
static short coadds_discarded=0;
static short images_discarded=0;
static int images_done=0;
static int images_started=0;
static int oldimagesdone=0;
static int oldimages_started=0;
static int coadds_done=0;
static int num_reads=0;
static int fmi=0;
static int fmo=0;
static int imi=0;
static int imo=0;
static char FMTime[25];
static unsigned int bufs_per_frame=0;
static ptr_t img_bufs[20];
static int num_bufs=0;
static frame_t frame[CIRC_BUF_SIZE+1];
static image_t image[CIRC_BUF_SIZE+1];
//static memory_t IniMem_array[MAX_MEM_POINTS];
//static memory_t EndMem_array[MAX_MEM_POINTS];
static int columns, rows;
static int nreads;
static int bytes_pp=2;
static int pix_per_image=2;
static int buff_size=BUF_BYTES_SIZE/2;
static unsigned short *framePtr;
static int framesPerBuffer=1;
static int framesCounter=0;
static int fastFramesCounter=0;
static int fastPtrNo=0;
static int pciPixCounter=0;
static int tot_pix_rec=0;
static int ufast_read = 0;
static unsigned short *highmemPtr;
static unsigned int exposure_time=0;
static unsigned int npics=0;
static unsigned int onpics=0;
static short support_abort = 0;
static short support_rit = 0;
static short cont_read = 0;
static unsigned long iniexpdelay=0;
static unsigned long expdelay=0;
unsigned short *fastbufptr=NULL;
unsigned short *curr_fastptr=NULL;
dptr_t curr_imsize;
unsigned short *max_ptr=NULL;
dptr_t fastbufsize;
int shmid=-1;
int shmqid=-1;
int autoidle = 1;
int actual_inttime = -1;
int guidebytes=0;
int gcols=0;
int grows=0;
int guide_mode=0;
int guideFtime=0;
int guideEtime=0;
float gbl_pixtime=0;

int calc_gftime (int pixtime)
{
        return ((int)(float)(gcols*grows)*pixtime / 1000.0);
}

int set_gframes (int fd, int exptime)
{
float gt;
int nimages;
int ret;

        gt =  (float)(2*guideFtime + guideEtime);
        nimages = (int)((float)exptime/gt);
	if ((ret=set_guider_nimages (fd, nimages)) < 0)
		return (ret);
	return (2*nimages);
}

int get_guide_mode (void)
{
	return (guide_mode);
}

int set_guide_mode (int fd, int gmode, int exptime, int cols, int rows)
{
int ret;

	if ((ret=set_window_mode (fd, gmode, cols, rows)) < 0)
		return (ret);
	guide_mode = gmode;
	printf ("guide mode set to %d\n", guide_mode); 
	if (gmode == 0){
		gcols = columns;
		grows = rows;
		return (0);
	}
	if ((ret=set_guider_exptime (fd, exptime)) < 0)
		return (ret);
	gcols = cols;
	grows = rows;
	guidebytes = gcols*grows*bytes_pp;	
	guideEtime = exptime;
	guideFtime = calc_gftime (gbl_pixtime);
        curr_imsize = cols*rows;
//	release_imbuf(framesPerBuffer*curr_imsize);
//        ret = alloc_fastbuf (framesPerBuffer, &shmid, &shmqid);
	return (0);
}

float get_pixeltime (int fd)
{
float ret;

	ret = get_pixtime (fd);
	if (ret < 0)
		return (gbl_pixtime);
	return (ret);
}

int set_pixeltime (int fd, double pixtime)
{
int ret;

	ret = set_pixtime (fd, pixtime);
	if (ret == 0){
		gbl_pixtime = pixtime;
		guideFtime = calc_gftime (pixtime);
	}	
	return (ret);
}

int stop_idle (int fd)
{
	autoidle = 0;
	return (stp_idl (fd));
}

int resume_idle (int fd)
{
	autoidle = 1;
	return (res_idl (fd));
}

void set_inixepdelay (float dms)
{
	if (dms < 0)
		dms = 0;
	iniexpdelay = (unsigned long) (1000*dms);
	printf ("iniexpdelay set to %ld\n", iniexpdelay);
}

void set_expdelay (float dms)
{
	if (dms < 0)
		dms = 0;
	expdelay = (unsigned long) (1000*dms);
	printf ("expdelay set to %ld\n", expdelay);
}

float get_iniexpdelay (void)
{
	return ((float) iniexpdelay/1000);
}

float get_expdelay (void)
{
	return ((float) expdelay/1000);
}

int set_nimages (int fd, int nimages)
{
	npics = npics + (nimages - onpics);
	if (npics < 0)
		npics = 0;
	printf ("npics %d, nimages %d, onpics %d\n", npics, nimages, onpics);
	return (OK);
}


int set_exp_time (int fd, unsigned int exp_time)
{
int ret=OK;

	if ((ret=set_exposure_time(fd, exp_time))<0)
		return (ret);

	exposure_time = exp_time;
	return (ret);
}			

int get_exp_time ()
{
	return (exposure_time);
}

int set_bpp (int bpp)
{
	bytes_pp = bpp/8;
	buff_size = BUF_BYTES_SIZE/bytes_pp;
	return (OK);
}
	

/**********************************************************************
 * Get the readout progress (how many buffers has been read till now)
 *********************************************************************/
int get_readout_progress (int fd)
{
int pix=0;

	if ((pix=get_read_progress (fd)) < 0)
		return (pix);

//	printf ("totpixs %d (tot_pix_rec %d     \n", pix, tot_pix_rec);
	pix = pix - tot_pix_rec;	/*only THIS frame progress*/
	if (pix < 0) {		/*it wnet back to the first buffer*/
		printf ("went to first buffer (pix %d); forced to max (%d)\n", pix, pix+tot_pix_rec);
		pix = pix_per_image;
	}

	return (pix);
}

int release_imbuf(int size)
{
#ifdef _SHMEM_
int ret;
char msg[MAXLEN];
             if (shmid >= 0) {
                  sprintf (msg, "SHMEM NEWSIZE %d", size);
                  SDSU_shmSendMsg (shmqid, msg, MSGSHM);
		  usleep (10000);
                  if ((ret = SDSU_shmDestroy (SHM_NAME,fastbufptr, shmid, shmqid)) < 0) {
                           return (ret);
                  }
            }
#else
        free(fastbufptr);
#endif
        fastbufsize = 0;
        fastbufptr = NULL;
        max_ptr = NULL;
        curr_fastptr=NULL;
        return (OK);
}

/*free struct pointers*/
int free_pointers()
{
int i;

        for (i=0; i<CIRC_BUF_SIZE; i++)
                if (image[i].IniMemVals != NULL)
                       free(image[i].IniMemVals);

        for (i=0; i<CIRC_BUF_SIZE; i++)
                if (image[i].EndMemVals != NULL)
                        free(image[i].EndMemVals);

	release_imbuf (0);
        return (OK);
}

int set_cont_read (int fd, int flag)
{
	if (flag)
		cont_read = 1;
	else
		cont_read = 0;

	return (OK);
}

int get_cont_read (int fd)
{
	return (cont_read);
}


/**********************************************************************
 *  * Closes the driver
 *   *********************************************************************/
int close_driver (int fd)
{
       free_pointers();
       return (close_drv (fd));
}


/***************************************************************************
 * Opens the driver. The "device" parameter says which card (starts from 0)
 ***************************************************************************/
int open_driver(int device, int *err)
{
int fd;

	printf ("DHE device %d\n", device);
	if ((fd = open_drv (device, &highmemPtr)) < 0)
		return (fd);
	if ((dptr_t)highmemPtr <= 0) {
		close_driver (fd);
		*err = -ENOMEM;
		return (0);
	}
	printf ("highmemPtr 0x%lx\n", (dptr_t) highmemPtr);
	init_cont_read ();
#if 0
int ret;
        if ((ret=load_pci (fd, "/home/ArcVIEW/Modules/SDSU/private/c/SDSUIII/DSPfiles/PCI/pci.lod")) < 0) {
	                close_driver(fd);
	                return (ret);
		        }
#endif
	*err=0;
	return (fd);
}



/**********************************************************************
 * Resets the Leach Controller
 *********************************************************************/
int reset_controller (int fd)
{
int ret;

	if ((ret = reset_ctrler (fd)) < 0)
		return (ret);
	nreads = 1;
	return (ret);
}

int alloc_fastbuf (int fpb, int *id, int *qid)
{
int nsize;
size_t tsize;

	nsize = fpb*curr_imsize;
	if (nsize < MINFASTBUF)
		nsize = MINFASTBUF;
	if (nsize > fastbufsize){
		if ((fastbufsize > 0) && (fastbufptr != NULL)){
			release_imbuf(nsize);
		}
		tsize = nsize*bytes_pp;
#ifdef _SHMEM_
		fastbufptr = SDSU_shmAttach (&tsize, SHM_NAME, NULL, id, qid);
		printf ("allocated shared memory segment (0x%lx) of %d bytes\n", (dptr_t) fastbufptr, tsize);
#else
		printf ("bytes_pp %d curr_imsize %d\n", bytes_pp, curr_imsize);
		printf ("allocated non-shared memory segment (%d)\n", tsize);
		fastbufptr = malloc (tsize);
		*id = -1;
		*qid = -1;
		printf ("after allocated non-shared memory segment (0x%x)\n", fastbufptr);
#endif
		if (fastbufptr == NULL){
			printf ("allocation failed\n");
			return (-ENOMEM);
		}
		fastbufsize = nsize;
	}
        printf ("1\n");
	curr_fastptr = fastbufptr;
        printf ("2\n");
	max_ptr = fastbufptr + (fastbufsize - curr_imsize);
        printf ("fastbufsize %d\n", fastbufsize);
	printf ("max bufs %d (bufsize %ld, imsize %ld)\n", (int) (fastbufsize / curr_imsize), fastbufsize, curr_imsize);
	return (OK);
}
	

int set_fastbufptr (ptr_t ptr, u64 size, int qid)
{
	if (ptr <= 1000)
		return (-EFAULT);
	fastbufptr = (unsigned short *)(dptr_t) ptr;
	curr_fastptr = fastbufptr;
	fastbufsize = (ptr_t)((float)size / (float)sizeof (short));
//	fqid = qid;
	return (OK);
}

//********************************************************************
int dimensions (int fd, unsigned int ncols, unsigned int nrows)
{
int ret=OK;

	if ((ret = set_size (fd, ncols, nrows)) < 0)
		return (ret);
	columns = ncols;
	rows = nrows;
	gcols = ncols;
	grows = nrows;
	curr_imsize = ncols*nrows;
        if (curr_imsize == 0){
		printf ("ERROR ILL defined FPA size\n");
		return (-EINVAL);
	}
	ret = alloc_fastbuf (framesPerBuffer, &shmid, &shmqid);
	return (ret);
}


//*************************************************************************
int pause_exposure (int fd)
{
int ret;

	if ((ret = pause_exp (fd))<0)
		return (ret);
	detstatus = ST_PAUSED;
	return (ret);
}


//***********************************************************************
int resume_exposure (int fd)
{
int ret;
	 if ((ret = resume_exp (fd))<0)
		 return (ret);
	 detstatus = ST_EXPO;
	 return (ret);
}

int set_maping_size (int size_MB)
{
	return (set_mapsize (size_MB));
}

int get_frames_per_buffer (int fd)
{
	return (framesPerBuffer);
}

int set_frames_per_buffer (int fd, int fpb)
{
int ret=0;

	if (fpb <= 0)
		fpb = 1;

	if ((ret = set_fpb (fd, fpb)) < 0)
		return (ret);
	framesPerBuffer = fpb;
	
	printf ("framesperbuffer is now %d\n", framesPerBuffer);
//	return (alloc_fastbuf (framesPerBuffer, &shmid, &shmqid));
	return (0);
}



unsigned short *Update_Frame_Info (int pixels_received)
{
	tot_pix_rec += pixels_received;
	pciPixCounter = tot_pix_rec;
	if ((tot_pix_rec % 512) != 0)
        	tot_pix_rec = 512*((int)(tot_pix_rec/512) + 1);

	if (++framesCounter >= framesPerBuffer)
		init_cont_read ();
//	printf ("UpdateFrameInfo: framePtr 0x%lx tot_pix_rec %d framesPerBuffer %d\n", (unsigned long) highmemPtr + tot_pix_rec, tot_pix_rec, framesPerBuffer);
//	printf ("UpdateFrameInfo: framePtr 0x%lx tot_pix_rec %d\n", (unsigned long) highmemPtr + tot_pix_rec, tot_pix_rec);
	return (highmemPtr + tot_pix_rec);
}

unsigned short *get_next_pci_ptr (int pix_per_frame)
{
unsigned short* next;

	next = (highmemPtr + tot_pix_rec);
//	Update_Frame_Info (pix_per_frame);
	return (next);
}

/*for  readouts <100 ms*/
int fast_expose (int fd, int exp_time, int nimages, int frames_per_image)
{
int ret,bytesperframe, nframes;

  detstatus = ST_READ;
  ufast_read = 1;
  clear_all (OK);
  init_cont_read ();
  reset_fastptr_val();
  if (frames_per_image <= 0)
	frames_per_image = 1;
  if (nimages <= 0)
	nimages = 1;
  nframes = frames_per_image*nimages;
  printf ("exp time %d nframes %d\n", exp_time, nframes);
 bytesperframe = gcols*grows*bytes_pp;
 if (exp_time < 10)
	exp_time = 10;
  if ((ret = set_exp_time (fd, exp_time)) < 0) {
	printf ("error %d setting exp time\n", ret);
	return (ret);
  }
  if ((ret = set_num_coadds (fd, nimages)) < 0) {
	printf ("error %d setting coadds\n", ret);
	return (ret);
  }
 if ((ret=start_exposure (fd)) != OK) {
        printf ("error starting exposure, trying again\n");
        if ((ret=start_exposure (fd)) != OK) {
               reset_pci (fd);
               return (ret);
	}
  }
 ret=readUltraFastframes (fd, nframes, bytesperframe);

  if (!autoidle)
	stop_idle (fd);
  clear_exp_vals();
  printf ("fast sequence finished with %d\n", ret);
  return (ret);
}
	

/*Optical exposure: expose -> read*/
int expose_optical (int fd, int bytes_to_receive, int exp_time, int nimages, int fast, int nreads)
{
int ret;
struct timeval tval;
struct tm *tm_time;
time_t startsecs;
int image_aborted=0;
int pix;

  printf ("optical expose routine started bytes2receive %d (bytes_pp %d tot_pix_rec %d, framesCounter %d, npics %d, fast %d)\n", bytes_to_receive, bytes_pp, tot_pix_rec, framesCounter, npics, fast);
  clear_counters ();
  clear_discards();
  clear_all (OK);
  clear_old_bufs ();
  images_done = oldimagesdone = oldimages_started = images_started = 0;
  onpics = nimages;
  npics = nimages;
  if (exp_time > 0)
	detstatus = ST_EXPO;
  else
	detstatus = ST_READ;
  printf ("exposure time %d\n", exp_time);
 if (npics <=0)
	npics = 1;
  if (exp_time < 0)
	  exp_time = 0;
//  if (exp_time == 0)
//	exp_time=5;

  printf ("exp time %d\n", exp_time);
  if ((ret = set_exp_time (fd, exp_time)) < 0) {
	frame[fmi].status = FM_ERROR;
	printf ("error %d setting exp time\n", ret);
	return (clear_all (ret));
  }
  while (npics && !stop_sequence) {
  	init_cont_read ();
	if (!cont_read)
		npics--;
        if (discard_coadd && stop_sequence)
                return (clear_all (-ECONNABORTED));
	if (npics < 0)
		break;

	if (stop_exposure){
		stop_exposure = 0;
		set_exp_time (fd, exp_time);
	}
	image_aborted=0;
	if (iniexpdelay > 0)
		usleep (iniexpdelay);

        gettimeofday (&tval, NULL);
        startsecs = tval.tv_sec;
        tm_time = gmtime (&startsecs);
        sprintf (FMTime, "%d-%02d-%02dT%02d:%02d:%06.3f", 1900 + tm_time -> tm_year, 1 + tm_time -> tm_mon, tm_time -> tm_mday,  tm_time -> tm_hour, tm_time -> tm_min, (double) tm_time -> tm_sec+(double) tval.tv_usec/1000000);
	printf ("UT: %s\n", FMTime);
	strcpy (image[imi].UTShut, FMTime);
  	if ((ret=start_exposure (fd)) != OK) {
		 printf ("error starting exposure, trying again\n");
	 	if ((ret=start_exposure (fd)) != OK) {
			frame[fmi].status = FM_ERROR;
			abort_exposure (fd);
			reset_pci (fd);
			return (clear_all (ret));
	 	}
  	} else
		printf ("exposure started\n");
	images_started++;
	usleep (100000);
//	if (exp_time > EXP_TIME_BREAK)
//		usleep (EXP_TIME_BREAK*1000);
	if ((pix=read_exp_time (fd, exp_time))<0) 
 		goto opt_err;
	printf ("finished reading exp time (%d)\n", exp_prog);
	exp_prog = get_exp_time ();
	detstatus = ST_READ;
	if (fast)
		pix=readNFastframes (fd, nreads, bytes_to_receive);
	else
		pix=readNframes (fd, nreads, bytes_to_receive);
opt_err:
	printf ("back from readNframes\n");
	if (pix <0) {
		images_discarded++;
		if (errno == ECONNABORTED){
			printf ("readout aborted\n");
			frame[fmi].status = FM_ABORTED;
		} else{
			frame[fmi].status = FM_ERROR;
			printf ("reading ret < 0! (%d)\n", pix);
			return (clear_all (pix));
		}
	}
	images_done++;
        if (imi++ > CIRC_BUF_SIZE)
                imi = 0;
	exp_prog = 0;
	if (expdelay > 0)
		usleep (expdelay);
    }
    frame[fmi].status = FM_RDENDED;
    clear_exp_vals ();
    if (discard_coadd && stop_sequence)
          return (clear_all (-ECONNABORTED));
  printf ("expose optical routine finished OK (%d bufs read)\n", bufs_read);
  if (!autoidle)
	stop_idle(fd);
return (OK);
}

void set_status_expo ()
{
	detstatus = ST_EXPO;
}

/***************************************************************************
 * Reads npics complete coadded images. For the optical case, num_coadds=1
 * always. Prevex and Postex are the amount of frames to be read before and
 * after the exposure, respectively. For the optical case, this will always
 * be prevex=0 and postex=1. For Fowler Sampling, it will normally be
 * prevex = postex = fawler_samples.
 ***************************************************************************/
int expose (int fd, int num_prevex, int num_postex, int bytes_to_receive, int exp_time, int num_coadds, int nimages, int num_sur, int fast)
{
int ret;
struct timeval tval;
struct tm *tm_time;
time_t startsecs;
int ncoadds=0;
int image_aborted=0;
int counter=0;
int pix;
int nsur;
int guideframes=0;
int pix_to_receive;

  pix_to_receive = (int)((float)bytes_to_receive/bytes_pp);
  ufast_read = 0;
  if (num_prevex == 0)
	return (expose_optical (fd, bytes_to_receive, exp_time, nimages, fast, num_postex));

  printf ("expose routine started prevex %d, postex %d bytes2receive %d (bytes_pp %d num_coadds %d, tot_pix_rec %d, framesCounter %d, npics %d, nsur %d exptime %d, fast %d)\n", num_prevex, num_postex, bytes_to_receive, bytes_pp, num_coadds, tot_pix_rec, framesCounter, npics, num_sur, exp_time, fast);
  clear_counters ();
  clear_discards();
  clear_all (OK);
  clear_old_bufs ();
  images_done = coadds_done = oldimagesdone = oldimages_started = images_started = 0;
  onpics = nimages;
  npics = nimages;
  printf ("exposure time %d\n", exp_time);
 if (npics <=0)
	npics = 1;
 if (num_sur <=0)
	num_sur = 1;
  if (num_coadds <= 0)
	num_coadds = 1;
  if (exp_time <= 0)
	  exp_time = 0;
  if (exp_time <EXP_TIME_BREAK){
  	num_prevex = num_prevex + num_postex;
        num_postex = 0;
  }
  if ((ret = set_exp_time (fd, exp_time)) < 0) {
	frame[fmi].status = FM_ERROR;
	printf ("error %d setting exp time\n", ret);
	return (clear_all (ret));
  }
 if (guide_mode)
	guideframes = set_gframes (fd, exp_time);
/*
  if ((ret = set_num_reads (fd, num_prevex)) < 0){
	frame[fmi].status = FM_ERROR;
	printf ("error %d setting number of reads (fsampling\n", ret);
	return (clear_all (ret));
  }
  printf ("fsampling: %d\n", num_prevex);
*/
  if ((ret = set_num_coadds (fd, num_coadds)) < 0){
	frame[fmi].status = FM_ERROR;
	printf ("error %d setting number of coadds\n", ret);
	return (clear_all (ret));
  }
  if ((ret = set_num_sur (fd, num_sur)) < 0){
	frame[fmi].status = FM_ERROR;
	printf ("error %d setting number of sur\n", ret);
	return (clear_all (ret));
  }
new_coadd:
  while (npics && !stop_sequence) {
  	init_cont_read ();
	if (!cont_read)
		npics--;
  	ncoadds = num_coadds;
        if (discard_coadd && stop_sequence)
                return (clear_all (-ECONNABORTED));
	if (discard_coadd)
		discard_coadd = 0;
	if (npics < 0)
		break;

	if (stop_exposure){
		stop_exposure = 0;
		set_exp_time (fd, exp_time);
	}

	image_aborted=0;
	coadds_done = 0;
new_image:
	if (iniexpdelay > 0)
		usleep (iniexpdelay);
	detstatus = ST_RESET;
        gettimeofday (&tval, NULL);
        startsecs = tval.tv_sec;
        tm_time = gmtime (&startsecs);
        sprintf (FMTime, "%d-%02d-%02dT%02d:%02d:%06.3f", 1900 + tm_time -> tm_year, 1 + tm_time -> tm_mon, tm_time -> tm_mday,  tm_time -> tm_hour, tm_time -> tm_min, (double) tm_time -> tm_sec+(double) tval.tv_usec/1000000);
//	printf ("UT: %s\n", FMTime);
  	if ((ret=start_exposure (fd)) != OK) {
		usleep (1000000);
		 printf ("error starting exposure, trying again\n");
	 	if ((ret=start_exposure (fd)) != OK) {
			frame[fmi].status = FM_ERROR;
			abort_exposure (fd);
			reset_pci (fd);
			return (clear_all (ret));
	 	}
  	} 
	strcpy (image[imi].UTShut, FMTime);
	images_started++;

	while (ncoadds && !stop_coadding){
		counter = 0;
		ncoadds--;
		nsur = num_sur;
		if (fast){
			if ((pix=readNFastframes (fd, num_prevex, bytes_to_receive))<0)
				goto err;
		} else{		
			if ((pix=readNframes (fd, num_prevex, bytes_to_receive))<0)
				goto err;
		}
	    	while (nsur && !stop_sur) {
			nsur--;
			if (try_stop_sur) {
				if (stop_hard_sur (fd) == OK) {
					stop_sur=1;
					try_stop_sur = 0;
				}
			}
			if (try_stop_coadding) {
				if (stop_hard_coadd (fd) == OK) {
					stop_coadding = 1;
					stop_sequence = 1;
					try_stop_coadding = 0;
				}
			}
			if (try_stop_next_exp) 
				if (stop_exp (fd) == OK)
					try_stop_next_exp = 0;

			if (guide_mode){
				printf ("guiding mode (%d frames, %d bytes)\n", guideframes, guidebytes);
//				init_cont_read();
				reset_fastptr_val();
				if ((pix= readUltraFastframes (fd, guideframes, guidebytes))<0){
 					goto err;
				}
				printf ("done fast\n");
				init_cont_read();
				pciPixCounter = get_readout_progress (fd);
//				tot_pix_rec = get_readout_progress (fd);
//				pciPixCounter = 0;
			} else {
				printf ("reading exptime\n");
				if ((pix=read_exp_time (fd, exp_time))<0) 
 					goto err;
			}
/*	
			if (discard_coadd){
				frame[fmi].status = CO_ABORTED;
				coadds_discarded++;
				goto new_coadd;
			}
*/
			if ((discard_coadd && stop_sequence)){
				frame[fmi].status = CO_ABORTED;
				return (clear_all(-ECONNABORTED));
			}
		
	//		printf ("finished reading exp time (%d)\n", exp_prog);
			exp_prog = exp_time;
			detstatus = ST_READ;
			if (fast)
				pix=readNFastframes (fd, num_postex, bytes_to_receive);
			else
				pix=readNframes (fd, num_postex, bytes_to_receive);
err:
			if (pix <0) {
				images_discarded++;
				if (discard_coadd){
		  			frame[fmi].status = CO_ABORTED;
					coadds_discarded++;
					image_aborted=1;
					goto new_coadd;
				}
				if (errno == ECONNABORTED){
					printf ("readout aborted\n");
		  			frame[fmi].status = FM_ABORTED;
					goto new_image;
				} else{
					frame[fmi].status = FM_ERROR;
					printf ("reading ret < 0! (%d)\n", pix);
					return (clear_all (pix));
				}
	  		}
		}
		coadds_done++;
		clear_exp_vals ();
//		if ((ret=read_reply (fd, DON, -1)) != OK)
//			return (clear_all (ret));
	}
	images_done++;
        if (imi++ > CIRC_BUF_SIZE)
                imi = 0;
	if (expdelay > 0)
		usleep (expdelay);
	if (support_rit)
		actual_inttime = read_actual_inttime (fd);
	clear_exp_vals ();
    }
//  clear_all (OK);
    frame[fmi].status = FM_RDENDED;
    if (discard_coadd && stop_sequence)
          return (clear_all (-ECONNABORTED));
  printf ("expose routine finished OK (%d bufs read)\n", bufs_read);
//  return (read_reply (fd, DON, -1));
#if 0  
  for (counter=0; counter<2048; counter++)
	if (*(framePtr + counter) != 32000)
		printf ("%ld=%d ", counter, *(framePtr + counter));
#endif	
	if (!autoidle)
		stop_idle(fd);
return (OK);
}


int readNframes (int fd, int num_frames, int bytes_to_receive)
{
int pix_to_receive;
int pix, oldpix, newpix, frames_read, tout, prevpix=0;
struct timeval timest;
double beg, end;

 pix_to_receive = (int)((float)bytes_to_receive/bytes_pp);
 pix_per_image = pix_to_receive;
 frames_read = 0;
 printf ("num_frames %d\n", num_frames);
 for (num_reads=0; num_reads<num_frames; num_reads++) {
 	printf ("num_reads %d\n", num_reads);
#if CHK_B
       if (chk_buffer () < 0) {
              abort_read (fd);
              return (-ENOBUFS);
       }
#endif
       gettimeofday (&timest, NULL);
       beg = (double)timest.tv_sec + (double)timest.tv_usec/1000000;
       if (fmi++ > CIRC_BUF_SIZE)
                   fmi = 0;
	frame[fmi].ptr = next_imptr;
	frame[fmi].status = FM_READING;
        next_imptr = 0;
	tout = 0;
	printf ("fmi %d fmo %d pointer %ld, progress %d pciPixCounter %d pix_to_receive %d\n", fmi, fmo, (dptr_t)frame[fmi].ptr, frame[fmi].buf_read, pciPixCounter, pix_to_receive);
//	printf ("fmi %d fmo %d progress %d \n", fmi, fmo, frame[fmi].buf_read);
	while (((pix = get_readout_progress (fd)) == pciPixCounter) && (tout < 30000)){
		usleep (1000);
		tout++;
	}
	oldpix = newpix = tout = prevpix = bufs_read = 0;
//	printf ("fist pix got: %d pointer %ld", pix, frame[fmi].ptr);
	while (frames_read <= num_reads){
		pix = get_readout_progress (fd);
//		printf ("read=%d oldpix = %d ", pix, oldpix);
//		usleep (1000000);
		if (pix == prevpix){
			usleep (10000);
//			printf ("same as before ... (%d)\n", tout);
			if (tout++ > 3000){
				printf("read timed out! (pix read: %d\n", pix);
				pix = -ETIME;
				errno = ETIME;
				abort_read (fd);
		//		reset_pci (fd);
				return (pix);
			}
		} else
			tout = 0;
		prevpix = pix;
		if (pix < 0)
			return (pix);
		if (pix > pix_to_receive){
			printf ("expose: warning, received more pixels than expected! (at least %d)\n", pix); 
			pix = pix_to_receive;
		}

		if ((pix < oldpix) && (framesCounter == framesPerBuffer -1)) {	/* it started next frame on initial address */
			pix = pix_to_receive;
			printf ("pix forced to max!\n");
		}
		
		if ((newpix = pix - oldpix) >= buff_size) {
//			printf ("copying %d pixs (0x%lx --> 0x%lx)\n", newpix, (unsigned long)(framePtr + oldpix), frame[fmi].ptr + oldpix);
			memcpy ((unsigned short *)((unsigned short *)(dptr_t)frame[fmi].ptr + oldpix), (unsigned short *)(framePtr + oldpix), newpix*bytes_pp);
			oldpix = pix;
			bufs_read = floor ((float)pix/buff_size);
		} else if (pix == pix_to_receive){
//				printf ("copying %d pixs (0x%lx --> 0x%lx)\n", pix_to_receive - oldpix, (unsigned long)(framePtr + oldpix), frame[fmi].ptr + oldpix);
				memcpy ((unsigned short *)((unsigned short *)(dptr_t)frame[fmi].ptr + oldpix), (unsigned short *)(framePtr + oldpix), bytes_to_receive - oldpix*bytes_pp);
				oldpix = pix;
				bufs_read = ceil ((float) pix/buff_size);
		}
		if (pix == pix_to_receive) {
			frames_read++;
			framePtr = Update_Frame_Info (pix_to_receive);
			bufs_read = ceil ((float) pix/buff_size);
		}

		if (pix < 0) 
			return (pix);
		frame[fmi].buf_read = bufs_read;
	  	printf ("progress %d of %d\r", frame[fmi].buf_read, bufs_per_frame);
	  }
          frame[fmi].status = FM_OK;
//	  printf ("fmi %d fmo %d progress %d\n", fmi, fmo, frame[fmi].buf_read);
	  gettimeofday (&timest, NULL);
          end = (double)timest.tv_sec + (double)timest.tv_usec/1000000;
	  printf("\nread %d took %5.3f\n", num_reads, end-beg);
#if 0
	  int i;
	  printf("\nfirst pixels:\n");
	  for (i=0; i<12; i++){
	  	printf("[%d]: %d\n", i, *(ushort *)((ushort *)frame[fmi].ptr+i));
	  }
#endif
    }
return (OK);
}	



int readUltraFastframes (int fd, int num_frames, int bytes_per_frame)
{
int frames_read, tout;
//unsigned short *ptr;
void *ptr;
//void *oldptr;
int pix_per_frame, ret, tries;
int pix=0;
//float sum;
//int i;
//int fcnt=0;

 frames_read = 0;
 fastFramesCounter = 0;
 pix_per_frame = (int)((float)bytes_per_frame/bytes_pp);
 for (num_reads=0; num_reads<num_frames; num_reads++) {
	tout = 0;
// 	ptr = (unsigned short *)get_fastptr_val();
 	ptr = (void *)(dptr_t)get_fastptr_val();
        while (((frames_read = get_frames_progress (fd)) <= fastFramesCounter) && (tout <= 4000)){
              usleep (1000);
            tout++;
       }
#if 0
	fcnt++;
	if (fcnt == 2){
		fnc = 0;
		for (i=0; i<pix_per_frame; i++){
	} else {
		oldptr = ptr;
	}	
#endif		
//	memcpy (ptr, (unsigned short *)(framePtr), bytes_per_frame);
	pix=get_read_progress(fd);
	memcpy (ptr, (void *)(framePtr), bytes_per_frame);
	printf ("%d %d %d 0x%lx %ld)\n", frames_read, *framePtr, pix, (dptr_t)framePtr, (dptr_t) ptr);
#if _SHMEM_
       	SDSU_shmSendNewImg (shmqid, fastPtrNo, bytes_pp, gcols, grows);
/*	sum = 0;
	for (i=0; i<pix_per_frame; i++)
		sum = sum + *((unsigned short *)ptr + i);
	printf ("mean %f\n", sum/(float)pix_per_frame);
*/
#endif
	fastFramesCounter++;
	printf ("nframes %d (%d)\n", fastFramesCounter,fastPtrNo);
        framePtr = Update_Frame_Info (pix_per_frame);
	if (tout > 4000) {
		return (-ETIME);
	}
	if ((stop_sequence) && (!guide_mode)){
		tries=0;
//		while ((ret=abort_exposure(fd)) == -EIO){
		while ((ret=stop_hard_coadd(fd)) == -EIO){
			usleep (5000);
			tries++;
			if (tries >= 250)
				break;
		}
		printf ("abort returned %d\n", ret);
		if (ret == -ETIME)
			ret = OK;
		return (ret);
	}
  }
return (OK);
}	


/*readtimes 100ms < read_time < 1s*/
int readNFastframes (int fd, int num_frames, int bytes_per_frame)
{
int pix_per_frame;
int frames_read, frames_proc, tout;

 pix_per_frame = (int)((float)bytes_per_frame/bytes_pp);
 printf ("pix_per_frame %d, framesCounter %d,num_frames %d\n", pix_per_frame, framesCounter, num_frames);
 frames_read = frames_proc = bufs_read = 0;
 for (num_reads=0; num_reads<num_frames; num_reads++) {
       if (fmi++ > CIRC_BUF_SIZE)
                   fmi = 0;
        frame[fmi].status = FM_READING;
        frame[fmi].ptr = get_fastptr_val();
        tout = bufs_read = 0;
        while (((frames_read = get_frames_progress (fd)) <= frames_proc) && (tout <= 4000)){
                usleep (1000);
                printf ("framesprog %d\r", frames_read);
                tout++;
        }
        if (tout > 4000) {
                frame[fmi].status = FM_ERROR;
                return (-ETIME);
        }
	memcpy ((unsigned short *)((unsigned short *)(dptr_t)frame[fmi].ptr), (unsigned short *)(framePtr), bytes_per_frame);
        frames_proc++;
//        printf ("\nframes_read %d frames_proc %d framesCounter %d\n", frames_read, frames_proc, framesCounter);
        framePtr = Update_Frame_Info (pix_per_frame);
        frame[fmi].buf_read = bufs_per_frame;
        bufs_read = bufs_per_frame;
        frame[fmi].status = FM_OK;
        printf ("frame[%d].buf_read = %d ptr=0x%lx\n", fmi, frame[fmi].buf_read, (dptr_t)frame[fmi].ptr);
    }
return (OK);
}



int read_exp_time (int fd, int exp_time)
{
int oldexp_prog, tout, counter;

		num_reads=oldexp_prog = tout =  0;
		if (get_exp_time() > EXP_TIME_BREAK){
			printf ("expose: reading exposure time\n");
			detstatus = ST_EXPO;
			exp_prog=0;
			counter=0;
			usleep (100);
                        while ((read_exposure_time (fd) >= get_exp_time()-EXP_TIME_BREAK) && counter<2000){
				  counter++;
				  usleep (1000);
				  printf ("exptime > requested (%d)\n", exp_time);
			}
			if (counter == 2000) {
				errno = ETIME;
				frame[fmi].status = FM_ABORTED;
				return (-ETIME);
			}	
			counter = 0;
        		while ((exp_prog < get_exp_time()-EXP_TIME_BREAK) && (exp_prog >=0)) {
			  if (detstatus == ST_EXPO){
				exp_prog=read_exposure_time (fd);
				printf ("exp time=%d (of %d)\r", exp_prog, get_exp_time());
				if ((exp_prog < 0) && (detstatus == ST_EXPO))
					if (counter < 9) {
						exp_prog = 0;
						counter++;
						usleep (1000);
						printf ("exp tmout, trying again...(%d)\n", counter);
					}
				if ((discard_coadd && stop_sequence)){
					frame[fmi].status = CO_ABORTED;
					return (clear_all(-ECONNABORTED));
				}
				if ((exp_prog == oldexp_prog) && (detstatus == ST_EXPO)) {
					printf ("exp time=%d\n", exp_prog);
					if (tout++ > 2000) {
						errno = ETIME;
						printf ("exposure time timed out\n");
						frame[fmi].status = FM_ABORTED;
						return (-ETIME);
					}
				} else
					tout = 0;
				oldexp_prog = exp_prog;
			  	usleep (1000);
			  } else
				usleep (30000); 
			};
		}
printf ("\n");
return (OK);
}



int clear_progress_buf (fd)
{
	bufs_read = 0;
	return (OK);
}



void clear_old_bufs ()
{
//	old_bufs = 0;
}

/***************************************************************************
 * Return a pointer to the place where the last image chunk was written
 * This function is used for real time displaying and real time writing to disk
 ***************************************************************************/
ptr_t return_data_pointer (ptr_t mbuff,  u64 buf_read, u64 cbuff_size, u64 dbuf_size, u64 *elems_to_write, u64 old_bufs)
{
#if 0
long bytes_to_write, bytes_remain, new_chunk, current_pos;
unsigned short *cbuff;


        if (buf_read <= 0)
                return (0);

        cbuff = (unsigned short *)mbuff;

        if (cbuff == NULL)
                return (-ENOMEM);
	
	current_pos = old_bufs * dbuf_size;
	new_chunk = (bufs_read - old_bufs) * dbuf_size;
	printf ("new_chunk %ld buf_read %ld old_bufs %ld ", new_chunk, buf_read, old_bufs);
        bytes_remain = cbuff_size -  current_pos;
        if (bytes_remain <0)
                return (0);

        if (bytes_remain >= new_chunk)
                bytes_to_write = new_chunk;
        else 
                bytes_to_write = bytes_remain;

	*elems_to_write = (int) bytes_to_write/2;
	printf ("return_data_ptr: pos = %ld, dbuf = %ld oldbufs = %ld buf_read=%ld elem2write %ld new_chunk %ld bytes_remain %ld\n", (unsigned long) (cbuff + current_pos/sizeof(short)), dbuf_size, old_bufs, buf_read, *elems_to_write, new_chunk, bytes_remain);
	old_bufs  = buf_read;
	return ((unsigned long) (cbuff + current_pos/sizeof(short)));
#else
	long bytes_to_write, bytes_remain, current_pos;
	unsigned short *cbuff;


	if (buf_read <= 0)
	          return (0);

        cbuff = (unsigned short *)(dptr_t)mbuff;

        if (cbuff == NULL)
                return (-ENOMEM);

        current_pos = (buf_read - 1) * dbuf_size;
        bytes_remain = cbuff_size -  current_pos;
        if (bytes_remain <0)
                return (0);

        if (bytes_remain >= dbuf_size)
                bytes_to_write = dbuf_size;
        else
                bytes_to_write = bytes_remain;
	*elems_to_write = (int) bytes_to_write/2;
										        return ((ptr_t)(dptr_t) (cbuff + current_pos/sizeof(short)));
#endif
} 	


#if 0
/***************************************************************************
 * read the memory points defined (added with the "add_mem_point function)"
 * and leave the actual results in the memory_t array passed
 ***************************************************************************/
memory_t *read_mem_points (int fd, int where, int num_points, memory_t *mem_array)
{
int i, args, ret;
char *mtype;
memory_t *static_mem_array, *initial_mem_array;

	if (num_points <= 0)
		return (NULL);

        if (where == INI_POINTS)
                static_mem_array = IniMem_array;
        else
                static_mem_array = EndMem_array;

	mem_array = (memory_t *)realloc (mem_array, num_points*sizeof (memory_t));
	initial_mem_array = mem_array;

        for (i=0; i<num_points; i++){
		*(mem_array) = *(static_mem_array);
                switch (mem_array->mem_type){
                        case 0: mtype = "D"; break;
                        case 1: mtype = "P"; break;
                        case 2: mtype = "R"; break;
                        case 3: mtype = "X"; break;
                        case 4: mtype = "Y"; break;
                        default: mtype = "U"; break;
                }

             printf ("reading mempoints: board %d mem_type %d address %ld value %d action %ld status %d\n", mem_array->board, mem_array->mem_type, mem_array->address, mem_array->action, mem_array->value, mem_array->status);

                if (mem_array->action == READ_MEM)
                        args = 2;
                else
                        args = 3;
                if ((ret=set_destination (fd, args, mem_array->board)))
                        return (NULL);

                if (mem_array->action == WRITE_MEM)
                        mem_array->status = write_memory (fd, mtype , mem_array->address, mem_array->value);
                 else {
                        mem_array->value = read_memory (fd, mtype , mem_array->address);
                        mem_array->status = mem_array->value;
                }
		printf ("reading board %d, on address %ld reseult %ld\n", mem_array->board, mem_array->address, mem_array->value);
                mem_array++;
		static_mem_array++;
        }
        return (initial_mem_array);
}
#endif
/************************************
*special readout handling routines**
*************************************/
void set_support_abort (int sabt)
{
	support_abort = sabt;
}

int get_support_abort ()
{
	return (support_abort);
}

void set_support_readinttime (int rit)
{
	support_rit = rit;
}

int get_support_readinttime ()
{
	return (support_rit);
}

int get_actual_inttime ()
{
	return (actual_inttime);
}

int abort_read (int fd)
{
	if (get_support_abort())
		return (abort_readout (fd));
	else
		return (-EPERM);
}

int stop_all (fd)
{
int ret;

	if (ufast_read){
		stop_seq();
		return (OK);
	}
	if (detstatus == ST_EXPO)
		ret = stop_exp(fd);
	 else {
		try_stop_next_exp = 1;
		ret = STOP_TIME;
	}
	stop_seq();
	stop_coadd(fd);
	stop_sample_up_the_ramp (fd);
	return (ret);
}

int stop_exp (fd)
{
int stop_time;
int ret=OK;

	if ((stop_time = exp_prog + STOP_TIME) < get_exp_time()){
		if ((ret = set_exp_time (fd, stop_time))<0)
			return (ret);
		stop_exposure = 1;
	} else
		stop_time = get_exp_time ();

	return (stop_time);
}
 	

void stop_seq ()
{
	stop_sequence = 1;
        printf ("stop_seq\n");
}

int stop_sample_up_the_ramp (int fd)
{
	if (detstatus == ST_EXPO){
       		printf ("stoping sur ...\n");
		if (stop_hard_sur (fd) == OK){
        		stop_sur = 1;
			return (OK);
		} 
	}
       	printf ("trying to stop sur ...\n");
        try_stop_sur = 1;
	return (OK);
}



int stop_hard_sur (int fd)
{
int ret;

	if ((ret=set_num_sur (fd, 0)) < 0){
       		printf ("sur cont. stoped\n");
		return (ret);
	}
	return (OK);
}



int stop_hard_coadd (int fd)
{
int ret;

	if ((ret=set_num_coadds (fd, 1)) < 0){
       		printf ("coadd cont. stoped\n");
		return (ret);
	}
	return (OK);
}


int stop_coadd (int fd)
{

	if (detstatus == ST_EXPO){
       		printf ("stoping coadd ...\n");
		if (stop_hard_coadd (fd) == OK){
        		stop_sequence = 1;
        		stop_coadding = 1;
			return (OK);
		} 
	}
       	printf ("trying to stop coadd ...\n");
        try_stop_coadding = 1;
	return (OK);
}

int abort_current_coadd (int fd)
{
        discard_coadd = 1;
        printf ("abort_coadd\n");
        return (abort_read (fd));
}

int abort_all (int fd)
{
int ret=0;
int status;

	printf ("abort_all\n");
	if (ufast_read)
		return (stop_all(fd));

	status = get_cont_status (fd);
	if ((detstatus == ST_EXPO) && (status != cont_READOUT)) {
		printf ("aborting exposure\n");
		abort_exposure (fd);
       		discard_coadd = 1;
       		stop_sequence = 1;
	} 
	if (detstatus == ST_PAUSED)
		printf ("aborting paused exposure\n");
		abort_exposure (fd);
       		discard_coadd = 1;
       		stop_sequence = 1;
	if (detstatus == ST_READ){
		printf ("aborting readout\n");
		abort_exposure (fd);
		if ((ret=abort_read (fd) < 0))
			return (ret);
		else {
      			discard_coadd = 1;
       			stop_sequence = 1;
		}
	}
        return (OK);
}

void clear_discards()
{
	coadds_discarded = images_discarded = 0;
}

void init_cont_read ()
{
  framePtr = highmemPtr;
  framesCounter=0;
  tot_pix_rec=0;
  printf ("init_cont_read: Resetting tot_pix and framesCounter\n");
}

void clear_counters ()
{
int i;
	fmi = fmo = imi = imo = 0;
	printf ("clearing exposure counters\n");
	for (i=0; i<CIRC_BUF_SIZE+1; i++){
	            frame[i].status = FM_IDLE;
	            frame[i].ptr = 0;
	            frame[i].buf_read = 0;
	    }
}

void clear_exp_vals ()
{
	detstatus = ST_IDLE;
	exp_prog = 0;
}


int clear_mem_points()
{
        n_inipoints = 0;
        n_endpoints = 0;
        return (OK);
}

int clear_all (int err)
{
	clear_exp_vals();
	stop_sequence = 0;
	stop_coadding = 0;
	stop_sur = 0;
	try_stop_coadding = 0;
	try_stop_sur = 0;
	try_stop_next_exp = 0;
	discard_coadd = 0;
	printf ("cler_all with %d status (frame with %d)\n", err, frame[fmi].status);
	return (err);
}


int chk_buffer (void)
{
clock_t beg, end;
clock_t buf_timeout = 5;

        beg = clock ();
        beg = beg / CLOCKS_PER_SEC;
        end = beg;
        while ((next_imptr == 0) && ((end - beg) < buf_timeout)) {
                end = clock ();
                end = end/CLOCKS_PER_SEC;
        }
        if (next_imptr == 0)
                return (-ENOBUFS);

        return (OK);
}



void get_expose_status (int *ss, int *sc)
{
	*ss = stop_sequence;
	*sc = stop_coadding;
	clear_all (OK);
}

ptr_t get_ptr_val ()
{
	return (next_imptr);
}

void reset_fastptr_val(void)
{
	curr_fastptr = fastbufptr;
	fastPtrNo = 0;
}

ptr_t get_fastptr_val (void)
{
	curr_fastptr = curr_fastptr + curr_imsize;
	fastPtrNo++;
	if (curr_fastptr >= max_ptr)
		reset_fastptr_val();
	return ((ptr_t)(dptr_t) curr_fastptr);
}


ptr_t get_free_buf (int index)
{
	if (index > num_bufs)
		return (-EINVAL);
	printf ("getting buf %ld\n", (dptr_t)img_bufs[index]);
	return (img_bufs[index]);
}


ptr_t get_used_buf ()
{
int index;
        for (index=1; index<=num_bufs; index++)
                if (!img_bufs[index])
                        return (index);
        return (0);
}


void set_num_bufs (int n_ptrs)
{
int index;

	num_bufs = n_ptrs;
        for (index=1; index<=n_ptrs; index++)
                img_bufs[index] = 0;
}


void set_ptr_vals (ptr_t *ptrs, int n_ptrs)
{
int index;
ptr_t *ptr=NULL;

        num_bufs = n_ptrs;
        ptr = ptrs;
        for (index=1; index<=n_ptrs; index++){
                img_bufs[index] = *ptr;
                ptr++;
        }
}

ptr_t set_new_buf (ptr_t ptr, int index)
{
        if (index > num_bufs)
                return (-EINVAL);
        img_bufs[index] = ptr;
	printf ("new buf[%d] %ld\n", index, (dptr_t)ptr);
        return (OK);
}



void set_ptr_val (ptr_t val)
{
	next_imptr = val;
}


void set_bufs_per_frame (unsigned int val)
{
	bufs_per_frame = val;
}


int get_imstatus (int fd, ptr_t *curr_ptr, short *detstat, int *exp, int *imread, int *imdones, int *coaddsdone, int *fmstat, int *coad_disc, int *img_disc, int *actual_buf_read)
{
int bufs;

//	if ((detstatus == ST_READ) || (detstatus == ST_RESET))
//	if ((fmo > CIRC_BUF_SIZE) || (fmi > CIRC_BUF_SIZE))
//		return (-ESPIPE);
	if (frame[fmi].buf_read < 0)
		return (frame[fmi].buf_read);

	if (fmo != fmi)
		if (frame[fmo].status == FM_OK){
			frame[fmo].buf_read = bufs_per_frame;
//			printf ("forced %d in %ld\n", bufs_per_frame, frame[fmo].ptr);
		}
//	printf ("bufs_read %d %d %d\n", frame[fmi].buf_read, frame[fmo].buf_read, bufs_read);
	*actual_buf_read = frame[fmi].buf_read;
	bufs = frame[fmo].buf_read;
	*curr_ptr = frame[fmo].ptr;
	*fmstat = frame[fmo].status;
	*detstat = detstatus;
	*exp = exp_prog;
	*imread = num_reads;
	*imdones = images_done;
	*coaddsdone = coadds_done;
	*coad_disc = coadds_discarded;
	*img_disc = images_discarded;
//	printf ("fmo %d status %d ptr %d prog %d\n", fmo, frame[fmo].status, (unsigned long)frame[fmo].ptr,  frame[fmo].buf_read);	
//	printf ("ptr %ld buffs %d status %d\n", frame[fmo].ptr, frame[fmo].buf_read, frame[fmo].status);
	if (fmo < fmi)
		fmo++;
	else {
		if (fmo > fmi){
        		if (fmo >= CIRC_BUF_SIZE)
                		fmo = 0;
			else
				fmo++;
		}
	}
	return (bufs);
}


/***************************************************************************
 * return the results for the UTSHUT keyword, and two arrays: the first one
 * with the results of the read_mem_point funstion, and the other one with
 * the "ids" of every point, so the caller can associate id - result pairs
 * Basically all this is returned for every image
 ***************************************************************************/
char* get_UT (void)
{
char *timeRet;

	if (oldimages_started < images_started){
		oldimages_started++;
		timeRet = image[imo].UTShut;
//		printf ("timeRet: %s %s %d %d\n", timeRet, image[imo].UTShut, imi, imo);
        	if (imo <= imi)
               		imo++;
        	else {
               		if (imo > imi){
                       		if (imo >= CIRC_BUF_SIZE)
                               		imo = 0;
                       		else
                               		imo++;
			}
		}
	} else
		timeRet = emptyTime;
	return (timeRet);
}

#if 0
/**********************************************************************
 * Add a point (board and memory address) for being read (or write)
 * before or after an image is read. 
 *********************************************************************/
void add_mem_point (void *m_array, int where)
{
memory_t *mptr;

	printf ("entered to mem_array\n");
	mptr = (memory_t *) m_array;
	if (mptr->id != 0){
		if (where == INI_POINTS){
			IniMem_array[n_inipoints] = *mptr;
			 printf ("id: %d, board %d mem_type %d address %ld value %d action %ld status %d\n", IniMem_array[n_inipoints].id, IniMem_array[n_inipoints].board, IniMem_array[n_inipoints].mem_type, IniMem_array[n_inipoints].address, IniMem_array[n_inipoints].action, IniMem_array[n_inipoints].value, IniMem_array[n_inipoints].status);
			n_inipoints++;
		} else {
			EndMem_array[n_endpoints] = *mptr;
			 printf ("id %d, board %d mem_type %d address %ld value %d action %ld status %d\n", EndMem_array[n_endpoints].id, EndMem_array[n_endpoints].board, EndMem_array[n_endpoints].mem_type, EndMem_array[n_endpoints].address, EndMem_array[n_endpoints].action, EndMem_array[n_endpoints].value, EndMem_array[n_endpoints].status);
			n_endpoints++;
		}
	}
}
#endif
