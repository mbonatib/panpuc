#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OK	0

/**
* Calls a postprox script using a system call
* @param[in] *ppath full path to script or exec. to call
* @param[in] *impath full path to image just written, in case the script requieres this information
* @param[in] *args arguments to be passed to the script or exec.
* @param[in] back flag to tell fi the script should be executed in background or we should wait (block)
**/
int call_postproc (char *pppath, char *impath, char *args, short back)
{
char cmd[2048];
char scr[2048];

	sscanf (pppath, "%s", scr);
//	printf ("postproc %s %s\n", scr, pppath);
	if (access (scr, R_OK | X_OK) == 0){
		if (access (impath, R_OK) == 0){
			if (back > 0)
				sprintf (cmd, "%s %s &", pppath, impath);
			else
				sprintf (cmd, "%s %s", pppath, impath);
			system (cmd);
			return (OK);
		}
	}
	return (-errno);
}
