/* FILE: color_loop.c		WRITTEN BY: Craig Morgenstern */

# include <stdio.h>
# include <malloc.h>

# include "static_graph.h"
# include "color_ops.h"
# include "sat_ops.h"
# include "bt.h"

# define conflict(v,c) sat_inconflict0(v,c)

int	color_loop(verbose, maxBT, SGincr)

	int verbose, maxBT, SGincr;
{
	int subn, BTcnt, bts, success;
	int v, r, c;

	if (! create_sat()) return(0);
	if (! initializeBT()) return(0);

	BTcnt = 0;
	success = 1;
	for (r = 0;  success && r < numvertices;  ++r)
	{
	    v = max_saturated_vertex();
	    for (c = 0;  c < numcc  &&  conflict(v,c);  ++c);
	    if (c < numcc)
		sat_setcolor(v,c);
	    else
		success = 0;
	    
	    if (!success)
	    {
		if (verbose)
		{
		    printf("impasse at level %d: ", r);
		    fflush(stdout);
		}

		subn = 0;
	        success= lir(v, SGincr, &subn, maxBT, &bts);
		BTcnt += bts;

		if (verbose)
		{
		    if (success) printf("resolved: ");
		    printf("BT = %d  subn = %d\n", bts, subn);
		    fflush(stdout);
		}
		if (!success) return(0);
	    }
	}
	printf("Total Backtracks = %d\n", BTcnt);
	return(1);
}
