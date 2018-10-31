#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
//#include "../../../public/c/include/GUnscrambling.h"
#include "GU_transformations.h"
#ifdef TIMEDEB
  #include <time.h>
  #include <sys/time.h>
#endif
#include <dlfcn.h>

static u32 namps=0;
static u32 ndets=0;
static amp_t amp[256];
static amp_t det[64];
static i32 PhysMap[MAX_AMPS];
i32 bytes_pp=0;
i32 detext=0;
i32 detbufsize = 0;
void *detbufptr=NULL;
void *ampbufptr=NULL;
i32 ampbufsize = 0;
i32 pix_swap=0;
i32 arrange_sections = 0;
i32 arrange_sections_left = 0;
i32 arrange_sections_right = 0;
i32 arrange_sections_upper = 0;
i32 arrange_sections_lower = 0;
void  *LocalLib=NULL;  
i32   (*GUget_left1)(); 
i32   (*GUget_left2)(); 
i32   (*GUget_left3)(); 
i32   (*GUget_right1)(); 
i32   (*GUget_right2)(); 
i32   (*GUget_right3)(); 
i32   (*GUget_lower1)(); 
i32   (*GUget_lower2)(); 
i32   (*GUget_lower3)(); 
i32   (*GUget_upper1)(); 
i32   (*GUget_upper2)(); 
i32   (*GUget_upper3)(); 
const char *dlError2;

//void *dptr_t (ptr_t inptr);
i32 GU_do_detextensions (ptr_t outbuff, i32 nelems, i32 chunkrows);
i32 GU_rearrange_sections (i32 chunkrows);

/**
* Checks dl error
**/
i32 GU_checkError2 (i32 iferr)
{
        dlError2 = dlerror ();
        if(dlError2) {
                printf ("dlerror %s\n", dlError2);
                return (-iferr);
        }
        return (OK);
}

/**
* swap bytes in the pixel if requested
**/
void GU_pix_swap (i32 flag)
{
	pix_swap = flag;
	printf ("pix_swap is now %d\n", pix_swap);
}

/**
* Sets the number of bytes per pixel
* @param[in] bytes_per_pixel number of bytes per pixel (usually 2 or 4)
**/
i32 GU_set_bytes_pp (i32 bytes_per_pixel)
{
	if (bytes_per_pixel < 0)
		return (-EINVAL);
	bytes_pp = bytes_per_pixel;
	return (OK);
}

/**
* Sets the physical map between amplifiers and output extensions
* @param[in] array array of integers with the order. The array index is the amplifier, the value the output position
* @param[in] num_elements numbe rof elements in the array
**/
i32 GU_SetPhysMap (ptr_t array, i32 num_elements)
{
i32 *arrptr;
i32 i, ele;

	arrptr = (i32 *)(dptr_t) array;
	if (arrptr == NULL)
		return (-EFAULT);
	if (namps == 0) {
		pdebug ("physMap: not initialized\n");
		return (-1);
	}
		
	for (i=0; i< namps; i++){
		if (i <= num_elements){
			ele = *arrptr;
			arrptr++;
		} else 
			ele = i;
				
		PhysMap[i] = ele;
	}

	return (OK);
}

/**
* Releases detectors buffer
**/
void GU_releasedetbuf (void)
{
	if (detbufptr != NULL){
		free (detbufptr);
		printf ("det buf released\n");
	}
	detbufsize = 0;
	detbufptr = NULL;
}
	

/**
* Get detector buffer pointer
**/
i32 GU_getdetbuf (void)
{
i32 rsize;

	if ((det[0].ncols == 0) || (bytes_pp == 0))
        	return (-1);
        rsize = det[0].ncols * det[0].nrows * bytes_pp * ndets;
        if (detbufsize < rsize) {
		GU_releasedetbuf();
                detbufptr = malloc (rsize);
                if (detbufptr == NULL)
                        return (-ENOMEM);
                detbufsize = rsize;
                printf ("det buffer acquired (ptr %ld size %d\n", (ulong)(ptr_t)(dptr_t) detbufptr, detbufsize);
	}
	return (OK);
}

/**
* Releases amplifiers buffer
**/
void GU_release_ampbuf (void)
{
        if (ampbufptr != NULL){
                free (ampbufptr);
                printf ("amp buf released\n");
        }
        ampbufsize = 0;
        ampbufptr = NULL;
}

/**
* Opens reference to unscrambling algorithm
* @param[in] algorpath name of the algorithm to be used to unscramble
**/
i32 GU_dlopen (char *algorpath)
{
i32 ret;

       if (LocalLib != NULL){
       		LocalLib = dlopen (algorpath, RTLD_LAZY);
       		if ((ret = GU_checkError2 (ELIBACC))){
                	printf ("error opening %d\n", ret);
              		return (ret);
        	}
	}
	return (OK);
}

/**
* Closes unscrambling algor
**/
void GU_dlclose (void)
{
        if (LocalLib != NULL){
                dlclose (LocalLib);
                LocalLib = NULL;
        }
}

/**
* Allocates amplifiers buffer based on amplifier structure
**/
i32 GU_get_ampbuf (void)
{
i32 rsize;

//	printf ("cols %d bytes_pp %d\n", amp[0].ncols, bytes_pp);
        if ((amp[0].ncols == 0) || (bytes_pp == 0))
                return (-1);
        rsize = amp[0].ncols * amp[0].nrows * bytes_pp;
        if (ampbufsize < rsize) {
                GU_release_ampbuf();
                ampbufptr = malloc (rsize);
                if (ampbufptr == NULL)
                        return (-ENOMEM);
                ampbufsize = rsize;
                printf ("amp buffer acquired (ptr %lu, size %d\n", (ulong)(ptr_t)(dptr_t) ampbufptr, ampbufsize);
        }
        return (OK);
}

