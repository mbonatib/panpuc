/*
	Procedures for SOAR TCS Communications.
	The command protocol is client/server with
	immediate response. A response should never take
	longer than 500 mS.
*/
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <time.h>

#define INVALID_SOCKET 0

static int m_Socket = INVALID_SOCKET;

int OpenTCSSocket(char *host, int port,char *response);
int Demand2TCS(char *command, char *response); 
void CloseTCSSocket();
void flaten(char *line);

/*
	Open the TCS connection. The connection uses
	sockets under TCP/IP.
*/
int OpenTCSSocket(char *host, int nPort, char *response)
{
	union MA
		{
			char line[256];
			int l;
		} ma;
	int n;
	int nResult;
	struct sockaddr_in m_SocketAddress;
	struct timeval tv;
	fd_set fds;

	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Socket == INVALID_SOCKET)
		return(-1);
	if (fcntl(m_Socket, F_SETFL, O_NONBLOCK) == -1)
	{
		close(m_Socket);
		return(-1);
	}
	                                        // Setup the socket address structure
  	m_SocketAddress.sin_addr.s_addr = inet_addr(host);
  	m_SocketAddress.sin_family      = AF_INET;
  	m_SocketAddress.sin_port        = htons(nPort);
	nResult = connect(m_Socket, (struct sockaddr *) &m_SocketAddress, 
		  sizeof(m_SocketAddress));
	if (nResult == -1)
	{
		FD_ZERO(&fds);
		FD_SET(m_Socket, &fds);
		tv.tv_sec = 0;
		tv.tv_usec = 5000;
		nResult = select(m_Socket + 1, NULL, &fds, NULL, &tv);
		if (nResult != 0)
		{				// If we are connected, wait for
						// the acknowledge message.
			FD_ZERO(&fds);
			FD_SET(m_Socket, &fds);
			tv.tv_sec = 0;
			tv.tv_usec = 1500000;
			n = select(m_Socket + 1, &fds, NULL, NULL, &tv); // wait ack
			if (n == 0) {
				strcpy (response, "ERROR timeout in connection\n"); 
				 return(-1);			// timeout
			}
			n = recv(m_Socket, &ma.l, 4, 0);	// get buffer length
			flaten(ma.line);			// into C order
			n = recv(m_Socket, response, ma.l, 0);	// get command response
//			response[ma.l] = '\0';			// NULL is C terminator
			if (!strcmp(response, "DONE"))
				return(0);		// OK
			else {
				strcpy (response, "ERROR authenticating\n"); 
				return(-1);		// Authentication error.
			}
		}		
		else {
			strcpy (response, "ERROR in connection\n"); 
			return(-1);		// Error in connection
		}
	}
	return(0);
}
/*
	Send command to TCS and receive response.
*/

int Demand2TCS(char *command, char *response)
{
	union MA
		{
			char line[256];
			int l;
		} ma;
	int n, ll;
	fd_set fds;
	struct timeval tv;
	
	if (m_Socket == INVALID_SOCKET) {
		strcpy (response, "ERROR invalid socket\n"); 
		 return(-1);
	}
	ll = strlen(command);
	ma.l = ll;					// header is string length
	flaten(ma.line);				// into LabVIEW order
	strcpy(&ma.line[4], command);			// add the command
	n = send(m_Socket, (char *) ma.line, ll+4, 0);	// send the package
	if (n != (ll+4))
	{						// error sending
		close(m_Socket);
		m_Socket = INVALID_SOCKET;
		strcpy (response, "ERROR sending\n"); 
		return(-1);
	}
	FD_ZERO(&fds);
	FD_SET(m_Socket, &fds);
	tv.tv_sec = 0;
	tv.tv_usec = 500000;
	n = select(m_Socket + 1, &fds, NULL, NULL, &tv); // wait for response
	if (n == 0) {
		 strcpy (response, "ERROR timeout waiting response\n"); 
		 return(-1);				// timeout
	}
	n = recv(m_Socket, &ma.l, 4, 0);		// get buffer length
	flaten(ma.line);				// into C order
	n = recv(m_Socket, response, ma.l, 0);		// get command response
//	response[ma.l] = '\0';				// NULL is C terminator
	return(0);
}
/*
	Close the socket
*/
void CloseTCSSocket()
{
	close(m_Socket);
	m_Socket = INVALID_SOCKET;
}
/*
	Cast the byte order from/to LabVIEW/C
*/
void flaten(char *line)
{
	char local[4];
	
	local[3] = line[0];
	local[2] = line[1];
	local[1] = line[2];
	local[0] = line[3];
	line[0] = local[0];
	line[1] = local[1];
	line[2] = local[2];
	line[3] = local[3];
}
