/* FILE : urand.c
*  
*                       uniform random number generator
*
*       Reference: "Computer Methods for Mathematical Computations"
*                   by Forsythe, Malcolm and Moler
*
*       CAUTION   -- Assumes 32 bit integer representation.
*/

#       include <math.h>

static  int     a, c, m;
static  double  s;

/*-----------------------------------------------------------------------------
  ------------------------------  init_urand  ---------------------------------
  -----------------------------------------------------------------------------
  Compute length, multiplier and increment for linear congruential
  method and scale factor for converting to floating point.  Must be called 
  exactly once before any calls to urand.
*/

void    init_urand()
{
        m = 1073741824;
        a = 8 * (int)(m * atan(1.0)/8.0) + 5;
        c = 2 * (int)(m * (0.5 - sqrt(3.0)/6.0)) + 1;
        s = 0.5 / (double) m;
}

/*-----------------------------------------------------------------------------
  ------------------------------  urand  --------------------------------------
  -----------------------------------------------------------------------------
  Compute next random number in the range [0,1).  Parameter y is the address of
  a seed value that must be >= 0 initially and should never be altered between 
  calls to urand.  Multiple streams are obtained by maintaining one seed per 
  stream.
*/

double  urand( y )     

        register int    *y;
{
        *y *= a;
        *y += c;
        if ( *y < 0 ) 
        {
            /* integer overflow occured */
            *y += m;
            *y += m;
        }
        return( (double)(*y) * s);
}
