#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<signal.h>

#define OK		"0"
#define UNKNOWN		"-1"

int new_socket;
extern int shmqid;
extern int running;
extern int overcorr;
extern int display;
extern int outut;
extern int delimg;
extern float VERSION;

void sig_pipe(int n) 
{
//   printf("Broken pipe signal\n");
}

int tcp_server (void)
{
   int create_socket;
   socklen_t addrlen;
   int bufsize = 1024;
   char *buffer = malloc(bufsize);
   char *cmd = malloc(20);
   char *response = malloc (bufsize);
   struct sockaddr_in address;
   int nrec;

   if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0)
   	printf("The socket was created\n");

   address.sin_family      = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port        = htons(5004);

   if (bind(create_socket,(struct sockaddr *) &address, sizeof(address)) == 0)
   	printf("Binding Socket\n");

   listen(create_socket, 3);
   if (signal(SIGPIPE, sig_pipe) == SIG_ERR) {
     printf("Unable to set up signal handler\n");
  }


   addrlen = sizeof(struct sockaddr_in);

again:
   printf("\nReady to accept connections...\n");

   new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen);
//   new_socket = accept(create_socket, &address, &addrlen);
   do {
	printf ("Ready to accept commands ...\n");
   
   	if (new_socket > 0) 
      		printf("The Client %s is connected...\n", inet_ntoa(address.sin_addr));

	nrec = recv(new_socket, buffer, bufsize, 0);
	if (nrec < 0) {
   		close(new_socket);
		goto again;
	}
	printf("Message received: %s\n", buffer); 
	sscanf (buffer, "%s", cmd);
	if (strncmp (cmd, "SHUTDOWN", 8) == 0){
		strcpy (cmd , "bye");
		running = 0;
		strcpy (response, "OK");
	} else if (strncmp (cmd, "NOBIAS", 6) == 0){
		printf ("overcorr is now %d\n", overcorr);
		overcorr = 0;
		strcpy (response, "OK");
	} else if (strncmp (cmd, "BIAS", 4) == 0){
		overcorr = 1;
		printf ("overcorr is now %d\n", overcorr);
		strcpy (response, "OK");
	} else if (strncmp (cmd, "DISP", 4) == 0){
		display = 1;
		printf ("display is now %d\n", display);
		strcpy (response, "OK");
	} else if (strncmp (cmd, "NODISP", 6) == 0){
		display = 0;
		printf ("display is now %d\n", display);
		strcpy (response, "OK");
	} else if (strncmp (cmd, "UT", 2) == 0){
		outut = 1;
		printf ("outut is now %d\n", outut);
		strcpy (response, "OK");
	} else if (strncmp (cmd, "NOUT", 4) == 0){
		outut = 0;
		printf ("outut is now %d\n", outut);
		strcpy (response, "OK");
        } else if (strncmp (cmd, "DELE", 4) == 0){
                delimg = 1;
                printf ("delimg is now %d\n", delimg);
                strcpy (response, "OK");
        } else if (strncmp (cmd, "NODELE", 6) == 0){
                delimg = 0;
                printf ("delimg is now %d\n", delimg);
                strcpy (response, "OK");
	} else if (strncmp (cmd, "GETDISP", 7) == 0){
		sprintf (response, "%d", display);
	} else if (strncmp (cmd, "GETBIAS", 7) == 0){
		sprintf (response, "%d", overcorr);
	} else if (strncmp (cmd, "GETDELE", 7) == 0){
		sprintf (response, "%d", delimg);
	} else if (strncmp (cmd, "VER", 3) == 0){
		sprintf (response, "%6.2f", VERSION);
	} else
		strcpy (response, UNKNOWN);

	printf ("response %s\n\n", response);
	if (send(new_socket, response, strlen (response), 0) < 0){
		close (new_socket);
		goto again;
	}
   } while(strncmp(cmd, "bye", 3)); 
   close(new_socket);
   close(create_socket);
   return (0);
}

