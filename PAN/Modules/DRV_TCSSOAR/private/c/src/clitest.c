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
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <time.h>

#define INVALID_SOCKET 0


int OpenTCSSocket(char *host, int port, char *response);
int Demand2TCS(char *command, char *response); 
void CloseTCSSocket();
void flaten(char *line);
int port=30085;
/*
	Test example for SOAR communications
	The port number is 30040
*/
int main(int argc, char *argv[])
{
	char host[80], command[80], response[256];
	int l;
	
	if (argc < 2)
	{
		printf("Usage: tsc IP number\n");
		return(0);
	}
	strcpy(host, argv[1]);
	port = strtol (argv[2], NULL, 10);
	if (OpenTCSSocket(host, port, response) < 0)
	{
		printf("Error openning socket (%s)\n", response);
		return(0);
	}
	while(1)
	{
		printf("-> ");
		fgets(command, 80, stdin);
		if (command[0] == 'q')		// quit on 'q' command
		{
			CloseTCSSocket();
			return(0);
		}
		l = strlen(command);
		command[l-1] = '\0';		// get rid of end of line
		if (Demand2TCS(command, response) < 0)
		{
			printf("error in command %s\n", command);
			continue;
		}
		printf("*** %s\n", response);
	}
}

