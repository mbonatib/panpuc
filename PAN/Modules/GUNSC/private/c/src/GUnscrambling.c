/*****************************************************************************************//*
Main unscrambling file. Here it opens the dinamyc libraries to be used to unscrambling.
This are the rutines to be called by the user (caller) program
*****************************************************************************************/


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <dlfcn.h>
#ifdef TIMEDEB
  #include <time.h>
  #include <sys/time.h>
#endif
#include "../../../public/c/include/GUnscrambling.h"

#ifdef _DEBUG_
#  define pdebug(fmt, args...) printf ("GUNSC: " fmt, ## args)
#else
#  define pdebug(fmt, args...)
#endif


void  *FunctionLib;             /**< Handle to shared lib file   */
ptr_t   (*GUnsc)();            /**<  Pointer to loaded routine   */
i32   (*GUadd)();            /**<  Pointer to loaded routine   */
i32   (*GUadddet)();            /**<  Pointer to loaded routine   */
i32   (*GUdetext)();            /**<  Pointer to loaded routine   */
i32   (*GUorder_left)();            /**<  Pointer to loaded routine   */
i32   (*GUorder_right)();            /**<  Pointer to loaded routine   */
i32   (*GUorder_lower)();            /**<  Pointer to loaded routine   */
i32   (*GUorder_upper)();            /**<  Pointer to loaded routine   */
i32   (*GUclear)();            /*  Pointer to loaded routine   */
i32   (*GUcopy)();            /*  Pointer to loaded routine   */
ptr_t (*GUgetPtr)();            /*  Pointer to loaded routine   */
ptr_t (*GUgetptr)();            /*  Pointer to loaded routine   */
i32   (*GUsetmap)();            /*  Pointer to loaded routine   */
i32   (*GUpixswap)();            /*  Pointer to loaded routine   */
i32   (*GUsetbytespp)();            /*  Pointer to loaded routine   */
#ifdef _DEBUG_
i32   (*GUshow)();            /*  Pointer to loaded routine   */
#endif
const char *dlError;            /*  Pointer to error string             */
i32 dtype=16;
i32 enabled=1;
char algorpath[1024];

#if 0
inline void *dptr_t (ptr_t inptr)
{
#ifdef OS64
      return ((void *)inptr);
#else
      return ((void *)(ptr32_t) inptr);
#endif
}
#endif

/**
* Checks dynamic library error
**/
i32 GU_checkError (i32 iferr)
{
	dlError = dlerror ();
	if(dlError) {
		printf ("%s\n", dlError);
		return (-iferr);
	}
	return (OK);
}

/**
* Opens the specified dynamic library
* @param[in] *filepath full library path to open
**/
i32 GU_open (char *filepath)
{
i32 ret;

	strcpy (algorpath, filepath);
	pdebug ("libpath %s\n", filepath);

        FunctionLib = dlopen (filepath, RTLD_LAZY);
	if ((ret = GU_checkError (ELIBACC)))
		return (ret);
	GUadd = dlsym (FunctionLib, "GU_add_amp");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);	 
	GUadddet = dlsym (FunctionLib, "GU_add_det");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);	 
	GUdetext = dlsym (FunctionLib, "GU_det_ext");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);	 
	GUclear = dlsym (FunctionLib, "GU_clear_amps");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);
	GUgetptr = dlsym (FunctionLib, "GU_get_amps_ptr");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);
	GUsetmap = dlsym (FunctionLib, "GU_SetPhysMap");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);
	GUcopy = dlsym (FunctionLib, "GU_copy_image");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);
	GUgetPtr = dlsym (FunctionLib, "GU_get_raw_ptr");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);
	GUsetbytespp = dlsym (FunctionLib, "GU_set_bytes_pp");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);
	GUpixswap = dlsym (FunctionLib, "GU_pix_swap");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);
        GUorder_left = dlsym (FunctionLib, "GU_set_arrange_sections_left");
        if ((ret = GU_checkError (ELIBACC)))
                return (ret);
        GUorder_right = dlsym (FunctionLib, "GU_set_arrange_sections_right");
        if ((ret = GU_checkError (ELIBACC)))
                return (ret);
        GUorder_lower = dlsym (FunctionLib, "GU_set_arrange_sections_lower");
        if ((ret = GU_checkError (ELIBACC)))
                return (ret);
        GUorder_upper = dlsym (FunctionLib, "GU_set_arrange_sections_upper");
        if ((ret = GU_checkError (ELIBACC)))
                return (ret);

#ifdef _DEBUG_
	pdebug ("opening show function\n");	
	GUshow = dlsym (FunctionLib, "GU_show_amps");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);	 
#endif
	return (OK);
}

/**
* Closes the opened library
**/
i32 GU_close (void)
{
i32 ret;
	
	ret = dlclose (FunctionLib);
	if ((ret = GU_checkError (ELIBACC)))
		return (ret);

	return (OK);
}