/**
* Arranges sections order. This is called dinamically. See GU_call_set_order_left()
* Based on the specified directive it will fill the algor to use. 
**/
i32 GU_set_arrange_sections_left (char *algorpath, char *left1, char *left2, char *left3)
{
i32 ret;
char fname[80];

	if ((left1 == NULL) || (left2 == NULL) || (left3 == NULL))
		return (-EINVAL);

	if ((strncmp (left1, "none", 4) != 0)){
		if ((ret=GU_dlopen (algorpath))<0)
			return (ret);
		sprintf (fname, "GU_copy_%s_left", left1);
                GUget_left1 = dlsym (LocalLib, fname);
                if ((ret = GU_checkError2 (ELIBACC)))
                        return (ret);
		sprintf (fname, "GU_copy_%s_left", left2);
                GUget_left2 = dlsym (LocalLib, fname);
                if ((ret = GU_checkError2 (ELIBACC)))
                        return (ret);
		sprintf (fname, "GU_copy_%s_left", left3);
                GUget_left3 = dlsym (LocalLib, fname);
                if ((ret = GU_checkError2 (ELIBACC)))
                        return (ret);
                printf ("arrange_sections_left is now %s %s %s\n", left1, left2, left3);
		arrange_sections_left = 1;
	} else
                arrange_sections_left = 0;

	arrange_sections = arrange_sections_left || arrange_sections_right;
	if (!arrange_sections){
		GU_dlclose ();
		GU_release_ampbuf ();
	}
	return (OK);
}

/**
* Arranges sections order. This is called dinamically. See GU_call_set_order_right()
**/
i32 GU_set_arrange_sections_right (char *algorpath, char *right1, char *right2, char *right3)
{
i32 ret;
char fname[80];

        if ((right1 == NULL) || (right2 == NULL) || (right3 == NULL))
                return (-EINVAL);

        if ((strncmp (right1, "none", 4) != 0)){
               if ((ret=GU_dlopen (algorpath))<0)
                        return (ret);
                sprintf (fname, "GU_copy_%s_right", right1);
                GUget_right1 = dlsym (LocalLib, fname);
                if ((ret = GU_checkError2 (ELIBACC)))
                        return (ret);
                sprintf (fname, "GU_copy_%s_right", right2);
                GUget_right2 = dlsym (LocalLib, fname);
                if ((ret = GU_checkError2 (ELIBACC)))
                        return (ret);
                sprintf (fname, "GU_copy_%s_right", right3);
                GUget_right3 = dlsym (LocalLib, fname);
                if ((ret = GU_checkError2 (ELIBACC)))
                        return (ret);
                printf ("arrange_sections_right is now %s %s %s\n", right1, right2, right3);
                arrange_sections_right = 1;
        } else
                arrange_sections_right = 0;

        arrange_sections = arrange_sections_left || arrange_sections_right;
	if (!arrange_sections){
		GU_dlclose ();
		GU_release_ampbuf ();
	}
        return (OK);
}


/**
* Arranges sections order. This is called dinamically. See GU_call_set_order_lower()
**/
i32 GU_set_arrange_sections_lower (char *algorpath, char *low1, char *low2, char *low3)
{
i32 ret;
char fname[80];

        if ((low1 == NULL) || (low2 == NULL) || (low3 == NULL))
                return (-EINVAL);

        if ((strncmp (low1, "ynone", 5) != 0)){
               if ((ret=GU_dlopen (algorpath))<0)
                        return (ret);
                sprintf (fname, "GU_copy_%s_low", low1);
                GUget_lower1 = dlsym (LocalLib, fname);
                if ((ret = GU_checkError2 (ELIBACC)))
                        return (ret);
                sprintf (fname, "GU_copy_%s_low", low2);
                GUget_lower2 = dlsym (LocalLib, fname);
                if ((ret = GU_checkError2 (ELIBACC)))
                        return (ret);
                sprintf (fname, "GU_copy_%s_low", low3);
                GUget_lower3 = dlsym (LocalLib, fname);
                if ((ret = GU_checkError2 (ELIBACC)))
                        return (ret);
                printf ("arrange_sections_low is now %s %s %s\n", low1, low2, low3);
                arrange_sections_lower = 1;
        } else
                arrange_sections_lower = 0;

        arrange_sections = arrange_sections_left || arrange_sections_right || arrange_sections_lower || arrange_sections_upper;

        if (!arrange_sections){
                GU_dlclose ();
                GU_release_ampbuf ();
        }
        return (OK);
}

/**
* Arranges sections order. This is called dinamically. See GU_call_set_order_upper()
**/
i32 GU_set_arrange_sections_upper (char *algorpath, char *upp1, char *upp2, char *upp3)
{
i32 ret;
char fname[80];

        if ((upp1 == NULL) || (upp2 == NULL) || (upp3 == NULL))
                return (-EINVAL);

        if ((strncmp (upp1, "ynone", 5) != 0)){
               if ((ret=GU_dlopen (algorpath))<0)
                        return (ret);
                sprintf (fname, "GU_copy_%s_upp", upp1);
                GUget_upper1 = dlsym (LocalLib, fname);
                if ((ret = GU_checkError2 (ELIBACC)))
                        return (ret);
                sprintf (fname, "GU_copy_%s_upp", upp2);
                GUget_upper2 = dlsym (LocalLib, fname);
                if ((ret = GU_checkError2 (ELIBACC)))
                        return (ret);
                sprintf (fname, "GU_copy_%s_upp", upp3);
                GUget_upper3 = dlsym (LocalLib, fname);
                if ((ret = GU_checkError2 (ELIBACC)))
                        return (ret);
                printf ("arrange_sections_upp is now %s %s %s\n", upp1, upp2, upp3);
                arrange_sections_upper = 1;
        } else
                arrange_sections_upper = 0;

        arrange_sections = arrange_sections_left || arrange_sections_right || arrange_sections_lower || arrange_sections_upper;

        if (!arrange_sections){
                GU_dlclose ();
                GU_release_ampbuf ();
        }
        return (OK);
}

/**
* Returns index to specified detector number
* @param[in] detno detector number inside detectors structure
**/
i32 GU_getdet (i32 detno)
{
i32 index;

	for (index=0; index<ndets; index++)
		if (det[index].detno == detno){
			return (index);
		}
	printf ("**** Warning: detector %d not found!!! ***\n", detno);
	return (-1);
}

