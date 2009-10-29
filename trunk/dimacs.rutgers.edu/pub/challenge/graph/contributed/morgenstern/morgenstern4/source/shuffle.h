/*  FILE       : shuffle.h
    WRITTEN BY : Craig Morgenstern
     
    Shuffle the contents of the given linear array of integers.
*/
     
void     shuffle(/* a, n, s, urand */);
     
        /* a     -- the linear array indexed from 0 .. n-1.
           n     -- number of integers in the linear array.
           *s    -- address of an integer seed passed into urand.
           urand -- a random number generator that takes the
                    address of an integer seed and returns a
                    real number in the range (0,1).
        */
