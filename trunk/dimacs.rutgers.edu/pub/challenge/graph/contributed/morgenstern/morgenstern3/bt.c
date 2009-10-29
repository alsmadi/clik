/* FILE: bt.c	WRITTEN BY: Craig Morgenstern */

# include <stdio.h>
# include <malloc.h>
# include "static_graph.h"
# include "color_ops.h"
# include "sat_ops.h"
# include "subgraph.h"

# define maxSGsz 256
# define conflict(v,c) sat_inconflict0(v,c)

static int K; /* number of colors */
static int n; /* number of vertices */

static int *StateVector = NULL;
# define SV(r) (*(StateVector + (r)))

/* ----------------------------  initializeBT  --------------------------------
 */

int	initializeBT()
{
	n = numvertices;
	K = numcc;

	if (StateVector != NULL) free((char *) StateVector);
	StateVector = (int *) malloc((unsigned)(maxSGsz) * sizeof(int));
	if (StateVector == NULL) return(0);
	if (!initialize_subgraph(maxSGsz)) return(0);
	return(1);
}

/* -------------------------  lir  -------------------------------------------
 * deep localized impasse resolution
 */

int	lir(impasse_v, nincr, subn, maxBT, BTcnt)

	int impasse_v, nincr, *subn, maxBT, *BTcnt;
{
	int r, c, v, ilevel, nlim, BTceiling;

	*BTcnt = 0;
	BTceiling = maxBT;
	nlim = nincr;
	start_subgraph(impasse_v);
	for (;;)
	{
	    if (!excise_subgraph(nlim, subn)) return(0);
	    prep_subgraph();

	    v = max_saturated_vertex();  
	    r = 0;  SV(r) = v;  c = -1;  ilevel = -1;
     
    	    while (r >= 0)
    	    {
                 for (++c;  c < K  &&  conflict(v,c);  ++c);
                 if (c < K)
                 {
                     sat_setcolor(v,c);
                     if (++r == *subn) return(1);
		     v = max_saturated_vertex();
		     SV(r) = v;
                     c = -1;
                 }
                 else
                 {
		     if (++(*BTcnt) > BTceiling) return(0);
		     if (r > ilevel) ilevel = r;
		     if (--r >= 0)
		     {
                         v = SV(r);
                         c = colorof(v);
                         sat_uncolor(v);
                     }
		 }
            }
	    nlim += nincr;
	    if (nlim >= maxSGsz) return(0);
	    BTceiling = *BTcnt + maxBT;
	}
}
