#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{

int fd;
int ret;
char resp[2048];

	fd = UNIV_Open ("serial", "port 0, brate 9600, data 7, parity odd");
	if (fd <= 0) {
		printf ("error opening port (%d)\n", fd);
		return (fd);
	}
	printf ("open ok\n");

	strcpy (resp, "*IDN?");
	if ((ret == UNIV_Write (fd, resp, 5)) < 0){
		printf ("error writing to port (%d)\n", ret);
		return (ret);
	}
	printf ("write ok\n");
	if ((ret == UNIV_Read (fd, resp, 5)) < 0){
		printf ("error reading from port (%d)\n", ret);
		return (ret);
	}
	printf ("response: %s\n", resp);
	if ((ret == UNIV_Close (fd)) < 0){
		printf ("error closing port (%d)\n", ret);
		return (ret);
	}
	return (0);
}
	
		
	