/**
* Get maximum number of amps
**/
i32 GU_GetMaxAmps (void)
{
	return (MAX_AMPS);
}

/**
* Selects the unscrambling routine to be used based on the datatype
* @param[in] datatype type of pixel to be unscrambled (short, long, float)
**/
i32 GU_compute (i32 datatype)
{
char funcname[20];
i32 ret;

    if (enabled) {
        if (datatype == 32) 
                sprintf (funcname, "GU_LONG");
        else if (datatype == -32) 
                sprintf (funcname, "GU_FLOAT");
        else if (datatype == 16) 
                sprintf (funcname, "GU_SHORT");
         else
                return (-EINVAL);

        GUnsc = dlsym (FunctionLib, funcname);
        if ((ret = GU_checkError (ENOSYS)))
                return (ret);
        pdebug ("GU_compute: now unscrambling function is %s\n", funcname);
     }

     (*GUsetbytespp) ((int)((float)abs (datatype)/8));

     dtype = datatype;
     return (OK);
}

/**
* Enables or disables unscrambling
**/
i32 GU_enable (i16 flag)
{
i32 ret;

        if (flag <=0){
                GUnsc = dlsym (FunctionLib, "GU_NONE");
                if ((ret = GU_checkError (ENOSYS))) {
                        return (ret);
		}
		enabled = 0;
                pdebug ("GU_compute: ***unscrambling is disabled***\n");
        } else {
		enabled = 1;
                ret = GU_compute (dtype);
	}

        return (ret);
}

/**
* Is unscrambling enabled
**/
i32 GU_is_enabled (void)
{
	return (enabled);
}

/**
* pixel swap requiered in some cases (upper/lower half)
**/
i32 GU_call_pix_swap (i32 flag)
{
i32 ret;

	ret = (*GUpixswap)(flag);
	return (OK);
}

/**
* Clears the amplifiers structure
**/
i32 GU_call_clear_amps (void)
{
i32 ret;

	ret = (*GUclear)();
	return (ret);
}

/**
* Enables/Disables the per-detector extensions
**/
i32 GU_call_det_ext (i32 flag)
{
	return ((*GUdetext)(flag));
}

/**
* Directs the order in which predata, data and postdata should be orderd for "left" amplifiers
* sections can be "pre", "dat", or "pos". 
* @param[in] left1 first section
* @param[in] left2 second section
* @param[in] left3 third section
**/
i32 GU_call_set_order_left (char *left1, char *left2, char *left3)
{
	return ((*GUorder_left)(algorpath, left1, left2, left3));
}

/**
* Directs the order in which predata, data and postdata should be orderd for "right" amplifiers
* sections can be "pre", "dat", or "pos"
* @param[in] right1 first section
* @param[in] right2 second section
* @param[in] right3 third section
**/
i32 GU_call_set_order_right (char *right1, char *right2, char *right3)
{
	return ((*GUorder_right)(algorpath, right1, right2, right3));
}

/**
* Directs the order in which predata, data and postdata should be orderd for "lower" amplifiers
* sections can be "pre", "dat", or "pos"
* @param[in] low1 first section
* @param[in] low2 second section
* @param[in] low3 third section
**/
i32 GU_call_set_order_lower (char *low1, char *low2, char *low3)
{
	return ((*GUorder_lower)(algorpath, low1, low2, low3));
}

/**
* Directs the order in which predata, data and postdata should be orderd for "upper" amplifiers
* section names can be "pre", "dat", or "pos"
* @param[in] upp1 first section
* @param[in] upp second section
* @param[in] upp3 third section
**/
i32 GU_call_set_order_upper (char *upp1, char *upp2, char *upp3)
{
	return ((*GUorder_upper)(algorpath, upp1, upp2, upp3));
}


/**
* Adds a new detector to detectors structure.
* All coordinates in global FPA coordinates
* @param[in] xstart lower right x position
* @param[in] precols number of predata pixels in x direction (prescans)
* @param[in] datacols number of col data pixels (x direction)
* @param[in] postcols number of postdata pixels in x direction (oversscans)
* @param[in] ystart lower right y position
* @param[in] prerows number of predata lines in y direction (yprescans)
* @param[in] datarows number of row data lines (y direction)
* @param[in] postrows number of postdata lines in y direction (yoversscans)
* @param[in] nxamps number of amplifiers in x direction (per line of detector)
* @param[in] nyamps number of amplifiers in y direction (per column of detector)
* @param[in] discard if set, indicates to discard this amplifer
* @param[in] detno detector number (ID) in the FPA
**/
i32 GU_call_add_det (i32 xstart, i32 precols, i32 datacols, i32 postcols, i32 ystart, i32 prerows, i32 datarows, i32 postrows, i32 nxamps, i32 nyamps, i32 discard, i32 detno)
{
i32 ret;

	ret = (*GUadddet) (xstart, precols, datacols, postcols, ystart, prerows, datarows, postrows, nxamps, nyamps, discard, detno);
	return (ret);
}


