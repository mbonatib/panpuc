/* getLib.h       02/17/1999      NOAO    */

/*
 *  Developed 1998 by the National Optical Astronomy Observatories(*)
 *
 * (*) Operated by the Association of Universities for Research in
 *     Astronomy, Inc. (AURA) under cooperative agreement with the
 *     National Science Foundation.
 */

/*
 * modification history
 * --------------------
 * 01a 20may93,gsh - written from the 4M TCP stuff
 * 02a 30aug94,gsh - added strUpperCase
 * 02b 03sep94,gsh - modified checkIfEos
 * 03a 22feb96,gsh - moved to 5.2
 * 03b 27jun96,gsh - moved slew to slew
 * 04a 26nov98,gsh - modified for hydra
 * 05a 26mar99,rcr - ported to Linux.
 * 06a 04feb05,rcr - add support to convert doubles
 */

#ifndef __GETLIB_H
#define __GETLIB_H

#define	ERR_ILLGCMD	1		/* illegal command        */
#define	ERR_ILLGPAR	2		/* illegal parameter      */
#define	ERR_TIMEOUT	3		/* timeout                */
#define	ERR_OK		4		/* ok                     */
#define	ERR_MOVING	5		/* moving                 */
#define	ERR_ALDYMOV	7		/* already moving         */
#define	ERR_NOVAL	8		/* no values defined      */
#define	ERR_ILLGPOS	9		/* illegal position       */
#define	ERR_STKOVER	10		/* stack overflow         */
#define	ERR_STKUNDER	11		/* stack underflow        */
#define	ERR_DOMEDIS	12		/* dome disabled          */
#define	ERR_GDROUT	13		/* guider out of limits   */
#define	ERR_DEVNIN	14		/* device not initialized */
#define ERR_TELNIN      15		/* telescope not inited	  */
#define ERR_DRVOFF	16		/* drives are off	  */
#define ERR_DACWRITE	17		/* failed to write DAC    */



extern char *getToken(char *line, char *token);
extern int checkIfEos(char *line);
extern char *getInteger (char *line, int *num);
extern char *getDouble (char *line, double *num);
extern char *ftoa(double x, char *str);
extern void errorResponse (char *line, int n);

#endif		/*  __GETLIB_H  */
