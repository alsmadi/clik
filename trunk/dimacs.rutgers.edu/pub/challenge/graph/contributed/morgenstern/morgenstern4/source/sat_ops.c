/* FILE       : sat_ops.c
   WRITTEN BY : Craig Morgenstern
     
   ASSUMES that the graph already exists and that the ClassOps package
       has been initialized.  Any change to the graph after calling
       create_sat will not be known in this package and can result
       in incorrect results.
     
   ASSUMES that all vertices are contiguously labeled 0 ... n-1.
*/
     
#    	include "graph_globals.h"
#    	include "graph_ops.h"
#    	include "class_ops.h"
#    	include "shuffle.h"
     
static	char	ACcnt[numV][maxC];  /* Adjacent Color count */

static	int	Sweight[numV],      /* Saturation weights */
		H[maxC][maxD],      /* Saturation Heap */
		NEHlist_cnt[maxC],  /* NonEmpty H list counts */
		prev[numV], next[numV], firsttime, firstv;
     
#	define	putSweight(v,c,d)  Sweight[v] = c*maxD+d
#	define	getSdeg(v)	   (Sweight[v] / maxD)
#	define	getUdeg(v)	   (Sweight[v] % maxD)

/*-----------------------------------------------------------------------------
  -------------------------  create_sat  --------------------------------------
  -----------------------------------------------------------------------------
*/
     
void    create_sat( n, s, urand )
     
        int    n;            /* number of vertices */
        int    *s;           /* address of seed for random number stream */
        double (*urand)();   /* random number generator */
{
        int    v,        /* vertex label */
               c,        /* color label */
	       d,	 /* vertex degree */
               a[numV];  /* hold vertex label permutation */
     
        for ( v = 0;  v < n;  ++v )  a[v] = v;
        shuffle( a, n, s, *urand );

	firsttime = 1;  firstv = a[1];

	for ( c = 0;  c < maxC;  ++c )  
	{
	     NEHlist_cnt[c] = 0;
	     for ( d = 0;  d < maxD;  ++d )  H[c][d] = NIL;
	}

        for ( v = 0;  v < n;  ++v ) 
        {
             d = degreeof(a[v]);
	     putSweight(a[v], 0, d);
	     if ( H[0][d] == NIL )  ++NEHlist_cnt[0];
	     prev[a[v]] = NIL;
	     next[a[v]] = H[0][d];
	     H[0][d] = a[v];
	     if ( next[a[v]] != NIL )  prev[ next[a[v]] ] = a[v];
             for ( c = 0;  c < maxC;  ++c ) ACcnt[a[v]][c] = 0;
        }
}
     
/*-----------------------------------------------------------------------------
  ------------------------  max_saturated_vertex  -----------------------------
  -----------------------------------------------------------------------------
*/
     
int     max_saturated_vertex()
{
	int	i, c, d;

	if ( firsttime )
	{
	     firsttime = 0;
	     return( firstv );
	}

	for ( c = maxC-1;  c >= 0 && NEHlist_cnt[c] == 0;  --c );
	d = -1;  i = NEHlist_cnt[c];
	do 
	{
	     ++d;
	     if ( H[c][d] != NIL )  --i;
	}
	while ( i > 0 );
	return( H[c][d] );
}
     
/*-----------------------------------------------------------------------------
  ---------------------------  sat_setcolor  ----------------------------------
  -----------------------------------------------------------------------------
*/
     