/**
* Adds a new amplifier to amplifiers structure
* all coordinates in detector-based coordinates
* @param[in] xstart lower right x position
* @param[in] precols number of predata pixels in x direction (prescans)
* @param[in] datacols number of col data pixels (x direction)
* @param[in] postcols number of postdata pixels in x direction (oversscans)
* @param[in] ystart lower right y position
* @param[in] prerows number of predata lines in y direction (yprescans)
* @param[in] datarows number of row data lines (y direction)
* @param[in] postrows number of postdata lines in y direction (yoversscans)
* @param[in] xdetstart lower right position of detctor in FPA
* @param[in] ydetstart lower right position of detector in FPA
* @param[in] type amplifier type (lowerleft (LL), Lowerright (LR), UpperLeft (UL) or UpperRight (UR
* @param[in] rotate rotation angle for the amplifier (0,90,180 or 270 allowed only)
* @param[in] flip indicates if the amplifier is flipped
* @param[in] extension indicates the extension number where this should be mapped
* @param[in] detno indicates the detector number to which this amplifier belongs
* @param[in] ampno indicates the amplifier number
* @param[in] discard indicates if this amplifier should be discarded
**/
i32 GU_call_add_amp (i32 xstart, i32 precols, i32 datacols, i32 postcols, i32 ystart, i32 prerows, i32 datarows, i32 postrows, i32 xdetstart, i32 ydetstart, i16 type, i32 rotate, i16 flip, i32 extension, i32 detno, i32 ampno, i32 discard)
{
i32 ret;
	ret = (*GUadd) (xstart, precols, datacols, postcols, ystart, prerows, datarows, postrows, xdetstart, ydetstart, type, rotate, flip, extension, detno, ampno, discard);
	return (ret);
}

/**
* Gets amplifiers structure pointer
**/
ptr_t GU_call_get_amps_ptr (void)
{
	pdebug ("call ampptr: %lu\n", (ulong)(*GUgetptr) ());
	return ((*GUgetptr) ());
}

/**
* Maps the amplifier read order to the position in the buffer
* @param[in] array array containing the structure
**/
i32 GU_call_SetPhysMap (ptr_t array, i32 num_elements)
{
	return ((*GUsetmap) (array, num_elements));
}

/**
* Shows (print out) the amplifiers structure)
**/
i32 GU_call_show_amps (void)
{
i32 ret;

#ifdef _DEBUG_
	ret = (*GUshow) ();
#else
	printf ("no in debug mode. Ciao\n");
	ret = OK;
#endif
	return (ret);
}



/**
* Unscrambles the data. This routine will call the function selected with GU_compute()
* @param[in] rptr pointer to the raw data buffer (as read from the controller)
* @param[in] imageptr pointer to the output data buffer (where to store the unscrambled data)
* @param[in] nelems number of pixels
* @param[out] *err status out
**/
ptr_t GU_call_unscrambling (ptr_t rptr, ptr_t imageptr, i32 nelems, i32 *err)
{
ptr_t ret;
#ifdef TIMEDEB
struct timeval tval;
time_t beg, end;

        gettimeofday (&tval, NULL);
        beg = 1000*tval.tv_sec + tval.tv_usec/1000;
#endif
	pdebug ("call_unsc: rptr %ld, imptr %ld\n", (ulong)(ptr_t)(dptr_t)rptr, (ulong)(ptr_t)(dptr_t)imageptr);
	
	ret = (*GUnsc)(rptr, imageptr, nelems, err);
	pdebug ("GU_call_unscrambling returned %ld\n", ret);
#ifdef TIMEDEB
        gettimeofday (&tval, NULL);
        end = 1000*tval.tv_sec + tval.tv_usec/1000;
	printf ("GUnscrambling took %ld\n", end-beg);
#endif		
	return (ret);
}					

/**
* Copy image from amplifiers structure to output buffer
* @param[in] buff buffer where image is to be copied
* @param[in] amplifiers pointer to amplifier structure
* @param[in] totcols number of columns in image
**/
i32 GU_call_copy_image (i32 buff, i32 amplifiers, i32 totcols)
{
i32 ret;

	ret = (*GUcopy)(buff, amplifiers, totcols);
		
	return (ret);
}

/**
* Get pointer to the databuffer inside the amplifiers structure
* @param[out] namplif number of amplifiers
**/
ptr_t GU_call_get_raw_ptr (i32 *namplif)
{
ptr_t ret;

	ret = (*GUgetPtr) (namplif);
		
	return (ret);
}
