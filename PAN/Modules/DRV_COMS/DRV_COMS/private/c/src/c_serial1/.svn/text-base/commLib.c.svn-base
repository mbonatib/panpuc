/* commLib.c 	08/30/1999	NOAO */

/* 
 *  Developed 1998 by the National Optical Astronomy Observatories(*)
 *
 * (*) Operated by the Association of Universities for Research in
 *    Astronomy, Inc. (AURA) under cooperative agreement with the
 *    National Science Foundation.
 */

/*
 *  modification history
 *  --------------------
 *  01b 30aug99, rcr - Created.
 *  02a 05dec02, gsh - shared library for LabVIEW
 */

/*------------------------------------------------------------------
 * commLib.c
 *
 *-----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <fcntl.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include "extcode.h"
#include "commLib.h" 

#define MAX_STR  240

#define TTY_ENABLE(flag, value) flag |= value
#define TTY_DISABLE(flag, value) flag &= ~(value)

/*----------------------------------------------------------------------
 *	Local Variables
 *---------------------------------------------------------------------*/

static int Verbose  = 0;

static int fdttys = -1;
static struct termios portio;

/*----------------------------------------------------------------------
 *	Local Function Prototypes
 *---------------------------------------------------------------------*/

/*------------------------------------------------------------------
 * comm_open_port - open channel port.
 *
 * Attempt to open channel port in variable ser. The function returns 0
 * when no problem is encountered, otherwise -1.
 *----------------------------------------------------------------*/
int comm_open_port (char *ttys, int ib)
{
	int baudrate;
	
  if((fdttys = open(ttys, (O_RDWR | O_APPEND))) < 0) {
//    fprintf (stderr, "ERROR: comm_open_port: can't open %s device\n",
//             ttys);
    return (-1);
  }

  if (!isatty(fdttys)) {
//    fprintf (stderr, "ERROR: comm_open_port: can't get tty info for %s\n",
//             ttys);
    return (-2);
  }

  if (ioctl(fdttys, TCGETS, &portio)) {
//    fprintf (stderr, "ERROR: comm_task: can't get serial device info\n");
    return (-3);
  }

  /* input  */
//  TTY_DISABLE(portio.c_iflag, ICRNL);  /* map <CR> to <NL>  */
  TTY_ENABLE(portio.c_iflag, ICRNL);  /* map <CR> to <NL>  */
  TTY_DISABLE(portio.c_iflag, IXON);   /* START/STOP control  */
  TTY_DISABLE(portio.c_iflag, IGNBRK);	/* ignore break  */
  TTY_DISABLE(portio.c_iflag, BRKINT);	/* signal on break  */

  /* output  */
  TTY_ENABLE(portio.c_oflag, CRTSCTS );	/* handshaking */
  TTY_DISABLE(portio.c_oflag, OPOST); 		/* process output  */
  TTY_DISABLE(portio.c_oflag, ONLCR);  	/* map <NL> to <CR><NL>  */

  /* control  */
  TTY_ENABLE(portio.c_cflag, CS8);     /* 8 bits  */
  TTY_DISABLE(portio.c_cflag, PARENB); /* parity  */
  TTY_ENABLE(portio.c_cflag, HUPCL);   /* hangup on close  */

  /* line discipline  */
  TTY_DISABLE(portio.c_lflag, ISIG);   /* check for INTR and QUIT  */
  TTY_ENABLE(portio.c_lflag, ICANON); /* canonical input  */
  TTY_DISABLE(portio.c_lflag, ECHO);   /* echo  */
  TTY_DISABLE(portio.c_lflag, IEXTEN); /* SunOS special characters  */

  /* other  */
  portio.c_cc[VMIN]  = 0;              /* match 'tip' */
  portio.c_cc[VTIME] = 0;
//  portio.c_cc[VEOL] = 13;
  
  switch(ib)
  {
  	case 2400:
		baudrate = B2400;
		break;
  	case 4800:
		baudrate = B4800;
		break;
  	case 9600:
		baudrate = B9600;
		break;
  	case 19200:
		baudrate = B19200;
		break;
  	case 38400:
		baudrate = B38400;
		break;
  	case 57600:
		baudrate = B57600;
		break;
  	case 115200:
		baudrate = B115200;
		break;
	default:
		baudrate = B9600;
		break;
  }

  /* baud rate  */
  if (cfsetospeed(&portio, baudrate) == -1) {
//    fprintf (stderr, "ERROR: comm_open_port: can't set output baud rate\n");
    return(-4);
  }
  if (cfsetispeed(&portio, baudrate) == -1) {
//    fprintf (stderr, "ERROR: comm_open_port: can't set input baud rate\n");
    return(-5);
  }

  if (ioctl(fdttys, TCSETS, &portio)) {
//    fprintf (stderr, "ERROR: comm_open_port: can't set tty info\n");
    return(-6);
  }

  return (0);
}

