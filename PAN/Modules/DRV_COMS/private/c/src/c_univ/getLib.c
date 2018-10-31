/* getLib.c      03/25/1999      NOAO    */

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
 * 03c 16oct96,pgr - cleaned up code and added "get.h"
 * 04a 29mar99,rcr - Ported to Linux.
 * 05a 29sep99,rcr - Modified to be part of a kernel module.
 * 05b 05may05,rcr - Add support for user space compilation.
 * 06a 09may05,rcr - Add support for converting doubles.
 */

#ifdef __KERNEL__
#include <linux/kernel.h>
#include <linux/stddef.h>                       /* NULL         */
#include <linux/ctype.h>
#include <rtai.h>
#else
#include <stdlib.h>
#include <stdio.h>
#endif
#include <float.h>
#include <errno.h>
#include <math.h>
#include "getLib.h"

#define ECMX_OK         0
#define ECMX_NOECHO     1
#define ECMX_NOTSENT    2

/*----------------------------------------------------------------------
 * getLib.c
 *
 * This is a collection of functions for parsing and getting numerous
 * parameters.
 *---------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 *	Local Function Prototypes
 *---------------------------------------------------------------------*/

static char *gtkbrk (char *string, char *token, char *bchar);
static double simple_strtod (const char *nptr, char **endptr);

/*----------------------------------------------------------------------
 * gtkbrk - Get next token from string and returns break character.        
 *
 * RETURNS: also updated input pointer. 
 *---------------------------------------------------------------------*/
char *gtkbrk (char *string, char *token, char *bchar)
{
  char c;
  int i;

  i = 0;
  while ((c = *string++)) {
    if (c == ' ' || c == '\t') {	/* skip white space         */
      if (i == 0)		/* no token yet             */
	continue;
      else			/* true break               */
	break;
    }

    if (c < ' ')
      break;
    if (c == ',')		/* break characters         */
      break;
    if (c == ';')
      break;

    *token++ = c;
    i++;
  }

  *token++ = '\0';		/* null terminate the string */
  *bchar = c;			/* break character           */

  if (c == '\0')
    --string;

  return (string);		/* return updated pointer    */
}

/*----------------------------------------------------------------------
 * getToken - get token from command line.
 *
 * RETURNS: updated pointer.
 *---------------------------------------------------------------------*/
char *getToken (char *line, char *token)
{
  char bchar;

  return (gtkbrk (line, token, &bchar));
}

/*----------------------------------------------------------------------
 * checkIfEos - Check if end of sentence.
 *
 *---------------------------------------------------------------------*/
int checkIfEos (char *line)
{
  char token[80];
 
  getToken (line, token);
  if (token[0] == '\0')
    return (1);
  else
    return (0);
 
}

/*----------------------------------------------------------------------
 * getInteger - get an integer from command line.
 *
 * RETURNS: updated pointer.
 *---------------------------------------------------------------------*/
char *getInteger (char *line, int *num)
{
  char *local, token[80], bchar;  

  local = gtkbrk (line, token, &bchar);
#ifdef __KERNEL__
  *num = simple_strtol (token, NULL, 0);
#else
  *num = strtol (token, NULL, 0);
#endif
  return (local);
}

/*----------------------------------------------------------------------
 * getDouble - get a double from command line
 *
 * RETURNS: updated pointer
 *---------------------------------------------------------------------*/
char *getDouble (char *line, double *num)
{
  char *local, token[80], bchar;

  local = gtkbrk (line, token, &bchar);
  *num = simple_strtod (token, NULL);
  return (local);
}

#define fixit(_from, _to) __extension__ ( \
  {                                       \
    union                                 \
    {                                     \
      const _to _copy; _to __p;           \
    } _quesie;                            \
    _quesie._copy = _from;                \
    _quesie.__p;                          \
  })

/*----------------------------------------------------------------------
 * simple_strtod -
 *
 * These functions shall convert the initial portion of the string
 * pointed to by nptr to double, float, and long double representation,
 * respectively.
 *---------------------------------------------------------------------*/
