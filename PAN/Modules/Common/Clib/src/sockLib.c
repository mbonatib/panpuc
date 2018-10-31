#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/time.h> 
#include <sys/types.h> 


#define OK 0

unsigned long dum_data (void)
{
unsigned short *data;

	data = malloc (20*sizeof (unsigned short));
	data[0] = 56;
	data[1] = 12;
	return ((unsigned long)data);
}
	

int socket_open (char *conn_type, char *hname, unsigned short port, int secs_timeout)
{
int sock, ret;
struct sockaddr_in sock_addr;
struct hostent *host_struct;
fd_set set;
struct timeval tv;
long response=0;
socklen_t resplen=4;
int type;

	type= SOCK_STREAM;
	if (strcmp (conn_type, "DGRAM") == 0)
		type = SOCK_DGRAM;
	tv.tv_sec = secs_timeout;
	tv.tv_usec = 0;
	if ((host_struct = gethostbyname (hname)) == NULL)
      		return (-errno);
      
	if ((sock = socket(AF_INET, type, 0)) == -1)
      			return (-errno);

	if (fcntl (sock, F_SETFL, O_NONBLOCK) <0)
		return (-errno); 
	bcopy (host_struct->h_addr, (char *) &sock_addr.sin_addr.s_addr, host_struct->h_length);
	sock_addr.sin_port = htons(port);
	sock_addr.sin_family = AF_INET;
	if ((connect(sock, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) == -1) && (errno != EINPROGRESS))
                        	return (-errno);  
	FD_ZERO (&set); 
	FD_SET(sock, &set);  
	ret = select (sock+1, NULL, &set, NULL, &tv);
	if (ret == -1)
		return (-errno);
	if (ret == 0)
		return (-ETIME); 

        if (getsockopt (sock, SOL_SOCKET, SO_ERROR, &response, &resplen) == -1)
		return (-errno);

	printf ("getsock ans %ld\n", response);
	if (response != 0)
		return (-response) ;
                       
  	return (sock);
}


int socket_close (int sock)
{
	return (close (sock));
}

int socket_write_header (int sock, char *header)
{
int ret=0;
int nelems=0;

	nelems = strlen (header);
	if ((ret = write (sock, (char *)header, nelems)) < 0)
		return (-errno);
        return (ret);
}
 

int socket_write_data (int sock, unsigned long data_pointer, int nelems)
{
char *data;
int ret=0;

	data = (char *)data_pointer;
	printf ("data[0]= %d\n data[1]=%d\n", data[0], data[1]);
	if ((ret = write (sock, (char *)data, nelems)) < 0)
                return (-errno);

	return (ret); 
}

/*
int socket_read (int sock, char *buf, int nelems)
{
int ret=0;

	if ((ret = read (sock, (char *)buf, nelems*2)) < 0)
                return (-errno);
	        ret = select (sock+1, NULL, &set, NULL, &tv);
        if (ret == -1)
                return (-errno);
        if (ret == 0)
                return (-ETIME);                                                
	return (ret);
}
*/

int socket_read (int sock, char *buf, int nelems, int secs_timeout)
{
int read_ele;
struct timeval  tv;
int ret=0;
fd_set set;

	tv.tv_sec = secs_timeout;
	tv.tv_usec = 0;
	read_ele = read (sock, (char *)buf, nelems);
	printf ("read returned %d\n", read_ele);
	if (read_ele < 0) {
		if (errno != EAGAIN)
			return (-errno);
		FD_ZERO (&set);
		FD_SET (sock, &set);
		ret = select (sock+1, &set, NULL, NULL, &tv);
        	if (ret == -1)
                	return (-errno);
        	if (ret == 0)
                	return (-ETIME);
		 read_ele = read (sock, (char *)buf, nelems);
	}

	return (read_ele);
}