/*------------------------------------------------------------------
 * comm_close_port - close serial port.
 * 
 * Attempt to close the communication port fdttys. closePort return
 * zero on success, or -1 if an error ocurred.
 *------------------------------------------------------------------*/
int comm_close_port ()
{
  if (close (fdttys) == -1) {
    fprintf (stderr, "ERROR: can't close serial device\n");
    return (-1);
  }

  /*
   * If we get here then we succed in closing the comm port.
   * fdttys goes to its default value.
   */
  fdttys = -1; 

  return (0);
}

/*------------------------------------------------------------------
 * comm_send_msg - send a string through the serial port.
 *
 * This function sends the given string through the serial port along
 * with a newline character '\r'.
 *------------------------------------------------------------------*/
int comm_send_msg (char *outString)
{

  if (Verbose)
     fprintf(stderr, "sendMessage %s\n", outString);

  /* dump_string("ptmc: sending to PTMC:", outString); */

  if (write(fdttys, outString, strlen(outString)) != (int)strlen(outString)) {
      if (Verbose)
         perror("sendMessage: string transmission to PTMC failed");
      return (-1);
  }
  return (0);
}

/*------------------------------------------------------------------
 * comm_get_msg - get a string from the serial port.
 *
 * This function reads a string from the serial port.
 *------------------------------------------------------------------*/
int comm_get_msg (char *inBuf)
{
   int i, ret = 0;  			/*  prime		*/
   struct timeval tv;
   fd_set fds;
   int nResult;

   FD_ZERO(&fds);
	printf ("inside inbuf\n");
   FD_SET(fdttys, &fds);
   tv.tv_sec = 1;
   tv.tv_usec = 0;
   nResult = select(fdttys + 1, &fds, NULL, NULL, &tv);
   if (nResult == 0) 
   {
   	inBuf[0] = (char) NULL;
   	return(-1);
   }
   ret = read (fdttys, inBuf, MAX_STR);

   if (ret == -1) {
      inBuf[0] = (char) NULL;
      if (Verbose)
         perror("getMessage");
   }
   else {
      inBuf[ret] = (char) NULL;   	/*  terminate string  */

      /*
       * Kludge! This will eliminate the 1 in the more significant  
       * bit of each byte we are receiving. I still have to determine
       * what's going on!
       */

      for (i=0; i < ret; i++) {
         inBuf[i] = inBuf[i] & 127;
      }

      if (Verbose) {
         if (ret != 0)
	    fprintf(stderr, "getMessage: ret = %d, inBuf = %s\n", ret, inBuf);
         /* dump_string("getMessage:", inBuf); */
      }


   }

   return(ret);
}

/*------------------------------------------------------------------
 * dump_string - dump a string to the stderr stream.
 *
 * This function dumps a string to the stderr stream character by
 * by character. Each character in the string is presented in four
 * different formats (ASCII, decimal, hexadecimal, octal) along with
 * its position within the string.
 *------------------------------------------------------------------*/
void comm_dump_string(char *mess, char *str)
{
   int i;

   if (Verbose)
   {
      fprintf(stderr, "%s%s\n", mess, str);

      for (i=0; i < (int) strlen(str); i++)
	fprintf(stderr, "\"%1c\":[%d]: 0%o %3d 0x%x\n",
		str[i], i, str[i], str[i], str[i]);
   }
}

/*----------------------------------------------------------------------
 *	Local Variables Get/Set Interface
 *---------------------------------------------------------------------*/

void comm_verbose (int flag) { Verbose = flag; }

/*----------------------------------------------------------------------
*
*	Wrappers for LabVIEW
*
*---------------------------------------------------------------------*/

int LVcomm_open_port(int *fd, LStrHandle port_name, int baudrate)
{
	char buf[256];
	int nchars;
	
	nchars = LStrLen(*port_name);
	strncpy(buf, *(char **) port_name+4, nchars);
	buf[nchars] = '\0';
	*fd = 1;
	return(comm_open_port(buf, baudrate));
}
int LVcomm_close_port(int fd)
{
	comm_close_port();
	return(0);
}
int LVcomm_send_msg(int fd, LStrHandle msg)
{
	char buf[256];
	int nchars;
	
	nchars = LStrLen(*msg);
	strncpy(buf, *(char **) msg+4, nchars);
	buf[nchars] = '\0';
	return(comm_send_msg(buf));
}
int LVcomm_get_msg(int fd, LStrHandle msg)
{
	char buf[256];
	int nchars, err;
	
	if (comm_get_msg(buf) <= 0)
		return(-1);
	nchars = strlen(buf);
	err = NumericArrayResize(uB, 1, (UHandle *) & msg, nchars + 1);
	if (!err)
	{
		LStrLen(*msg) = nchars;
		strcpy(*(char **) msg+4, buf);
		return(0);
	}
	else return(-1);
}