double simple_strtod (const char *nptr, char **endptr )
{
  long double result;   /* result */
  long double fract;    /* fract */
  int  expo, sign, esign, i, flags = 0, errno;

  result = 0.0;
  sign   = 1;
  expo   = 0;
  esign  = 1;

  if ( endptr )
    *endptr = fixit( nptr, char * );

  while( isspace( *nptr ) || iscntrl( *nptr ) )
    nptr++;

  if ( *nptr == '+' )
  {
    nptr++;
  } else if ( *nptr == '-' )
  {
    sign = -1;
    nptr++;
  }

  while ( isdigit( *nptr ) )
  {
    flags  |= 1;
    result *= 10.0;
    result += ( *nptr - '0' );
    nptr++;
  }/* while ( isdigit( *nptr ) ) */

  if ( *nptr == '.' )
  {
    fract = 0.1L;
    nptr++;

    while ( isdigit( *nptr ) )
    {
      flags |= 2;
      result += fract * (*nptr - '0');
      nptr++;
      fract *= 0.1L;
    }/* while ( isdigit( *nptr ) ) */
  }/* if ( *nptr == '.' ) */

  if ( !flags )
    return 0;

  if ( endptr )
    *endptr = fixit( nptr, char * );

  if ( ( *nptr == 'e' ) || ( *nptr == 'E')
    || ( *nptr == 'd' ) || ( *nptr == 'D' ))
  {
    nptr++;

    if ( *nptr == '+' )
    {
      nptr++;
    } else if ( *nptr == '-' )
    {
      nptr++;
      esign = -1;
    }

    if ( !isdigit( *nptr ) )
      return ( result * sign );

    while ( isdigit( *nptr ) )
    {
      expo *= 10;
      expo += *nptr - '0';
      nptr++;
    }/* while ( isdigit( *nptr ) ) */
  }/* if ( ( *nptr == 'e' ) || ( *nptr == 'E' ) ) */

  /* Detection of overflow
   */
  if ( expo < 0 )
  {
    errno  = ERANGE;
    result = 0xFFFFFFFF;
  } else if ( esign < 0 )
  {
    for ( i = 1; i <= expo; i++ )
    {
      result *= 0.1L;

      if ( result < DBL_MIN )
      {
        errno = ERANGE;
        result = 0.0;
        break;
      }/* if ( result < DBL_MIN) */
    }/* for ( i = 1; i <= expo; i++ ) */
  } else
  {
    for ( i = 1; i <= expo; i++ )
    {
      result *= 10.0;

      if (result > DBL_MAX)
      {
        errno  = ERANGE;
        result = 0xFFFFFFFF;
        break;
      }/* if (result > DBL_MAX) */
    }/* for ( i = 1; i <= expo; i++ ) */
  }/* end of if elseif else */

  if ( endptr )
    *endptr = fixit( nptr, char * );

  return ( result * sign );
}

/*----------------------------------------------------------------------
 * ftoa - convert a double to string
 *
 *----------------------------------------------------------------------*/
char *ftoa(double x, char *str)
{
  char s = (x >= 0 ? ' ' : '-');
  int tmp1, tmp2;

  x = fabs(x);
  tmp1 = floor(x);
  tmp2 = 1e6*(x - tmp1);
  sprintf(str, "%c%d.%06d", s, tmp1, tmp2);
  return str;
}

/*----------------------------------------------------------------------
 * errorResponse - Command error handler.
 *
 * Forms an error message on 'line'.
 *---------------------------------------------------------------------*/
void errorResponse (char *line, int n)
{
  switch (n) {
  case ERR_OK:
    sprintf (line, "ok");
    break;
  case ECMX_NOECHO:
    sprintf (line, "command echo not received");
    break;
  case ERR_ILLGPAR:
    sprintf (line, "error: illegal parameter");
    break;
  default:
    *line = '\0';
    break;
  }
}
