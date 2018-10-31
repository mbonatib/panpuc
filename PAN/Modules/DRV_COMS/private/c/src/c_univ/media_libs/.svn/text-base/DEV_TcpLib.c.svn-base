#include <stdio.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "../getLib.h"
#include "../univ_gendefs.h"
#include<netdb.h>


int DEV_tcp_open (char *info)
{
char *local;
char  token[256];
char  host[128];
int port=-1;
int validp=0;
int validh=0;
int sock;
struct sockaddr_in address;
struct hostent *host_struct;

        local = info;
	validp = validh = 0;
        while (!checkIfEos (local)){
                local = getToken (local, token);
//                printf ("token: %s local: %s\n", token, local);
                if (strcmp (token, "port") == 0) {
                        local = getInteger (local, &port);
                        if ((port == 0) && (errno == EINVAL))
                                return (-errno);
                        validp = 1;
                }
                if (strcmp (token, "host") == 0) {
                        local = getToken (local, host);
                        if (errno == EINVAL)
                                return (-errno);
                        validh = 1;
                }
        }
	if (!validh || !validp){
		printf ("ERROR no port or host defined\n");
		return (-EINVAL);
	}

   if ((host_struct = gethostbyname (host)) == NULL){
           pdebug ("ERROR %d getting_mem\n", errno);
           return (-errno);
   }

   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        pdebug("ERROR %d creating_socket\n", errno);
        return (-errno);
   }

   if (fcntl (sock, F_SETFL, O_NONBLOCK) <0){
                pdebug ("ERROR %d setting_socket", errno);
                close (sock);
                return (-errno);
   }

   pdebug ("TCP: host %s, port %d\n", host, port);
    bcopy (host_struct->h_addr, (char *) &address.sin_addr.s_addr, host_struct->h_length);
   address.sin_family = AF_INET;
   address.sin_port = htons(port);

   if ((connect(sock,(struct sockaddr *) &address, sizeof(address)) == -1) && (errno != EINPROGRESS)) {
        pdebug ("ERROR %d connecting\n", errno);
        close (sock);
        return (-errno);
   }

   return (sock);
}


int DEV_tcp_close (int fd)
{
	return (close(fd));
}
