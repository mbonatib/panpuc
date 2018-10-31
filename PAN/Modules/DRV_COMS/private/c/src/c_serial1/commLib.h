/* commLib.h 	08/30/1999	NOAO */

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
 */

/*------------------------------------------------------------------
 * commLib.h
 *
 *-----------------------------------------------------------------*/

#ifndef __COMMLIB_H
#define __COMMLIB_H

extern int comm_open_port(char *ttys, int ib);
extern int comm_close_port(void);
extern int comm_send_msg(char *outString);
extern int comm_get_msg(char *inBuf);
extern void comm_dump_string(char *mess, char *str);
extern void comm_verbose(int flag);
extern int LVcomm_open_port(int *fd, LStrHandle port_name, int baudrate);
extern int LVcomm_close_port(int fd);
extern int LVcomm_send_msg(int fd, LStrHandle msg);
extern int LVcomm_get_comm(int fd, LStrHandle msg);

#endif
