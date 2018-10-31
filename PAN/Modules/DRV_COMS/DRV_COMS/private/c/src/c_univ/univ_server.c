/*SE READ THE FILE NB-APOLOGY!!!!  There are some things you should
   know about this source before you read it.  Thanks.

   
   Quang Ngo alerted me to a bug where the variable listnum in deal_with_data()
   wasn't being passed in by parameter, thus it was always garbage. I have
   quick-fixed this in the code below. - Spencer (October 12, 1999)
   

   Non blocking server demo 
   By Vic Metcalfe (vic@acm.org)
   For the unix-socket-faq
*/

#include "sockhelp.h"
#include "media_libs/dev_funcs.h"
#include "univ_gendefs.h"
#include "getLib.h"
#include <ctype.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <dlfcn.h>


int sock;            /* The socket file descriptor for our "listening"
                   	socket */
int connectlist[5];  /* Array of connected sockets so we know who
	 		we are talking to */
fd_set socks;        /* Socket file descriptors we want to wake
			up for, using select() */
int highsock;	     /* Highest #'d file descriptor, needed for select() */

int (*DEV_OpenPtr)();
int (*DEV_ClosePtr)();
void *Lib;
const char *dlError;
int     gbl_timeout = DEF_TIMEOUT;
int     gbl_maxrate = DEF_REQRATE;
int     gbl_retries = DEF_RETRIES;
int     gbl_shutdown = 0;
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
        Lib = dlopen (NULL, RTLD_LAZY);
        if ((ret=LibChkErr (ELIBACC)) < 0)
                return (ret);

        sprintf (func, "DEV_%s_open", type);
        DEV_OpenPtr = dlsym (Lib, func);
        if ((ret=LibChkErr (ELIBACC)) < 0)
                return (ret);

        sprintf (func, "DEV_%s_close", type);
        DEV_ClosePtr = dlsym (Lib, func);
        if ((ret=LibChkErr (ELIBACC)) < 0)
                return (ret);

        return ((*DEV_OpenPtr)(params));
}

int UNIV_Close (int fd)
{
        (*DEV_ClosePtr)(fd);
        dlclose (Lib);
	printf ("closing device and library\n");
        return (OK);
}

int UNIV_Error (int fd, int error)
{
	if ((error == -9) || (error = -111))
		hFd = ERROR;
	else
        	UNIV_Close (fd);
        return (error);
}


