/*
	test serial communications
*/
#include <stdio.h>
#include "commLib.h"
#include "commLib.c"

main()
{
	char line[80];
	
	if (comm_open_port("/dev/ttyS0") < 0)
	{
		exit(0);
	}
	comm_send_msg("hello world\n");
	if (comm_get_msg(line) > 0)
		printf("%s\n", line); 
	comm_close_port();
	exit(0);
}
