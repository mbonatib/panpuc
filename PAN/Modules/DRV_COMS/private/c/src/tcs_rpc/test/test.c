#include <stdio.h>
#include <rpc/rpc.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <errno.h>
#include "../tcsCmd.h"


int main (void)
{
char *buffer;
int ref;
int err;

	if ((ref=tcsOpen ("139.229.12.8", &err)) == 0){
		printf ("error opening (%d)\n", err);
		return (-1);
	}
	printf ("reference %d\n", ref);
	buffer = malloc (8192);
	while (1){
		sleep (2);
		strcpy (buffer, "info\n");
		if ((err=sendTCS (ref, buffer, 5000)) < 0) {
			printf ("error: %s\n", buffer);
		} else {
			printf ("resp: %s\n", buffer);
		}
	}
	tcsClose (ref);
}
	
