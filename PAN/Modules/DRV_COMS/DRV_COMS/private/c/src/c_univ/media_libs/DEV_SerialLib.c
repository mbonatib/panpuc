#include <stdio.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include "../univ_gendefs.h"
#include "../getLib.h"

long BAUD;
long DATABITS;
long STOPBITS;
long PARITYON;
long PARITY;

int DEV_serial_set_port();
struct termios oldtio;
short oldtio_valid=0;

int DEV_serial_close (int fd)
{

	if (oldtio_valid){
		tcsetattr(fd,TCSANOW,&oldtio);
		oldtio_valid = 0;
		close (fd);
	} else
		close (fd);
	pdebug ("Serial device closed\n");
	return (0);
}


int DEV_serial_open (char *info)
{
char *local;
char  token[128];
int brate=9600;
int data_bits=8;
int stop_bits=1;
int parit=0;
int iport=-1;
int valid=1;
int fd;
int timeout=120;
int block_mode=1;

        local = info;
        while (!checkIfEos (local)){
                local = getToken (local, token);
		printf ("SerialParse: %s\n", local);
                if (strcmp (token, "port") == 0) {
                        local = getInteger (local, &iport);
                        if ((iport == 0) && (errno == EINVAL))
                                return (-errno);
                        valid = 1;
                }
                if (strcmp (token, "brate") == 0) {
                        local = getInteger (local, &brate);
                        if ((brate == 0) && (errno == EINVAL))
                                return (-errno);
                }
                if (strcmp (token, "data") == 0) {
                        local = getInteger (local, &data_bits);
                        if ((brate == 0) && (errno == EINVAL))
                                return (-errno);
                }
                if (strcmp (token, "stop") == 0) {
                        local = getInteger (local, &stop_bits);
                        if ((brate == 0) && (errno == EINVAL))
                                return (-errno);
                }
              if (strcmp (token, "timeout") == 0) {
                        local = getInteger (local, &timeout);
                        if (errno == EINVAL)
                                return (-errno);
                }
              if (strcmp (token, "block") == 0) {
                        local = getInteger (local, &block_mode);
                        if (errno == EINVAL)
                                return (-errno);
                }
              if (strcmp (token, "parity") == 0) {
                	local = getToken (local, token);
                        if (errno == EINVAL)
                                return (-errno);
			if (strcmp (token, "odd") == 0) {
				parit = 1;
			} else if (strcmp (token, "even") == 0) {
				parit = 2;
			} else 
				parit = 0;
                }
        }
        pdebug ("port %d, brate %d, data %d, stop %d, timeout %d, block %d, parity %d\n", iport, brate, data_bits, stop_bits, timeout, block_mode, parit);
	if (iport < 0){
		pdebug ("No PORT defined!, assuming 0\n");
		iport = 0;
	}
        switch (brate){
         case 38400:
         default:
            BAUD = B38400;
            break;
         case 19200:
            BAUD  = B19200;
            break;
         case 9600:
            BAUD  = B9600;
            break;
         case 4800:
            BAUD  = B4800;
            break;
         case 2400:
            BAUD  = B2400;
            break;
         case 1800:
            BAUD  = B1800;
            break;
         case 1200:
            BAUD  = B1200;
            break;
         case 600:
            BAUD  = B600;
            break;
         case 300:
            BAUD  = B300;
            break;
         case 200:
            BAUD  = B200;
            break;
        }  //end of switch baud_rate
      switch (data_bits)
      {
         case 8:
         default:
            DATABITS = CS8;
            break;
         case 7:
            DATABITS = CS7;
            break;
         case 6:
            DATABITS = CS6;
            break;
         case 5:
            DATABITS = CS5;
            break;
      }  //end of switch data_bits
      switch (stop_bits)
      {
         case 1:
         default:
            STOPBITS = 0;
            break;
         case 2:
            STOPBITS = CSTOPB;
            break;
      }  //end of switch stop bits
      switch (parit)
      {
         case 0:
         default:                       //none
            PARITYON = 0;
            PARITY = 0;
            break;
         case 1:                        //odd
            PARITYON = PARENB;
            PARITY = PARODD;
            break;
         case 2:                        //even
            PARITYON = PARENB;
            PARITY = 0;
            break;
      }  //end of switch parity

	if ((fd = DEV_serial_set_port (iport)) < 0){
		DEV_serial_close(fd);
		return (fd);
	}
	return (fd);
}

int DEV_serial_set_port (int port)
{
struct termios newtio;
char devicename[30];
int fd;

	sprintf (devicename, "/dev/ttyS%d", port);
	pdebug ("opening port %d\n", port);
	fd = open (devicename, O_RDWR | O_NOCTTY);
        if (fd < 0) {
		printf ("Error %d opening serial port %s\n", errno, devicename);
        	return (-errno);
	}

	pdebug ("getting atributes (port fd %d ...\n", fd);
	tcgetattr (fd, &oldtio);
	oldtio_valid = 1;
//	newtio.c_cflag = BAUD | CRTSCTS | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD;
	newtio.c_cflag = BAUD | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD;
//ignore paroty errors, convert CR (Carriage Return) into NL (terminating character)
	newtio.c_iflag = IGNPAR | ICRNL;
//set canonical processing (CR will terminate read)
	newtio.c_lflag = ICANON;
	
	pdebug ("setting new atributes ...\n");
	tcsetattr(fd,TCSANOW,&newtio);
	pdebug ("setting as non blocking ...\n");
        if (fcntl (fd, F_SETFL, O_NONBLOCK) < 0)
               return (-errno);
#if 0
//signal handler for reading
struct sigaction saio;
        saio.sa_handler = COSread_handler;
        sigemptyset (&saio.sa_mask);
        saio.sa_flags = 0;
        saio.sa_restorer = NULL;
        sigaction (SIGIO, &saio,NULL);
#endif
	pdebug ("setting ownership ...\n");
        fcntl (fd, F_SETOWN, getpid ());
	pdebug ("serial port set OK (fd %d) ...\n", fd);
        return (fd);
}