/**
* Enables or disables unscrambling per detector instead of per amplifier. This causes the output 
unscrambled buffer to be ordered by detector instead as per amplifer
* @params[in] flag enable(1) or disable(0) per detector unscrambling
**/
i32 GU_det_ext (i32 flag)
{
	detext = flag;
	printf ("detext is now %d\n", detext);
#if 1
	if (flag){
		GU_getdetbuf ();
	} else {
		GU_releasedetbuf ();
	}
#endif
	return (OK);
}

/**
* Adds a new detector into structure. See GU_call_add_det()
**/
i32 GU_add_det (i32 xstart, i32 precols, i32 datacols, i32 postcols, i32 ystart, i32 prerows, i32 datarows, i32 postrows, i32 nxamps, i32 nyamps, i32 discard, i32 detno)
{
	det[ndets].discard = discard;
        det[ndets].ncols = datacols + (precols + postcols) * nxamps;
        det[ndets].nrows = datarows + (prerows + postrows) * nyamps;
        det[ndets].xstart = xstart;
        det[ndets].ystart = ystart;
        det[ndets].detno = detno;
	pdebug ("det %d, xstart %d, ystart %d, ncols %d, nrows %d discard %d detno %d\n", ndets, det[ndets].xstart, det[ndets].ystart, det[ndets].ncols, det[ndets].nrows, det[ndets].discard, det[ndets].detno);
	ndets++;
	return (OK);
}


/**
* Adds a new detector into structure. See GU_call_add_amp()
**/
i32 GU_add_amp (i32 xstart, i32 precols, i32 datacols, i32 postcols, i32 ystart, i32 prerows, i32 datarows, i32 postrows, i32 xdetstart, i32 ydetstart, i16 type, i32 rotate, i16 flip, i32 extension, i32 detno, i32 ampno, i32 discard)
{
div_t	angle;
int detindex;

	amp[namps].discard = discard;
	amp[namps].ncols = precols + datacols + postcols;
	amp[namps].nrows = prerows + datarows + postrows;
	amp[namps].xstart = xstart;
	amp[namps].ystart = ystart;
	amp[namps].precols = precols;
	amp[namps].postcols = postcols;
	amp[namps].prerows = prerows;
	amp[namps].postrows = postrows;
	amp[namps].datacols = datacols;
	amp[namps].datarows = datarows;
	amp[namps].xdetstart = xdetstart;
	amp[namps].ydetstart = ydetstart;
	amp[namps].detno = detno;
	detindex = GU_getdet (detno);
	if ((detindex <= ndets) && (detindex >= 0)) {
		if (amp[namps].type & LOWER) 
			det[detindex].type = LL;
		else
			det[detindex].type = UL;
	}
	amp[namps].ampno = ampno;
	amp[namps].type = (0x8 >> type);	/*maps 0 --> 0x8 and 3 -> 0x1 */

/**************************************************************************************//**
 * First, convert any angle to a value [0, 359] degrees.
 * Second, the only allowable angles are multiple of 90 (90, 180, 270).
 * Third,  then rotation on 180 degrees can be avoided by changing the amplifier definition
 *  with the following rule:
 * Change from Lower to Upper anf from Righr to Left (and vice-versa), which means, the
 * opposite, diagonal type
 * **************************************************************************************/
	angle = div (rotate, 360);
	amp[namps].rotate = angle.rem;

	angle = div (amp[namps].rotate, 90);
	if (angle.rem != 0) {
		amp[namps].rotate = 0;
		pdebug ("add: WARNING, rotation angle %d not allowed: discarded\n", rotate);
	}

	if (abs(amp[namps].rotate) == 180) {
		amp[namps].rotate = NOROT;
		if (amp[namps].type & DIAGR)	
			amp[namps].type = (~amp[namps].type) & DIAGR;
		else
			amp[namps].type = (~amp[namps].type) & DIAGL;
	}

/***************************************************************************************//**
 *Now, all flips can be avoided just changing the amplifiers definitions using this rule:
 * For Horizontal Flips, a Lower amplifiers change to an Upper amplifier (and vice-versa)
 * For Vertical Flips, a Right amplifier changes to a Left amplifier (and vice-versa)
 * This implies that, for example, a LL flipped vertically is a LR, and flipped horizontally
 * is an UL
 *******************************************************************************************/
	if (flip == FLIPVERT) {
		if (amp[namps].type & LOWER) {
			amp[namps].type = (~amp[namps].type) & LOWER;
		} else  
			amp[namps].type = (~amp[namps].type) & UPPER;
	} else if (flip == FLIPHORI) {
		if (amp[namps].type & LEFT)
			 amp[namps].type = (~amp[namps].type) & LEFT;
		else
			amp[namps].type = (~amp[namps].type) & RIGHT;
	}
	amp[namps].flip = NOFLIP;
	PhysMap[namps] = extension;
	namps++;
	return (OK);
}

/**
* Gets amplifiers structure pointer
**/
ptr_t GU_get_amps_ptr ()
{
	if (detext)
		return ((ptr_t)(dptr_t)det);
	else 
		return ((ptr_t)(dptr_t)amp);
}

/**
* Clears amplifiers structure
**/
i32 GU_clear_amps (void)
{
i32 i;

	for (i=0; i<MAX_AMPS; i++) {
		amp[i].ncols = amp[i].nrows = 0;
		PhysMap[i] = i;
	}
	namps = 0;
	ndets = 0;
	GU_releasedetbuf();
	return (OK);
}

