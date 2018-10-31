#include "media_libs/dev_funcs.h"
#include "univ_gendefs.h"
#include "getLib.h"
#include <ctype.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>


int (*DEV_OpenPtr)();
int (*DEV_ClosePtr)();
void *Lib;
char media[30];
const char *dlError;
int     gbl_timeout = DEF_TIMEOUT;
int     gbl_maxrate = DEF_REQRATE;
int     gbl_retries = DEF_RETRIES;
char    gbl_cmdterm = DEF_CMDTERM;
char    gbl_respterm = DEF_RESPTERM;
char	gbl_increspterm = 0;
struct timeval gbl_tvlast;

int     hFd = ERROR;

int LibChkErr (int iferr)
{
        dlError = dlerror ();
        if(dlError) {
                printf ("%s\n", dlError);
                return (-iferr);
        }
        return (OK);
}

int UNIV_SockClose (int sock)
{
        return (close(sock));
}



int UNIV_Open (char *type, char *params)
{
//char *path[256];
char func[30];
int ret=OK;

//      sprintf (path, "%s/libuniv%s.co", LibPath, type)
//      sprintf (path, "../src/driver_api/libarcCOS%s.so", type);
 //       printf ("path (%s)\n", path);
       #if 0
        Lib = dlopen (NULL, RTLD_LAZY);
        if ((ret=LibChkErr (ELIBACC)) < 0)
                return (ret);

	printf ("univ: ok, opened local\n");
        sprintf (func, "DEV_%s_open", type);
        DEV_OpenPtr = dlsym (Lib, func);
        if ((ret=LibChkErr (ELIBACC)) < 0)
                return (ret);
	printf ("univ: ok, opened open %s\n", func);

        sprintf (func, "DEV_%s_close", type);
        DEV_ClosePtr = dlsym (Lib, func);
        if ((ret=LibChkErr (ELIBACC)) < 0)
                return (ret);
	printf ("univ: ok, opened open %s\n", func);

        return ((*DEV_OpenPtr)(params));
#else
	strcpy (media, type);
	if (strcmp (media, "serial") == 0)
		return (DEV_serial_open (params));

	return (DEV_tcp_open (params));
#endif
}

int UNIV_Close (int fd)
{
	printf ("closing device and library\n");
#if 0
        (*DEV_ClosePtr)(fd);
        dlclose (Lib);
	printf ("closing device and library\n");
        return (OK);
#else
	if (strcmp (media, "serial") == 0)
		return (DEV_serial_close (fd));
	return (DEV_tcp_close (fd));
#endif
	
}

int UNIV_Error (int fd, int error)
{
	if ((error == -9) || (error = -111))
		hFd = ERROR;
	else
        	UNIV_Close (fd);
        return (error);
}


int UNIV_Write (int fd, char *value, int timeout)
{
int nread;
struct timeval tvstart, tvend;

        pdebug ("writting %s to fd %d\n", value, fd);
	strcat (value, "\n\r");
        gettimeofday (&tvstart, NULL);
        while ((nread = write (fd, value, strlen(value))) < 0) {
           if (errno != EAGAIN) {
                   pdebug ("ERROR %d writing_message\n", errno);
                   UNIV_Error (fd, -errno);
                   return (-errno);
           }
          usleep (1000);
           gettimeofday (&tvend, NULL);
           if (tvend.tv_sec - tvstart.tv_sec >= timeout) {
                   pdebug ("ERROR %d TIMEOUT\n", -ETIME);
                   UNIV_Error (fd, -errno);
                   return (-ETIME);
           }
        }
        return (OK);
}

int UNIV_read_timeout (int fd, char *buf, int timeout)
{
int nread;
struct timeval tvstart, tvend;

        gettimeofday (&tvstart, NULL);
        while ((nread = read (fd, buf, 1)) < 0) {
                if (errno != EAGAIN) {
                        pdebug ("ERROR %d reading\n", errno);
         //               UNIV_Error (fd, -errno);
                        return (-errno);
                }
                usleep (1000);
                gettimeofday (&tvend, NULL);
                if (tvend.tv_sec - tvstart.tv_sec >= timeout) {
                        pdebug ("ERROR %d TIMEOUT\n", -ETIME);
         //               UNIV_Error (fd, -ETIME);
                        return (-ETIME);
                }
        }
        return (1);
}
#if 0
int UNIV_read_timeout (int fd, char *c, int usec)
{
        struct timeval  Timer;
        fd_set          readFds;
	int		ret=0;

        if (usec < 0)
                usec = gbl_timeout * 1000;

        Timer.tv_sec = (long)(usec/1000000);
        Timer.tv_usec = (long)(usec - Timer.tv_sec*1000000);

        FD_ZERO (&readFds);
        FD_SET (fd, &readFds);
        if (! select (FD_SETSIZE, &readFds, (fd_set *)NULL,
          (fd_set *)NULL, &Timer))
                return (-ETIME);
        else
		while (ret != -EAGAIN) 
                	ret = read (fd, c, 1);
        return (OK);
}
#endif
/* DBSP box returns \n\r combo, so can discard and terminate on \r; \n
will be included in string, with null appended */
int UNIV_Read (int fd, char *buf, int timeout)
{
	int		ret=0;
	char		last_read=0;
	char		*current_position;
	int		total_count=0;

	current_position = buf;
	while (last_read != gbl_respterm) {
        	if ((ret=UNIV_read_timeout (fd, &last_read, timeout)) < 0) 
               		 return (ret);
		if ((total_count < BUFLEN) && (last_read != gbl_respterm)){
			current_position[0] = last_read;
			current_position++;
			total_count++;
		}
	}
	if (gbl_increspterm){
        	current_position[0] = gbl_respterm;
		current_position++;
		total_count++;
	}
        *current_position = '\0';
        return (total_count);
}


