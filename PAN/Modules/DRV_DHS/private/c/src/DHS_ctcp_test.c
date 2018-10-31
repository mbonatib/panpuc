#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "DHS_ctcp_defs.h"

int main(void)
{

int fd=0;
int *buffer;
int cols=10, rows=10, bpp=32;
float bytes_pp;

	bytes_pp = (float) bpp /8;
	buffer = malloc (cols*rows*bytes_pp);
	buffer[0] = 121;
	fd = DHS_connect ("localhost", 5004);
	DHS_sendPixels (fd, (unsigned long) buffer, cols, rows, bpp);
	DHS_close (fd);
	return (0);
}