#ifdef _DEBUG_
/**
* Shows (prints) amplifier structure. Only if debug is enabled
**/
i32 GU_show_amps (void)
{
i32 i;
char strtype[3];
char strflip[5];

	pdebug ("number of amplifiers: %d\n", namps);

	for (i=0; i<namps; i++){
//		PhysMap [i] = i;
		if (amp[i].type == 0x8)
			sprintf (strtype, "%s", "LL");
		else if (amp[i].type == 0x4)
			sprintf (strtype, "%s", "LR");
		else if (amp[i].type == 0x2)
			sprintf (strtype, "%s", "UL");
		else sprintf (strtype, "%s", "UR");
		if (amp[i].flip == 0)
			sprintf (strflip, "%s", "none");
		else
			sprintf (strflip, "%s", "yes");
		pdebug ("amp %d: xstart %d, ncols %d, ystart %d, nrows %d xdetstart %d ydetstart %d, type %s(%d), rotate %d, flip %s(%d)-->extension %d, discard %d, precols %d, datacols %d, postcols %d, prerows %d, datarows %d, postrows %d\n", i, amp[i].xstart, amp[i].ncols, amp[i].ystart, amp[i].nrows, amp[i].xdetstart, amp[i].ydetstart, strtype, amp[i].type, amp[i].rotate, strflip, amp[i].flip, PhysMap[i]+1, amp[i].discard, amp[i].precols, amp[i].datacols, amp[i].postcols, amp[i].prerows, amp[i].datarows, amp[i].postrows);
	}
	return (OK);
}
#endif

/**
* Unscrambling routine called for SHORT datatype (16 bits per pixel, integer). Selected automatically (dinamically) when the datatype is set in the routine GU_compute()
* If detector extenions are selected (see GU_det_ext()) then it calls automaticall GU_do_detextensions()
* @param[in] rptr pointer to raw (incoming) pixels. This is the buffer where the scrambled pixels are.
* @param[in] imageptr pointer to output buffer. This is the bufefr where the unscrambled data will be written
* @param[in] nelems number of elements (pixels) in the raw buffer
* @param[out] *err status out
**/
ptr_t GU_SHORT (ptr_t rptr, ptr_t imageptr, i32 nelems, i32 *err)
{
u32 procpix, endline;
i16 *dataptr;
i16 *rawptr;
i16 *imptr;
u32 ptr;
u32 ampdata;
register u32 y,x,i;
i32 a, chunkrows=amp[0].nrows;

	rawptr = (i16 *) (dptr_t) rptr;
	imptr = (i16 *) (dptr_t) imageptr;
	if ((imptr == NULL) || (rawptr == NULL)){
		pdebug ("error bad pointer %ld %ld\n", (ulong)rptr, (ulong)imageptr);
//		*err= -EFAULT; 
		return (0);
	}

	ampdata = nelems/namps;
	pdebug ("ampdata %d, outptr 0x%lx, rawptr 0x%lx, swap %d\n", ampdata, (ulong)imageptr, (ulong)rptr, pix_swap);
       if ((namps == 1) && (pix_swap)){
                i32 tmp;
                dataptr = rawptr;
                for (y=0; y<ampdata; y=y+2){
                     tmp = *(dataptr+y);
                     *(dataptr+y) = *(dataptr+y+1);
                     *(dataptr+y+1) = tmp;
                }
        }
	for (i=0; i<namps; i++){
		a = PhysMap[i];
		dataptr = imptr + a*ampdata;
		amp[a].dataptr = (dptr_t) dataptr;
		chunkrows = (i32) ((f32)ampdata/amp[a].ncols);
		ptr = i; procpix = 0;
		pdebug ("unscrambling i16 image from 0x%lx to 0x%lx, element= %d pixels (amplifiers %lu)\n", (ulong)rptr, amp[a].dataptr, nelems, (ulong)amp);  
		switch (amp[a].type) {
			case LL:{
				for (y=0; y<ampdata; y++){
					*(dataptr+procpix) = *(rawptr+ptr);
					ptr+=namps;
					procpix++;
				}} break;	

			case UR:{
				for (y=0; y<ampdata; y++){
					*(dataptr+ampdata-procpix-1) = *(rawptr+ptr);
					ptr+=namps;
					procpix++;
				}} break;
					
			case LR:{
				for (y=0; y<chunkrows; y++){
					endline = (y+1)*amp[a].ncols - 1 + procpix;
					for (x=0; x<amp[a].ncols; x++){
						*(dataptr+endline-procpix) = *(rawptr+ptr);
#if 0
						if ((y==0) && (x<4)){
							printf ("UNSC UR %d to position %ld\n", *(dataptr+endline-procpix), (ulong) (endline-procpix));
						}
#endif
						ptr+=namps;
						procpix++;
					}
				}} break;
			default:{			/* UL */
				if (amp[a].type & UL) {
					for (y=0; y<chunkrows; y++){
						endline = (y+1)*amp[a].ncols - 1 + procpix;
						for (x=0; x<amp[a].ncols; x++){
							*(dataptr+ampdata-endline+procpix-1) = *(rawptr+ptr);
#if 0
							if ((y==0) && (x<4)){
								printf ("UNSC UL %d to position %ld\n", *(dataptr+ampdata-endline+procpix-1), (ulong) (ampdata-endline+procpix-1));
							}
							if ((y==chunkrows-1) && (x==0))
								printf ("UNSC UL %d to position %ld\n", *(dataptr+ampdata-endline+procpix-1), (ulong) (ampdata-endline+procpix-1));
#endif
							ptr+=namps;
							procpix++;
						}
					}
				}}
		}
		if (amp[a].rotate)
			RI_SHORT ((i16 *)amp[a].dataptr, amp[a].ncols, chunkrows, amp[a].rotate);
	}
	bytes_pp = 2;
	pdebug ("GU_SHORT: finished amplifier unscrambling");

	if (arrange_sections)
		GU_rearrange_sections (chunkrows);

        if (detext) {
                GU_do_detextensions ((ptr_t)(dptr_t)detbufptr, nelems, chunkrows);
                return ((ptr_t)(dptr_t)det);
        } else
                return ((ptr_t)(dptr_t)amp);
}					

