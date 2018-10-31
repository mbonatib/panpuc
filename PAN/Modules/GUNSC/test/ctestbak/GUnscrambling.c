#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "../../public/c/include/GUnscrambling.h"

void  *FunctionLib;             /*  Handle to shared lib file   */
int   (*GUnsc)();            /*  Pointer to loaded routine   */
int   (*GUadd)();            /*  Pointer to loaded routine   */
int   (*GUclear)();            /*  Pointer to loaded routine   */
int   (*GUgetptr)();            /*  Pointer to loaded routine   */
#ifdef _DEBUG_
int   (*GUshow)();            /*  Pointer to loaded routine   */
#endif
const char *dlError;            /*  Pointer to error string             */


int GU_checkError (int iferr)
{
	dlError = dlerror ();
	if(dlError) {
		printf ("%s\n", dlError);
		return (-iferr);
	}
	return (OK);
}


int GU_open (void)
{
char filepath[40];
int ret;

	sprintf (filepath, "/home/ArcVIEW/Modules/GUNSC/test/ctest/libunscalgors.so");

        FunctionLib = dlopen (filepath, RTLD_LAZY);
	if ((ret = GU_checkError (ELIBACC)))
		return (ret);
	GUadd = dlsym (FunctionLib, "GU_add_amp");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);	 
	GUclear = dlsym (FunctionLib, "GU_clear_amps");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);
	GUgetptr = dlsym (FunctionLib, "GU_get_amps_ptr");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);
#ifdef _DEBUG_
	printf ("opening show function\n");	
	GUshow = dlsym (FunctionLib, "GU_show_amps");
	if ((ret = GU_checkError (ELIBACC)))
	        return (ret);	 
#endif
	return (OK);
}

int GU_close (void)
{
int ret;
	
	ret = dlclose (FunctionLib);
	if ((ret = GU_checkError (ELIBACC)))
		return (ret);

	return (OK);
}


int GU_compute (int datatype)
{
char funcname[20];
int ret;

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
#ifdef _DEBUG_
	printf ("GU_compute: now unscrambling function is %s\n", funcname);
#endif

        return (OK);
}


int GU_call_clear_amps (void)
{
int ret;

	ret = (*GUclear)();
	return (ret);
}


int GU_call_add_amp (int xstart, int precols, int datacols, int postcols, int ystart, int prerows, int datarows, int postrows, int xdetstart, int ydetstart, short type, int rotate, short flip, int extension)
{
int ret;
	ret = (*GUadd) (xstart, precols, datacols, postcols, ystart, prerows, datarows, postrows, xdetstart, ydetstart, type, rotate, flip, extension);
	return (ret);
}

int GU_call_get_amps_ptr (void)
{
	return ((*GUgetptr) ());
}

int GU_call_show_amps (void)
{
int ret;

#ifdef _DEBUG_
	ret = (*GUshow) ();
#else
	printf ("no in debug mode. Ciao\n");
	ret = OK;
#endif
	return (ret);
}




int GU_call_unscrambling (unsigned long rptr, unsigned long imageptr, int nelems)
{
int ret;

	ret = (*GUnsc)(rptr, imageptr, nelems);
		
	return (ret);
}					