int UNIV_Write (int fd, char *val, int timeout)
{
int nread;
struct timeval tvstart, tvend;
char *value;

	val = malloc (strlen (val) + 3);
	strcpy (value, val);
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
	printf ("%c\n", buf);
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
                if (strncmp (token, "SHUTDOWN", 11) == 0) {
			gbl_shutdown = 1;
			sprintf (outbuffer, "shutting down");
			break;
                }
                if (strncmp (token, "CLOSE", 5) == 0) {
			gbl_shutdown = 1;
			sprintf (outbuffer, "shutting down");
			break;
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


void setnonblocking(sock)
int sock;
{
	int opts;

	opts = fcntl(sock,F_GETFL);
	if (opts < 0) {
		perror("fcntl(F_GETFL)");
		exit(EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(sock,F_SETFL,opts) < 0) {
		perror("fcntl(F_SETFL)");
		exit(EXIT_FAILURE);
	}
	return;
}

void build_select_list() {
	int listnum;	     /* Current item in connectlist for for loops */

	/* First put together fd_set for select(), which will
	   consist of the sock veriable in case a new connection
	   is coming in, plus all the sockets we have already
	   accepted. */
	
	
	/* FD_ZERO() clears out the fd_set called socks, so that
		it doesn't contain any file descriptors. */
	
	FD_ZERO(&socks);
	
	/* FD_SET() adds the file descriptor "sock" to the fd_set,
		so that select() will return if a connection comes in
		on that socket (which means you have to do accept(), etc. */
	
	FD_SET(sock,&socks);
	
	/* Loops through all the possible connections and adds
		those sockets to the fd_set */
	
	for (listnum = 0; listnum < 5; listnum++) {
		if (connectlist[listnum] != 0) {
			FD_SET(connectlist[listnum],&socks);
			if (connectlist[listnum] > highsock)
				highsock = connectlist[listnum];
		}
	}
}

void handle_new_connection() {
	int listnum;	     /* Current item in connectlist for for loops */
	int connection; /* Socket file descriptor for incoming connections */

	/* We have a new connection coming in!  We'll
	try to find a spot for it in connectlist. */
	connection = accept(sock, NULL, NULL);
	if (connection < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	setnonblocking(connection);
	for (listnum = 0; (listnum < 5) && (connection != -1); listnum ++)
		if (connectlist[listnum] == 0) {
			printf("\nConnection accepted:   FD=%d; Slot=%d\n",
				connection,listnum);
			connectlist[listnum] = connection;
			connection = -1;
		}
	if (connection != -1) {
		/* No room left in the queue! */
		printf("\nNo room left for new client.\n");
		sock_puts(connection,"Sorry, this server is too busy.   Try again later!\r\n");
		close(connection);
	}
}

void deal_with_data(
	int listnum 		/* Current item in connectlist for for loops */
	) {
	char wbuffer[80];     /* Buffer for socket reads */
	char rbuffer[BUFLEN];     /* Buffer for socket reads */
	int timeout;
	double timediff;
	struct timeval	tvnew;
	int	ret;

	if (sock_gets(connectlist[listnum],wbuffer,80) < 0) {
		/* Connection closed, close this end
		   and free up entry in connectlist */
		printf("\nConnection lost: FD=%d;  Slot=%d\n",
			connectlist[listnum],listnum);
		close(connectlist[listnum]);
		connectlist[listnum] = 0;
	} else {
		printf("\nReceived: %s; ",wbuffer);
		if (!UNIV_IsLocalCmd (wbuffer, rbuffer)){
			if (hFd > 0){
				timeout = gbl_timeout;
				rbuffer[0] = '\0';
wait:
                                gettimeofday (&tvnew, NULL);
                                timediff = (float)((tvnew.tv_sec*1E6 + tvnew.tv_usec) - (gbl_tvlast.tv_sec*1E6 + gbl_tvlast.tv_usec))/1E6;
                                if (timediff < (double) gbl_maxrate/1000) {
                                    printf ("waiting ... (%g time)\r", timediff);
				    goto wait;
                                }
                                printf ("time since last %g (maxrate %g)\n", timediff, (double)gbl_maxrate/1000);
                                gettimeofday (&gbl_tvlast, NULL);
				tcflush (hFd, TCIOFLUSH);
				if (UNIV_Write (hFd, wbuffer, timeout) < 0){
					UNIV_Error (hFd, -errno);
					sprintf (rbuffer, "ERROR %d writting to hardware\n", -errno);
				} else {
					usleep (gbl_maxrate * 1E3);
					if ((ret=UNIV_Read (hFd, rbuffer, timeout) < 0)){
				//		UNIV_Error (hFd, ret);
						sprintf (rbuffer, "ERROR %d reading from hardware (hFd %d\n", ret, hFd);
					}
				}
			} else
				sprintf (rbuffer, "ERROR not connected to hardware\n");
		}
		sock_puts(connectlist[listnum],rbuffer);
		sock_puts(connectlist[listnum],"\n");
		printf("response: %s\n",rbuffer);
	}
}

void read_socks() {
	int listnum;	     /* Current item in connectlist for for loops */

	/* OK, now socks will be set with whatever socket(s)
	   are ready for reading.  Lets first check our
	   "listening" socket, and then check the sockets
	   in connectlist. */
	
	/* If a client is trying to connect() to our listening
		socket, select() will consider that as the socket
		being 'readable'. Thus, if the listening socket is
		part of the fd_set, we need to accept a new connection. */
	
	if (FD_ISSET(sock,&socks))
		handle_new_connection();
	/* Now check connectlist for available data */
	
	/* Run through our sockets and check to see if anything
		happened with them, if so 'service' them. */
	
	for (listnum = 0; listnum < 5; listnum++) {
		if (FD_ISSET(connectlist[listnum],&socks))
			deal_with_data(listnum);
	} /* for (all entries in queue) */
}

int main (int argc, char *argv[]) 
//int argc;
//char *argv[];
{
//	char *ascport;  /* ASCII version of the server port */
	int port;       /* The port number after conversion from ascport */
	struct sockaddr_in server_address; /* bind info structure */
	int reuse_addr = 1;  /* Used so we can re-bind to our port
				while a previous connection is still
				in TIME_WAIT state. */
	struct timeval timeout;  /* Timeout for select */
	int readsocks;	     /* Number of sockets ready for reading */

	int hCt=0, i;
	char type[20];
	char params[255];

       if (argc == 1) {
                printf ("ERROR no media type specified\n");
                return (-EINVAL);
        }
        i = 1;
	strcpy (params, "");
        while (i < argc){
                if (strlen (argv[i]) > 0){
                        strcat (params, argv[i]);
                        strcat (params, " ");
                }
                i++;
        }

	gettimeofday (&gbl_tvlast, NULL);
	pdebug ("params %s\n", params);
        if (UNIV_ParseIniArgs (params, type, &port) < 0)
                return (-EINVAL);

	/* Make sure we got a port number as a parameter */
//	if (argc < 2) {
//		printf("Usage: %s PORT\r\n",argv[0]);
//		exit(EXIT_FAILURE);
//	}

	/* Obtain a file descriptor for our "listening" socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	/* So that we can re-bind to it without TIME_WAIT problems */
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
		sizeof(reuse_addr));

	/* Set socket to non-blocking with our setnonblocking routine */
	setnonblocking(sock);

	/* Get the address information, and bind it to the socket */
//	ascport = argv[1]; /* Read what the user gave us */
//	port = atoi(ascport); /* Use function from sockhelp to
 //                                   convert to an int */
	memset((char *) &server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(port);
	if (bind(sock, (struct sockaddr *) &server_address,
	  sizeof(server_address)) < 0 ) {
		perror("bind");
		close(sock);
		exit(EXIT_FAILURE);
	}

	/* Set up queue for incoming connections. */
	listen(sock,5);

	/* Since we start with only one socket, the listening socket,
	   it is the highest socket so far. */

	
	highsock = sock;
	memset((char *) &connectlist, 0, sizeof(connectlist));
	hCt = 21;

	while (!gbl_shutdown) { /* Main server loop - forever */
		build_select_list();
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

                if (hFd == ERROR) {
                       if (hCt++ > 20) {
                              hFd = UNIV_Open (type, params);
                              printf ("Opening connection to Hardware (%d)\n", hFd);
                              if (hFd <= 0) {
//                                      UNIV_SockClose (sockFd);
                                      printf ("ERROR %d opening %s\n", hFd, type );
                              }
                              hCt = 0;
                        }
                }

		
		/* The first argument to select is the highest file
			descriptor value plus 1. In most cases, you can
			just pass FD_SETSIZE and you'll be fine. */
			
		/* The second argument to select() is the address of
			the fd_set that contains sockets we're waiting
			to be readable (including the listening socket). */
			
		/* The third parameter is an fd_set that you want to
			know if you can write on -- this example doesn't
			use it, so it passes 0, or NULL. The fourth parameter
			is sockets you're waiting for out-of-band data for,
			which usually, you're not. */
		
		/* The last parameter to select() is a time-out of how
			long select() should block. If you want to wait forever
			until something happens on a socket, you'll probably
			want to pass NULL. */
		
		readsocks = select(highsock+1, &socks, (fd_set *) 0, 
		  (fd_set *) 0, &timeout);
		
		/* select() returns the number of sockets that had
			things going on with them -- i.e. they're readable. */
			
		/* Once select() returns, the original fd_set has been
			modified so it now reflects the state of why select()
			woke up. i.e. If file descriptor 4 was originally in
			the fd_set, and then it became readable, the fd_set
			contains file descriptor 4 in it. */
		
		if (readsocks < 0) {
			perror("select");
			exit(EXIT_FAILURE);
		}
		if (readsocks == 0) {
			/* Nothing ready to read, just show that
			   we're alive */
			printf("Waiting\r");
			fflush(stdout);
		} else 
			read_socks();
	} /* while(1) */
	UNIV_Close (hFd);
	exit (OK);
} /* main */
		
