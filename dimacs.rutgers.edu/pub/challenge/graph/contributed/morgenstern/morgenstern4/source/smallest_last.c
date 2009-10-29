/* FILE       : smallestL_last.c
   WRITTEN BY : Craig Morgenstern
*/
     
#	include "graph_globals.h"
#    	include "graph_ops.h"
     
void	SmallestLast( a )
     
    	int    	a[];
{
    	int    	dlist[ maxD ], next[ numV ], prev[ numV ];
	char	deg[ numV ];
        int	d, i, n, v, w;
     
    	n = numvertices();
    	if ( n == 0 ) return;
    	i = 0;
    	for ( d = 0;  d < maxD;  ++d )  dlist[d] = NIL;
    	for ( i = 0;  i < n;  ++i )
    	{
            v = a[i];
            d = degreeof(v);
            deg[v] = d;
            prev[v] = NIL;
            next[v] = dlist[d];
            if ( next[v] != NIL )  prev[ next[v] ] = v;
            dlist[d] = v;
        }
     
    	for ( d = 0, i = n-1;  i >= 0;  --i )
    	{
            while ( dlist[d] == NIL )  ++d;
            v = dlist[d];
            deg[v] = NIL;
            a[i] = v;
            if ( next[v] != NIL )
            prev[ next[v] ] = NIL;
            dlist[d] = next[v];
            for ( w = firstadj(v);  w != NIL;  w = nextadj(v) )
            if ( deg[w] != NIL )
            {
                 if ( next[w] != NIL ) prev[ next[w] ] = prev[w];
                 if ( prev[w] != NIL )
                      next[ prev[w] ] = next[w];
                 else
                      dlist[ deg[w] ] = next[w];
                 --deg[w];
                 prev[w] = NIL;
                 next[w] = dlist[ deg[w] ];
                 if ( next[w] != NIL ) prev[ next[w] ] = w;
                 dlist[ deg[w] ] = w;
                 if ( deg[w] < d ) d = deg[w];
            }
    }
}