/**
* Unscrambling routine called for LONG datatype (32 bits per pixel, integer). Selected automatically (dinamically) when the datatype is set in the routine GU_compute()
* If detector extenions are selected (see GU_det_ext()) then it calls automaticall GU_do_detextensions()
* @param[in] rptr pointer to raw (incoming) pixels. This is the buffer where the scrambled pixels are.
* @param[in] imageptr pointer to output buffer. This is the bufefr where the unscrambled data will be wri
tten
* @param[in] nelems number of elements (pixels) in the raw buffer
* @param[out] *err status out
**/
ptr_t GU_LONG (ptr_t rptr, ptr_t imageptr, i32 nelems, i32 *err)
{
u32 procpix=0, endline;
i32 *dataptr;
i32 *rawptr;
i32 *imptr;
u32 ptr;
u32 ampdata;
register u32 y,x,i;
i32 a;
i32 chunkrows=amp[0].nrows;

	rawptr = (i32 *)(dptr_t) rptr;
	imptr = (i32 *)(dptr_t) imageptr;
	if ((imptr == NULL) || (rawptr == NULL)) {
		*err= -EFAULT;
		return (0);
	}

	pdebug ("GU_LONG: amplifiers %ld\n", (ulong)(ptr_t) amp);
	ampdata = nelems/namps;
       if ((namps == 1) && (pix_swap)){
                i32 tmp;
                dataptr = rawptr;
                for (y=0; y<ampdata; y=y+2){
                     tmp = *(dataptr+y);
                     *(dataptr+y) = *(dataptr+y+1);
                     *(dataptr+y+1) = tmp;
                }
        }
	for (i=0; i<namps; i++){
		a = PhysMap[i];
		dataptr = imptr + a*ampdata;
		amp[a].dataptr = (ptr_t)(dptr_t) dataptr;
		pdebug ("GU_LONG: amp[%d].dataprt = %ld\n",a, amp[a].dataptr);
		chunkrows = (int) ((double)ampdata/amp[a].ncols);
		ptr = i; procpix = 0;
		switch (amp[a].type) {
			case LL:{
				for (y=0; y<ampdata; y++){
					*(dataptr+procpix) = *(rawptr+ptr);
					ptr+=namps;
					procpix++;
				}} break;	

			case UR:{
				for (y=0; y<ampdata; y++){
					*(dataptr+ampdata-procpix-1) = *(rawptr+ptr);
					ptr+=namps;
					procpix++;
				}} break;
					
			case LR:{
				for (y=0; y<chunkrows; y++){
					endline = (y+1)*amp[a].ncols - 1 + procpix;
					for (x=0; x<amp[a].ncols; x++){
						*(dataptr+endline-procpix) = *(rawptr+ptr);
						ptr+=namps;
						procpix++;
					}
//					pdebug ("GU_LR: endline %d procpix %d buptr %ld data %d\n", endline, procpix, (ptr_t)  (dataptr+endline-procpix-1), *(dataptr+endline-procpix-1));
				}} break;
			default:{			/* UL */
				if (amp[a].type & UL) {
					for (y=0; y<chunkrows; y++){
						endline = (y+1)*amp[a].ncols - 1 + procpix;
						for (x=0; x<amp[a].ncols; x++){
							*(dataptr+ampdata-endline+procpix-1) = *(rawptr+ptr);
							ptr+=namps;
							procpix++;
						}
					}
				}}
		}
		if (amp[a].rotate) {
			RI_LONG ((i32 *)amp[a].dataptr, amp[a].ncols, chunkrows, amp[a].rotate);
		}
	}
	bytes_pp = 4;

        if (arrange_sections)
                GU_rearrange_sections (chunkrows);

	if (detext) {
		GU_do_detextensions ((ptr_t)(dptr_t)detbufptr, nelems, chunkrows);
		return ((ptr_t)(dptr_t)det);
	} else	
		return ((ptr_t)(dptr_t)amp);
}					

/**
* Unscrambling routine called for FLOAT datatype (32 bits per pixel, float). Selected automatically (dinamically) when the datatype is set in the routine GU_compute()
* If detector extenions are selected (see GU_det_ext()) then it calls automaticall GU_do_detextensions()
* @param[in] rptr pointer to raw (incoming) pixels. This is the buffer where the scrambled pixels are.
* @param[in] imageptr pointer to output buffer. This is the bufefr where the unscrambled data will be wri
tten
* @param[in] nelems number of elements (pixels) in the raw buffer
* @param[out] *err status out
**/
ptr_t GU_FLOAT (ptr_t rptr, ptr_t imageptr, i32 nelems, i32 *err)
{
u32 procpix, endline;
f32 *dataptr;
f32 *rawptr;
f32 *imptr;
u32 ptr;
u32 ampdata;
register u32 y,x,i;
i32 a;
i32 chunkrows=amp[0].nrows;

	rawptr = (f32 *)(dptr_t) rptr;
	imptr = (f32 *)(dptr_t) imageptr;
	if ((imptr == NULL) || (rawptr == NULL)){
		*err=-EFAULT; 
		return (0);
	}

	ampdata = nelems/namps;
       if ((namps == 1) && (pix_swap)){
                i32 tmp;
                dataptr = rawptr;
                for (y=0; y<ampdata; y=y+2){
                     tmp = *(dataptr+y);
                     *(dataptr+y) = *(dataptr+y+1);
                     *(dataptr+y+1) = tmp;
                }
        }
	for (i=0; i<namps; i++){
		a = PhysMap[i];
		dataptr = imptr + a*ampdata;
		amp[a].dataptr = (ptr_t)(dptr_t) dataptr;
		chunkrows = (i32) ((f32)ampdata/amp[a].ncols);
		ptr = i; procpix = 0;
		switch (amp[a].type) {
			case LL:{
				for (y=0; y<ampdata; y++){
					*(dataptr+procpix) = *(rawptr+ptr);
					ptr+=namps;
					procpix++;
				}} break;	

			case UR:{
				for (y=0; y<ampdata; y++){
					*(dataptr+ampdata-procpix-1) = *(rawptr+ptr);
					ptr+=namps;
					procpix++;
				}} break;
					
			case LR:{
				for (y=0; y<chunkrows; y++){
					endline = (y+1)*amp[a].ncols - 1 + procpix;
					for (x=0; x<amp[a].ncols; x++){
						*(dataptr+endline-procpix) = *(rawptr+ptr);
						ptr+=namps;
						procpix++;
					}
				}} break;
			default:{			/* UL */
				if (amp[a].type & UL) {
					for (y=0; y<chunkrows; y++){
						endline = (y+1)*amp[a].ncols - 1 + procpix;
						for (x=0; x<amp[a].ncols; x++){
							*(dataptr+ampdata-endline+procpix-1) = *(rawptr+ptr);
							ptr+=namps;
							procpix++;
						}
					}
				}}
		}
		if (amp[a].rotate)
			RI_FLOAT ((f32 *)amp[a].dataptr, amp[a].ncols, chunkrows, amp[a].rotate);
	}
	bytes_pp = 4;
        if (arrange_sections)
                GU_rearrange_sections (chunkrows);

        if (detext) {
                GU_do_detextensions ((ptr_t)(dptr_t)detbufptr, nelems, chunkrows);
                return ((ptr_t)(dptr_t)det);
        } else
                return ((ptr_t)(dptr_t)amp);
}					