void UNIV_ParseLocalCmd (char *inbuffer, char *outbuffer)
{	
char *local;
char  token[256];
char  term[5];
char  incterm[5];
int     tmt=0, mr=0, retries=0;

        local = inbuffer;
        while (!checkIfEos (local)){
                local = getToken (local, token);
//		pdebug ("UNIV LOCAL parsing Token %s\n", token);
                if (strncmp (token, "timeout", 7) == 0) {
                        local = getInteger (local, &tmt);
			if (tmt < 0)
				gbl_timeout = NO_WAIT;
			if (tmt > 0)
				gbl_timeout = tmt;
			sprintf (outbuffer, "timeout now is %d [s]\n", gbl_timeout);
                }
                if (strncmp (token, "maxrate", 7) == 0) {
                        local = getInteger (local, &mr);
			if (mr > 0) {
				gbl_maxrate = mr;
				sprintf (outbuffer, "maxreqtime now is %d [ms]\n", gbl_maxrate);
			} else
				sprintf (outbuffer, "ERROR invalid argument\n");
		}
                if (strncmp (token, "retries", 7) == 0) {
                        local = getInteger (local, &retries);
			if (retries > 0) {
				gbl_retries = retries;
				sprintf (outbuffer, "retries now is %d times\n", gbl_retries);
			} else
				sprintf (outbuffer, "ERROR invalid argument\n");
		}
                if (strncmp (token, "cmdterm", 7) == 0) {
                        local = getToken (local, term);
			if (strncmp (term, "CR", 2) == 0) {
				gbl_cmdterm = CR;
				sprintf (outbuffer, "cmdterm now is %s\n", term);
			} else {
				gbl_cmdterm = LF;
				sprintf (outbuffer, "cmdterm now is LF\n");
			}
		}
                if (strncmp (token, "respterm", 8) == 0) {
                        local = getToken (local, term);
			if (strncmp (term, "CR", 2) == 0) {
				gbl_respterm = CR;
				sprintf (outbuffer, "respterm now is %s\n", term);
			} else {
				gbl_respterm = LF;
				sprintf (outbuffer, "respterm now is LF\n");
			}
		}
                if (strncmp (token, "increspterm", 11) == 0) {
                        local = getToken (local, incterm);
			if (strncmp (incterm, "yes", 3) == 0) {
				gbl_increspterm = 1;
				sprintf (outbuffer, "increspterm now is %s\n", incterm);
			} else {
				gbl_increspterm = 0;
				sprintf (outbuffer, "respterm now is NO\n");
			}
		}
                if (strncmp (token, "get", 3) == 0) {
			sprintf (outbuffer, "maxrate %d [ms], timeout %d [ms], retries %d, cmdterm 0x%x, respterm 0x%x, increspterm %d", gbl_maxrate, gbl_timeout, gbl_retries, gbl_cmdterm, gbl_respterm, gbl_increspterm);
			break;
                }
        }
}

int UNIV_IsLocalCmd (char *inbuffer, char *outbuffer)
{
char	cmd[20];

	sscanf (inbuffer, "%s", cmd);
/*take it to upper cases*/
#if 0
	char *cur_char;
	cur_char = cmd;
        while (cur_char[0] != 0) {
        	cur_char[0] = toupper(cur_char[0]);
        	cur_char++;
        }
#endif
	if (strcmp (cmd, "UNIV") != 0){
		return (0);
	}
	UNIV_ParseLocalCmd (inbuffer, outbuffer);
	return (1);
}

int UNIV_ParseIniArgs (char *info, char *type, int *server_port)
{
int valid=0;
char *local;
char  token[256];
char  outbuffer[256];
char  tp[20];
int     server=DEF_SVR_PORT;

	gbl_timeout = DEF_TIMEOUT;
	gbl_retries = DEF_RETRIES;
	gbl_maxrate = DEF_REQRATE;

	gbl_cmdterm = DEF_CMDTERM;
	gbl_respterm = DEF_RESPTERM;
        local = info;
        while (!checkIfEos (local)){
                local = getToken (local, token);
                pdebug ("next %s, rem %s\n", token, local);
               if (strncmp (token, "type", 4) == 0) {
                        local = getToken (local, tp);
                       if (errno == EINVAL)
                                return (-errno);
                        valid = 1;
                }
               if (strncmp (token, "server_port", 12) == 0) {
                        local = getInteger (local, &server);
                       if (errno == EINVAL)
                                return (-errno);
                        valid = 1;
                }
        }
        if (!valid){
                pdebug ("Not media TYPE defined!, aborting\n");
                return (-EINVAL);
        }
	
	UNIV_ParseLocalCmd (info, outbuffer);
        strcpy (type, tp);
        *server_port = server;
        pdebug ("parser: type %s, timeout %d, retries %d, maxrate %d, server_port %d\n", type, gbl_timeout, gbl_retries, gbl_maxrate, *server_port);
        return (OK);

}


int UNIV_Init (char *params) 
//int argc;
//char *argv[];
{
//	char *ascport;  /* ASCII version of the server port */
	int port;       /* The port number after conversion from ascport */

	char type[20];
//	char params[255];

	gettimeofday (&gbl_tvlast, NULL);
	pdebug ("params %s\n", params);
        if (UNIV_ParseIniArgs (params, type, &port) < 0)
                return (-EINVAL);

	return (UNIV_Open (type, params));

} /* main */
		
