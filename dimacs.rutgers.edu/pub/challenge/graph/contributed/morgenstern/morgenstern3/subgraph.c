/* FILE: subgraph.c	WRITTEN BY: Craig Morgenstern */

# include <stdio.h>
# include <malloc.h>
# include "static_graph.h"
# include "color_ops.h"
# include "sat_ops.h"

static int head, tail, *Qp, *Qq;
static int *queue = NULL;
char *onqueue = NULL;
# define Q(i) (*(queue + (i)))
# define onQ(v) (*(onqueue + (v)))

int 	initialize_subgraph(maxSGsz)

	int maxSGsz;
{

	int i;

	if (queue != NULL) free((char *) queue);
	queue = (int *) malloc((unsigned)(maxSGsz) * sizeof(int));
	if (queue == NULL) return(0);

	if (onqueue != NULL) free(onqueue);
	onqueue = malloc((unsigned) numvertices);
	if (onqueue == NULL) return(0);

	head = -1;  tail = -1;
	Qp = NULL;  Qq = NULL;
	for (i = 0;  i < numvertices;  ++i) onQ(i) = 0;
	return(1);
}

void	start_subgraph(impasse_v)

	int impasse_v;
{
	int i;

	for (i = 0;  i < tail;  ++i) onQ(Q(i)) = 0;
	sat_makedirty(impasse_v);
	head = 0;  tail = 1;
	Q(0) = impasse_v;  onQ(impasse_v) = 1;
	Qp = NULL;  Qq = NULL;
}

void	prep_subgraph()
{
	int i, v;

	for (i = 0;  i < tail;  ++i) 
	{
	    v = Q(i);
	    if (colorof(v) != nocolor) sat_uncolor(v);
	}
}

int	excise_subgraph(nlim, subn)

	int nlim, *subn;
{
	int t, v, w;

	t = tail;
	while (Qp != Qq  &&  tail < nlim)
	{
	   w = *Qp;
	   if (sat_dirty(w) && !onQ(w))
	   {
	       Q(tail++) = w; 
	       onQ(w) = 1;
	   }
	   ++Qp;
	}

	while (head < tail  &&  tail < nlim)
	{
	    v = Q(head++);
	    for (Qp = adjstart(v), Qq = adjstop(v); Qp != Qq && tail<nlim; ++Qp)
	    {
	        w = *Qp;
	        if (sat_dirty(w) && !onQ(w))
	        {
	            Q(tail++) = w;
		    onQ(w) = 1;
	        }
	    }
	}
	*subn = tail;
	return(tail > t);
}
