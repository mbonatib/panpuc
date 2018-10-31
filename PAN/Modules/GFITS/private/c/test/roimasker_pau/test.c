#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<unistd.h>
#include <string.h>
#include <errno.h>
#include "roiMask.h"

#define	CMD	1
#define DATA	2


int main(int argc, char *argv[])
{
//char impath[256];
int i;
int display = 0;
int delimg = 0;
int overcorr = 1;

   i = 1;
   if (argc > 1){
        while (i < argc) {
                if (argv[i][0] == '-') {
                        if (argv[i][1] == 'd')
                                delimg = 1;
                        else
                                if (argv[i][1] == 's') {
                                        display = 1;
                                }
                        else
                                if (argv[i][1] == 'n') {
                                        overcorr = 0;
                                }
                }
                i++;
        }
    }
    RMASK_GetImage (argv[1], delimg, display, overcorr, 1);
    RMASK_Close ();

    return (0);
}
