/*  FILE       : shuffle.c
    WRITTEN BY : Craig Morgenstern
     
    Shuffle the contents of the given linear array of integers.
*/
     
/*----------------------------------------------------------------------------
  ----------------------------  shuffle  -------------------------------------
  ----------------------------------------------------------------------------*/
     
void    shuffle( a, n, s, urand )
     
        register int    *a, n;
        int             *s;
     
        /* a  contains  n  entries, indexed from 0 .. n-1 */
        /* s  is the address of an integer seed for urand */
     
        double  (*urand)();
        /* uniform random number generator; takes the address of an
           integer seed, s, and returns a real in the range (0,1).  */
     
{
        register int     *i, *j, t;
     
        if ( n <= 1 ) return;
        for ( j = a + n - 1;  n > 1;  --j, --n )
        {
             i = a + (int)((*urand)(s) * n);
             /* assumes double to int conversion by truncation */
     
             t = *i;   *i = *j;   *j = t;  /* swap */
        }
}