void    sat_setcolor( v, c )
     
        int	v,  /* vertex label */
                c;  /* color label */
{
        int     w,  /* vertex label */
		Sdeg, Udeg, newSdeg, newUdeg;
     
        for ( w = firstadj(v);  w != NIL;  w = nextadj(v) )
        {
             /* maintain saturation and uncolored degrees for all vertices */
             Sdeg = getSdeg(w);  Udeg = getUdeg(w);
             newUdeg = Udeg-1;   newSdeg = Sdeg;
             if ( (++ACcnt[w][c]) == 1 ) ++newSdeg;
	     putSweight( w, newSdeg, newUdeg );
     
             if ( colorof(w) == NIL )  /* w is in the heap */
	     { 
	          /* Delete w */
		  if ( prev[w] == NIL )
		       H[Sdeg][Udeg] = next[w];
		  else
		       next[ prev[w] ] = next[w];
		  if ( next[w] != NIL )  prev[ next[w] ] = prev[w];
		  if ( H[Sdeg][Udeg] == NIL ) --NEHlist_cnt[Sdeg];

		  /* Insert w with new saturation weight */
		  if ( H[newSdeg][newUdeg] == NIL ) ++NEHlist_cnt[newSdeg];
	          prev[w] = NIL;
	          next[w] = H[newSdeg][newUdeg];
	          H[newSdeg][newUdeg] = w;
		  if ( next[w] != NIL )  prev[ next[w] ] = w;
	     }
        }
     
        setcolor(v,c);
        /* Assumes v is in the heap (colorof(v) == NIL) */
	/* delete v */
        Sdeg = getSdeg(v);  Udeg = getUdeg(v);
	if ( prev[v] == NIL ) 
	     H[Sdeg][Udeg] = next[v]; 
	else 
	     next[ prev[v] ] = next[v];
  	if ( next[v] != NIL )  prev[ next[v] ] = prev[v];
	if ( H[Sdeg][Udeg] == NIL ) --NEHlist_cnt[Sdeg];
}
     
/*-----------------------------------------------------------------------------
  ---------------------------  sat_uncolor  -----------------------------------
  -----------------------------------------------------------------------------
*/
     
void    sat_uncolor(v)
     
        int     v;  /* vertex label */
{
        int     w,  /* vertex label */
                c,  /* color label */
		Sdeg, Udeg, newSdeg, newUdeg;
     
        c = colorof(v);
        for ( w = firstadj(v);  w != NIL;  w = nextadj(v) )
        {
             /* maintain saturation and uncolored degrees for all vertices */
             Sdeg = getSdeg(w);  Udeg = getUdeg(w);
             newUdeg = Udeg+1;   newSdeg = Sdeg;
             if ( (--ACcnt[w][c]) == 0 ) --newSdeg;
	     putSweight( w, newSdeg, newUdeg );
     
             if ( colorof(w) == NIL )  /* w is in the heap */
	     { 
	          /* Delete w */
		  if ( prev[w] == NIL )
		       H[Sdeg][Udeg] = next[w];
		  else
		       next[ prev[w] ] = next[w];
		  if ( next[w] != NIL )  prev[ next[w] ] = prev[w];
		  if ( H[Sdeg][Udeg] == NIL ) --NEHlist_cnt[Sdeg];

		  /* Insert w with new saturation weight */
		  if ( H[newSdeg][newUdeg] == NIL ) ++NEHlist_cnt[newSdeg];
	          prev[w] = NIL;
	          next[w] = H[newSdeg][newUdeg];
	          H[newSdeg][newUdeg] = w;
		  if ( next[w] != NIL )  prev[ next[w] ] = w;
	     }
        }
     
        uncolor(v);
        /* Assumes v is not in the heap (colorof(v) != NIL) */
	/* insert v */
        Sdeg = getSdeg(v);  Udeg = getUdeg(v);
	if ( H[Sdeg][Udeg] == NIL ) ++NEHlist_cnt[Sdeg];
	prev[v] = NIL;
	next[v] = H[Sdeg][Udeg];
	H[Sdeg][Udeg] = v;
	if ( next[v] != NIL )  prev[ next[v] ] = v;
}
     
/*-----------------------------------------------------------------------------
  -----------------------  sat_inconflict  ------------------------------------
  -----------------------------------------------------------------------------
*/
     
int	sat_inconflict( v, c )
     
        int	v,  /* vertex label */
                c;  /* color label */
{
        return( ACcnt[v][c] > 0 );
}
     
/*-----------------------------------------------------------------------------
  -----------------------  sat_numAdjColors  ----------------------------------
  -----------------------------------------------------------------------------
*/
     
int     sat_numAdjColors( v, c )
     
        int     v,  /* vertex label */
                c;  /* color label */
{
        return( ACcnt[v][c] );
}