/**
* Unscrambling routine when unscrambling is disabled. It basically copies the raw data into the amplifers data without changes
* @param[in] rptr pointer to raw (incoming) pixels. This is the buffer where the scrambled pixels are.
* @param[in] imageptr pointer to output buffer. This is the bufefr where the unscrambled data will be wri
tten
* @param[in] nelems number of elements (pixels) in the raw buffer
* @param[out] *err status out
**/
ptr_t GU_NONE (ptr_t rptr, ptr_t imageptr, i32 nelems, i32 *err)
{
i32 i, ampdata,d, detdata;
void *dataptr;
void *imptr;
i32 dused[64];

	printf ("GU_algor: unscrambling is NONE\n");
	if ((rptr == 0) || (imageptr == 0)) {
		*err = -EFAULT;
		return (0);
	}

	imptr = (void *)(dptr_t) imageptr;
	memcpy ((void *)(dptr_t)imageptr, (void *)(dptr_t)rptr, nelems * bytes_pp);
	if (!detext) {
		ampdata = nelems/namps;
		for (i=0; i<namps; i++){
			dataptr = imptr + i * bytes_pp * ampdata;
			amp[i].dataptr = (ptr_t)(dptr_t) dataptr;
		}
		return ((ptr_t)(dptr_t) amp);
	} else {
		for (i=0; i<ndets; i++)
			dused[i]=0;
		detdata = nelems / ndets;
		pdebug ("nelems %d ndets %d\n", nelems, ndets);
        	for (i=0; i<namps; i++){
			d = GU_getdet (amp[i].detno);
			if (d < 0)
				return (-ENXIO);
			if (dused[d] != 0)
				continue;
			dused[d] = 1;
    	            	det[d].dataptr = (ptr_t)(dptr_t) (imptr + d*(detdata * bytes_pp));
			pdebug ("%lu copying data (%d) for det %d [%d:%d,%d:%d]\n", det[d].dataptr, detdata, d, det[d].xstart, det[d].ncols + det[d].xstart - 1, det[d].ystart, det[d].nrows + det[d].ystart - 1);
		}
		return ((ptr_t)(dptr_t) det);
	}
}

	

/**
* Adds an the image in the specified subimage buffer in the specified position in the output buffer. The position here means geometryc position rather than index position. This is used, for example, to compose detector data (when detextor extensions are enabled)
* @param[in] genptr pointer to general (complete, output) image buffer
* @param[in] subimptr pointer to subimage to be added
* @param[in] subxstart lower left x position of subimage in complete image (genptr)
* @param[in] subystart lower left y position of subimage in complete image (genptr)
* @param[in] subcols number of columns in subimage
* @param[in] subrows number of rows in subimage
* @param[in] totalcols number of cols in complete (general) image
**/
i32 GU_add_subimage (ptr_t genptr, ptr_t subimptr, i32 subxstart, i32 subystart, i32 subcols, i32 subrows, i32 totalcols)
{
i32 y, subyend;
void *imptr;
void *subptr;
void *currptr;
                                                                                             
        imptr = (void *)(dptr_t)genptr;
        subptr = (void *)(dptr_t)subimptr;
        if ((imptr == NULL) || (subptr == NULL))
                return (-EFAULT);
        subyend = subystart + subrows - 1;
        for (y=subystart-1; y<subyend ; y++) {
                currptr = imptr + (y*totalcols + (subxstart-1))*bytes_pp;
                memcpy (currptr, subptr, subcols*bytes_pp);
                subptr += subcols*bytes_pp;
        }
        return (OK);
}
  
/**
* Copy image from inbuff to outbuff
* @param[in] output buffer pointer
* @param[in] inbuff input buffer pointer
* @param[in] totcols number of columns in input image
**/                                                                                           
i32 GU_copy_image (long outbuff, long inbuff, i32 totcols)
{
i32 i, ret;
i32 sxstart;

        printf ("GU_copy_image: getting image from %ld to %ld (namps %d, bytes_pp %d, totcols %d)\n", amp[0].dataptr, outbuff, namps, bytes_pp, totcols);
        if (namps == 0)
                return (-EFAULT);

	if (bytes_pp <=0)
		return (-EFAULT);

        for (i=0; i<namps; i++) {
		if (amp[i].rotate == 90)
			sxstart = amp[i].xstart + amp[i].nrows;
		else
			sxstart = amp[i].xstart;
                if ((ret=GU_add_subimage ((ptr_t) outbuff, amp[i].dataptr, sxstart, amp[i].ystart, amp[i].ncols, amp[i].nrows, totcols)) < 0)
                        return (ret);
	}
        return (OK);
}

ptr_t GU_get_raw_ptr (i32 *namplif)
{
	*namplif = namps;
	return ((ptr_t)amp[0].dataptr);
}

/*********************************************************/
/*routines for rearranging sections of the amplifier data*/
/*********************************************************/

