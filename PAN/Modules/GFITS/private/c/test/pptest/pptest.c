#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>


int main(int argc, char *argv[])
{
#if 0
int ret;
   ret = access (argv[0], R_OK | X_OK);
   if (ret< 0)
	printf ("pppath error -%d\n", errno);
   ret = access (argv[1], R_OK);
   if (ret< 0)
	printf ("impath error -%d\n", errno);
#else	
   call_postproc (argv[1], argv[2], NULL, 0);
#endif
   return (0);
}
