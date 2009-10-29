/* FILE: urand.h	WRITTEN BY: Craig Morgenstern */

/* NOTE -- Assumes 32 bit integer and unsigned representations.
 * NOTE -- urand right shifts so that only the 24 or so most significant 
 *	   bits are used.  The least significant bits are not very random (the 
 *	   rightmost bit toggles between 0 and 1 on each call).  
 *
 * --------------------  urand and drand  -------------------------------------	
 * Two pseudo-random number generator macros.
 * CAUTION -- assumes 32 bit unsigned integer representation.
 * 
 * drand returns a uniformly random double in the range [0,1).
 * urand returns a uniformly random unsigned integer in the range 0,..,2^24-1.
 * 
 * The parameter s is the address of an unsigned integer seed.  
 * This seed should be initialized to an arbitrary unsigned integer prior 
 * to the first call to urand or durand and never changed thereafter.  
 * Multiple streams are obtained by maintaining one seed per stream. 
 */

# define urand(s) ((*(s) = *(s) * 3373259429 + 907633385) >> 8)
# define drand(s) ((*(s) = *(s) * 3373259429 + 907633385) / 4294967296.0)