/**
* Copies prescan data from left amplifiers
* @param[in] ptr image buffer pointer
* @param[in] a amplifier index
* @param[in] row row number
* @param[in,out] *nbytes number of bytes to copy
*/
void GU_copy_pre_left (void *ptr, i32 a, i32 row, i32 *nbytes)
{
    *nbytes = amp[a].precols*bytes_pp;
    memcpy (ptr, (void *) (amp[a].dataptr + (row*amp[a].ncols)*bytes_pp), *nbytes);
//    if (row==0)
//      printf ("copied row %d, nbytes %ld to ptr 0x%lx\n", row, nbytes, (ptr_t) currptr);
}

/**
* Copies prescan data from right amplifiers
* @param[in] ptr image buffer pointer
* @param[in] a amplifier index
* @param[in] row row number
* @param[in,out] *nbytes number of bytes to copy
*/
void GU_copy_pre_right (void *ptr, i32 a, i32 row, i32 *nbytes)
{
    *nbytes = amp[a].precols*bytes_pp;
    memcpy (ptr, (void *) (amp[a].dataptr + (row*amp[a].ncols + amp[a].postcols + amp[a].datacols)*bytes_pp), *nbytes);
}

/**
* Copies postscan (overscan) data from left amplifiers
* @param[in] ptr image buffer pointer
* @param[in] a amplifier index
* @param[in] row row number
* @param[in,out] *nbytes number of bytes to copy
*/
void GU_copy_pos_left (void *ptr, i32 a, i32 row, i32 *nbytes)
{
      *nbytes = amp[a].postcols*bytes_pp;
      memcpy (ptr, (void *)(amp[a].dataptr + (row*amp[a].ncols + amp[a].precols + amp[a].datacols)*bytes_pp), *nbytes);
//    if (row==0)
//      printf ("copied row %d, nbytes %ld to ptr 0x%lx\n", row, nbytes, (ptr_t) currptr);
}	

/**
* Copies postscan (overscan) data from right amplifiers
* @param[in] ptr image buffer pointer
* @param[in] a amplifier index
* @param[in] row row number
* @param[in,out] *nbytes number of bytes to copy
*/
void GU_copy_pos_right (void *ptr, i32 a, i32 row, i32 *nbytes)
{
      *nbytes = amp[a].postcols*bytes_pp;
      memcpy (ptr, (void *)(amp[a].dataptr + (row*amp[a].ncols)*bytes_pp), *nbytes);
}	

/**
* Copies data pixels from left amplifiers
* @param[in] ptr image buffer pointer
* @param[in] a amplifier index
* @param[in] row row number
* @param[in,out] *nbytes number of bytes to copy
*/
void GU_copy_dat_left (void *ptr, i32 a, i32 row, i32 *nbytes)
{
   *nbytes = amp[a].datacols*bytes_pp;
   memcpy (ptr, (void *)(amp[a].dataptr + (row*amp[a].ncols + amp[a].precols)*bytes_pp), *nbytes);
//    if (row==0)
//      printf ("copied row %d, nbytes %ld to ptr 0x%lx\n", row, nbytes, (ptr_t) currptr);
}

/**
* Copies data pixels from right amplifiers
* @param[in] ptr image buffer pointer
* @param[in] a amplifier index
* @param[in] row row number
* @param[in,out] *nbytes number of bytes to copy
*/
void GU_copy_dat_right (void *ptr, i32 a, i32 row, i32 *nbytes)
{
   *nbytes = amp[a].datacols*bytes_pp;
   memcpy (ptr, (void *)(amp[a].dataptr + (row*amp[a].ncols + amp[a].postcols)*bytes_pp), *nbytes);
}

/**
* Copies y prescan data  from lower amplifiers
* @param[in] ptr image buffer pointer
* @param[in] a amplifier index
* @param[in] row row number
* @param[in,out] *nbytes number of bytes to copy
*/
void GU_copy_ypre_low (void *ptr, i32 a, i32 *nbytes)
{
   *nbytes = amp[a].prerows*amp[a].ncols*bytes_pp;
   memcpy (ptr, (void *)(amp[a].dataptr), *nbytes);
}

/**
* Copies y prescan data  from upper amplifiers
* @param[in] ptr image buffer pointer
* @param[in] a amplifier index
* @param[in] row row number
* @param[in,out] *nbytes number of bytes to copy
*/
void GU_copy_ypre_upp (void *ptr, i32 a, i32 *nbytes)
{
   *nbytes = amp[a].prerows*amp[a].ncols*bytes_pp;
   memcpy (ptr, (void *)(amp[a].dataptr + bytes_pp*(amp[a].postrows + amp[a].datarows)*amp[a].ncols), *nbytes);
}

/**
* Copies y postcan (y overscan) data  from lower amplifiers
* @param[in] ptr image buffer pointer
* @param[in] a amplifier index
* @param[in] row row number
* @param[in,out] *nbytes number of bytes to copy
*/
void GU_copy_ypos_low (void *ptr, i32 a, i32 *nbytes)
{
   *nbytes = amp[a].postrows*amp[a].ncols*bytes_pp;
   memcpy (ptr, (void *)(amp[a].dataptr + bytes_pp*(amp[a].prerows + amp[a].datarows)*amp[a].ncols), *nbytes);
//   printf ("nbytes %ld, pix[0] %d [1] %d\n", *nbytes, *((u16 *)ptr), *((u16 *)(ptr+1)));
}

/**
* Copies y postcan (y overscan) data  from upper amplifiers
* @param[in] ptr image buffer pointer
* @param[in] a amplifier index
* @param[in] row row number
* @param[in,out] *nbytes number of bytes to copy
*/
void GU_copy_ypos_upp (void *ptr, i32 a, i32 *nbytes)
{
   *nbytes = amp[a].postrows*amp[a].ncols*bytes_pp;
   memcpy (ptr, (void *)(amp[a].dataptr), *nbytes);
}

/**
* Copies data  from lower amplifiers
* @param[in] ptr image buffer pointer
* @param[in] a amplifier index
* @param[in] row row number
* @param[in,out] *nbytes number of bytes to copy
*/
void GU_copy_ydat_low (void *ptr, i32 a, i32 *nbytes)
{
   *nbytes = amp[a].datarows*amp[a].ncols*bytes_pp;
   memcpy (ptr, (void *)(amp[a].dataptr + bytes_pp*(amp[a].prerows)*amp[a].ncols), *nbytes);
}

/**
* Copies data  from upper amplifiers
* @param[in] ptr image buffer pointer
* @param[in] a amplifier index
* @param[in] row row number
* @param[in,out] *nbytes number of bytes to copy
*/
void GU_copy_ydat_upp (void *ptr, i32 a, i32 *nbytes)
{
   *nbytes = amp[a].datarows*amp[a].ncols*bytes_pp;
   memcpy (ptr, (void *)(amp[a].dataptr + bytes_pp*(amp[a].postrows)*amp[a].ncols), *nbytes);
}

/**
* Rearrange data based on the pointer of the arrange algors (GU_copy_ydat_upp() etc) selected with GU_set_arrange_sections_upper() etc)
* @param[in] chunkrows number of rows to arrange
**/
i32 GU_rearrange_sections (i32 chunkrows)
{
#ifdef TIMEDEB
struct timeval tval;
time_t beg, end;
#endif
i32 ret, i, y;
void *currptr;
i32 nbytes, totbytes;

        if ((ret=GU_get_ampbuf ()) < 0)
                return (ret);

        if (ampbufptr == NULL)
                return (-ENOMEM);

        pdebug ("rearranging sections...");
#ifdef TIMEDEB
        gettimeofday (&tval, NULL);
        beg = 1000*tval.tv_sec + tval.tv_usec/1000;
#endif
	totbytes = amp[0].ncols*chunkrows*bytes_pp;
        for (i=0; i<namps; i++){
        	currptr = ampbufptr;
		if ((amp[i].type & LEFT) && (arrange_sections_left)) {
                	for (y=0; y<chunkrows; y++){
				(*GUget_left1) (currptr, i, y, &nbytes); currptr += nbytes;
				(*GUget_left2) (currptr, i, y, &nbytes); currptr += nbytes;
				(*GUget_left3) (currptr, i, y, &nbytes); currptr += nbytes;
			}
        		memcpy ((void *)(dptr_t)amp[i].dataptr, ampbufptr, totbytes);
		} else {
			if (arrange_sections_right) {
                		for (y=0; y<chunkrows; y++){
                               	 (*GUget_right1) (currptr, i, y, &nbytes); currptr += nbytes;
                               	 (*GUget_right2) (currptr, i, y, &nbytes); currptr += nbytes;
                               	 (*GUget_right3) (currptr, i, y, &nbytes); currptr += nbytes;
                        	}
                        	memcpy ((void *)(dptr_t)amp[i].dataptr, ampbufptr, totbytes);
			}
		}
        	currptr = ampbufptr;
		if ((amp[i].type & LOWER) && (arrange_sections_lower)) {
                        (*GUget_lower1) (currptr, i, &nbytes); currptr += nbytes;
                        (*GUget_lower2) (currptr, i, &nbytes); currptr += nbytes;
                        (*GUget_lower3) (currptr, i, &nbytes); currptr += nbytes;
                        memcpy ((void *)(dptr_t)amp[i].dataptr, ampbufptr, totbytes);
		} else {
               		if (arrange_sections_upper) {
                      	  (*GUget_upper1) (currptr, i, &nbytes); currptr += nbytes;
                       	  (*GUget_upper2) (currptr, i, &nbytes); currptr += nbytes;
                       	  (*GUget_upper3) (currptr, i, &nbytes); currptr += nbytes;
                       	  memcpy ((void *)(dptr_t)amp[i].dataptr, ampbufptr, totbytes);
			}
                }
	}
#ifdef TIMEDEB
        gettimeofday (&tval, NULL);
        end = 1000*tval.tv_sec + tval.tv_usec/1000;
        printf ("rearranging time: %ld msecs\n", end-beg);
#endif
        return (OK);
}

/***********end rearranging routines***************************************/


/**
* Performs the rearranging of the amplifier data so it gets blended into a detector. IN other words,
takes the data of each amplifier ibn a given detector and puts them toguether so the complete detector
data gets togueter and can be later passed to the fits server to be written as a "detector extension"
* @param[in] outbuff output buffer pointer
* @param[in] nelems number of elements (pixels) in each amplifier
* @param[in] chunkrows number of rows to blend
**/
i32 GU_do_detextensions (ptr_t outbuff, i32 nelems, i32 chunkrows)
{
i32 i, d, ret=0;
i32 subxstart, subystart;
void *bufptr;
i32 detdata=0;
#ifdef TIMEDEB
struct timeval tval;
time_t beg, end;

       gettimeofday (&tval, NULL);
       beg= 1000*tval.tv_sec + tval.tv_usec/1000;
#endif
	if (outbuff == 0){
		GU_getdetbuf ();
		outbuff = (ptr_t)(dptr_t) detbufptr;
	}
	bufptr = (void *)(dptr_t) outbuff;
	detdata = nelems / ndets;
	if (bufptr == NULL)
		return (-ENOMEM);
	for (i=0; i<namps; i++){
		d = GU_getdet (amp[i].detno);
		if (d < 0)
			return (-ENXIO);
//		det[d].dataptr = (ptr_t) (bufptr + d*(det[d].ncols * det[d].nrows * bytes_pp));
		det[d].dataptr = (ptr_t)(dptr_t) (bufptr + d*(detdata * bytes_pp));
		subxstart = amp[i].xstart - det[d].xstart + 1;
		subystart = amp[i].ystart - det[d].ystart + 1;
		pdebug ("det %d, amp %d, subxstart %d, subystart %d, dataprt %lu\n", d, i, subxstart, subystart, det[d].dataptr);
		pdebug ("rearranging amplifier ...%d\n", i);
		if ((ret=GU_add_subimage (det[d].dataptr, amp[i].dataptr, subxstart, subystart, amp[i].ncols, chunkrows, det[d].ncols)) < 0)
			return (ret);
	}
#ifdef TIMEDEB
        gettimeofday (&tval, NULL);
        end = 1000*tval.tv_sec + tval.tv_usec/1000;
	printf ("rearranging amplifiers took %ld\n", end-beg);
#endif
	return (OK);
}
