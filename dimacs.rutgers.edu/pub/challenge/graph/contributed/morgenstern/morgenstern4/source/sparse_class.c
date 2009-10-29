/* FILE       : sparse_class.c
   WRITTEN BY : Craig Morgenstern
*/

#    include "graph_globals.h"

#    define  swap(i,j)   { int t;  t = i;  i = j;  j = t; }
        	/* swap two integer variables */
     

/******************************************************************************
 ***************************  GRAPH OPS  **************************************
 ******************************************************************************
*/
     
     
#    define  updateH      if (D[H] == 0) for (--H; H!=NIL && D[H]==0; --H );
                /* Maintain H; always the highest non-isolated vertex label */
     
static  int    	*adjnode,      /* Node pool for adjacency lists */
           	*next,
           	pristine,
           	freeptr = NIL;
     
static	int     A[ numV ],     /* adjacency list heads                    */
        	D[ numV ],     /* vertex degrees                          */
		S[ numV ],     /* scan cursers for firstadj and nextadj   */
		H = NIL,       /* highest nonisolated vertex label        */
        	E = 0,         /* maintain a count of the number of edges */
        	V = 0;         /* a count of num of nonisolated vertices  */
     
/*-----------------------------------------------------------------------------
  ---------------------  new  and  dispose  -----------------------------------
  -----------------------------------------------------------------------------
*/
     
static void	dispose(p)
     
    	int    p;
{
    	*(next+p) = freeptr;
    	freeptr = p;
}
     
static int	new()
{
    	int    p;
    	 
    	if ( freeptr != NIL )
    	{
             p = freeptr;
             freeptr = *(next + freeptr);
        }
        else
             p = pristine++;
        return(p);
}
     
/*----------------------------------------------------------------------------
  -----------------------  createvertices  -----------------------------------
  ----------------------------------------------------------------------------
  Initialize the package so that all vertices are isolated.
*/
     
void	createvertices( n , e )
     
        int   n,   /* Desired number of isolated vertices, labeled 0 .. n-1 */
	      e;   /* Desired number of edges possible */
{
        int   i;
     
        if ( V > 0 )
	{
	     free( (char *) adjnode );
	     free( (char *) next );
	}

	adjnode = (int *) calloc( (unsigned)(e*2), sizeof(int) );
	next = (int *) calloc( (unsigned)(e*2), sizeof(int) );

        E = 0;
        V = 0;
        H = NIL;
        for ( i = 0;  i < n;  ++i )
        {
             D[ i ] = 0;
             A[ i ] = NIL;
        }
}
     
/*----------------------------------------------------------------------------
  -----------------------------  addedge  ------------------------------------
  ----------------------------------------------------------------------------
  Add an edge between v1 and v2.  Assumes that v1 and v2 are not adjacent and
  that v1 <> v2.
*/
     
void	addedge( v1, v2 )
     
        int    v1, v2;      /* vertex labels; add edge (v1,v2) */
{
        int    p;
     
        p = new();
        *(next+p) = *(A+v1);
        /* *(A+v1) = p; */ A[v1] = p;
        *(adjnode+p) = v2;
         
        p = new();
        *(next+p) = *(A+v2);
        *(A+v2) = p;
        *(adjnode+p) = v1;
         
        ++E;
        if ( ++( *(D+v1) ) == 1 )
        {
             ++V;
             if ( v1 > H )  H = v1;
        }
        if ( ++( *(D+v2) ) == 1 )
        {
             ++V;
             if ( v2 > H )  H = v2;
        }
}
     
/*----------------------------------------------------------------------------
  ----------------------------  removeedge  ----------------------------------
  ----------------------------------------------------------------------------
  Remove the edge between v1 and v2.  Assumes that v1 and v2 are adjacent and
  that v1 <> v2.
*/
     
void	removeedge( v1, v2 )
     
        int       v1, v2;    /* endpoints of the edge */
{
    	int    p, q;
     
    	for ( q = NIL, p = A[v1];  *(adjnode+p) != v2;  q = p, p = *(next+p) );
    	if ( q == NIL )
             A[v1] = *(next+p);
        else
             *(next+q) = *(next+p);
        dispose(p);
     
        for ( q = NIL, p = A[v2];  *(adjnode+p) != v1;  q = p, p = *(next+p) );
        if ( q == NIL )
             A[v2] = *(next+p);
        else
             *(next+q) = *(next+p);
        dispose(p);
     
        --E;
        if ( --D[ v2 ] == 0 )  --V;
        if ( --D[ v1 ] == 0 )  --V;
        updateH;
}
     
/*----------------------------------------------------------------------------
  ----------------------------  adjacent  ------------------------------------
  ----------------------------------------------------------------------------
  Does there exist an edge between v1 and v2?
*/
     
int	adjacent( v1, v2 )
     
        int	v1, v2;   /* vertex labels */
{
    	int    p;
     
        if ( *(D+v1) < *(D+v2) ) swap(v1, v2)
        for ( p = *(A+v1);  p != NIL && *(adjnode+p) != v2;  p = *(next+p) );
        return( p != NIL );
}
     
/*----------------------------------------------------------------------------
  -----------------------------  degreeof  -----------------------------------
  ----------------------------------------------------------------------------
  Return the degree of v.
*/
     
int	degreeof( v )
     
        int  v;   /* vertex label */
{
        return( D[ v ] );
}
     
/*---------------------------------------------------------------------------
  ----------------------------  numedges  -----------------------------------
  ---------------------------------------------------------------------------
  Return the number of edges in the graph.
*/
     
int	numedges()
{
        return( E );
}
     
/*---------------------------------------------------------------------------
  ----------------------------  numvertices  --------------------------------
  ---------------------------------------------------------------------------
  Return the number of nonisolated vertices in the graph.
*/
     
int	numvertices()
{
        return( V );
}
     
/*---------------------------------------------------------------------------
  ---------------------------  highvertex  ----------------------------------
  ---------------------------------------------------------------------------
  Return the highest label that is a non-isolated vertex.
*/
     
int     highvertex()
{
        return( H );
}
     
/*----------------------------------------------------------------------------
  -----------------------------  removevertex  -------------------------------
  ----------------------------------------------------------------------------
  Make v an isolated vertex.
*/
     
void	removevertex( v1 )
     
        int     v1;   /* vertex label -- make v1 isolated */
{
        int     v2;   /* vertex label */
     
        if ( D[v1] == 0 )  return;
        do
        {
             v2 = *(adjnode + A[v1]);
             removeedge( v1, v2 );
        }
        while ( A[v1] != NIL );
}
     
/*----------------------------------------------------------------------------
  ----------------------------  allneighbors  --------------------------------
  ----------------------------------------------------------------------------
  Place the labels of all neighbors of v into the nlist array, starting at
  nlist[0].
*/
     
void	allneighbors( v, d, nlist )
     
        int	v,        /* vertex label */
                *d,       /* number of neighbors; degree of v */
                *nlist;   /* array of vertex labels returned */
{
        int    p;
     
        for ( *d = 0, p = *(A+v);  p != NIL;  p = *(next+p) )
             *(nlist + (*d)++) = *(adjnode+p);
}
     
/*-----------------------------------------------------------------------------
  --------------------------  firstadj and nextadj  ---------------------------
  -----------------------------------------------------------------------------
*/
     
int 	nextadj( v )
     
    	int	v;
{
    	int    w;
     
        if ( S[v] == NIL )  return(NIL);
        w = *(adjnode + S[v]);
        S[v] = *(next + S[v]);
        return(w);
}
     
int     firstadj( v )
     
        int    v;
{
        S[v] = A[v];
        return( nextadj(v) );
}
     
/*-----------------------------------------------------------------------------
  ---------------------------  swaplabels  ------------------------------------
  -----------------------------------------------------------------------------
*/
     
void	swaplabels( v1, v2 )
     
        int    v1, v2;  /* vertex labels */
{
        int    i, j, u;
     
        for ( i = A[v1];  i != NIL;  i = *(next+i) )
        {
             u = *(adjnode+i);
             for ( j = A[u];  *(adjnode+j) != v1;  j = *(next+j) );
                  *(adjnode+j) = v2;
        }
        for ( i = A[v2];  i != NIL;  i = *(next+i) )
        {
             u = *(adjnode+i);
             for ( j = A[u];  *(adjnode+j) != v2;  j = *(next+j) );
                  *(adjnode+j) = v1;
        }
        i = A[v1]; A[v1] = A[v2]; A[v2] = i;
        i = D[v1]; D[v1] = D[v2]; D[v2] = i;
        i = S[v1]; S[v1] = S[v2]; S[v2] = i;
        updateH;
}


/******************************************************************************
 ****************************  CLASS OPS  *************************************
 ******************************************************************************
*/

#    define  updateHC    if ( order[HC] == 0 )\
                 	     for ( --HC;  HC!=NIL && order[HC]==0;  --HC );
                /* Maintain HC to be the highest nonempty class label */
     
     
static int   color[ numV ],
             order[ maxC ],
             HC,
             chi;
     
     
/*----------------------------------------------------------------------------
  ------------------------  createclasses  -----------------------------------
  ----------------------------------------------------------------------------
  Initialize all vertices to be uncolored.  Save pointers to the needed graph
  operations.
*/
     
void  	createclasses()
{
        int  c, v;
     
        chi = 0; HC  = NIL;
        for ( v = 0;  v < numV;  ++v ) *(color + v) = NIL;
        for ( c = 0;  c < maxC;  ++c ) *(order + c) = 0;
}
     
/*----------------------------------------------------------------------------
  ---------------------------  orderof  --------------------------------------
  ----------------------------------------------------------------------------
  Return the number of vertices color c.
*/
     
int	orderof( c )
     
     	int  c;  /* class label */
{
     	return( order[ c ] );
}
     
/*----------------------------------------------------------------------------
  ----------------------------  colorof  -------------------------------------
  ----------------------------------------------------------------------------
  Return the the color of v.
*/
     
int  	colorof( v )
     
     	int  v;   /* vertex label */
{
     	return( color[ v ] );
}
     
/*----------------------------------------------------------------------------
  -----------------------------  chivalue  -----------------------------------
  ----------------------------------------------------------------------------
  Return the number of colors used.
*/
     
int   	chivalue()
{
      	return( chi );
}
     
/*----------------------------------------------------------------------------
  -----------------------------  highclass  ----------------------------------
  ----------------------------------------------------------------------------
  Return the highest color label used.
*/
     
int   	highclass()
{
      	return( HC );
}
     
/*----------------------------------------------------------------------------
  --------------------------  setcolor  --------------------------------------
  ----------------------------------------------------------------------------
  Assign vertex v color c.  Assumes that colorof(v) is NIL.
*/
     
void  	setcolor( v, c )
     
      	int   v;  /* vertex label */
      	int   c;  /* color class label */
{
      	*(color + v) = c;
      	if ( ++( *(order + c) ) == 1 )
      	{
             ++chi;
             if ( c > HC )  HC = c;
        }
}
     
/*----------------------------------------------------------------------------
  --------------------------  uncolor  ---------------------------------------
  ----------------------------------------------------------------------------
  Assumes that colorof(v) is not NIL;  colorof(v) becomes NIL.
*/
     
void  	uncolor( v )
     
      	int   v;     /* vertex label */
{
      	int   c;     /* class label */
     
      	c = *(color + v);
      	*(color + v) = NIL;
      	if ( --( *(order + c) ) == 0 )  --chi;
      	updateHC;
}
     
/*----------------------------------------------------------------------------
  --------------------------  makeempty  -------------------------------------
  ----------------------------------------------------------------------------
  Uncolor all vertices with color c.
*/
     
void  	makeempty( c )
     
      	int  c;  /* class label */
{
      	int  v;  /* vertex label */
     
      	for ( v = 0;  order[c] > 0;  ++v )
       	if ( color[ v ] == c )
       	{
             color[v] = NIL;
             --order[c];
        }
     
        --chi;
        updateHC;
}
     
/*----------------------------------------------------------------------------
  ------------------------------  swapclasses  -------------------------------
  ----------------------------------------------------------------------------
  Swap the labels of two colors.
*/
     
void  	swapclasses( c1, c2 )
     
      	int  c1, c2;  /* color labels */
{
      	int  v;     /* vertex label */
     
      	for ( v = 0;  v < numV;  ++v )
       	if ( color[v] = c1 )
             color[v] = c2;
        else if ( color[v] = c2 )
             color[v] = c1;
     
        swap( order[ c1 ], order[ c2 ] );
        updateHC;
}
     
/*----------------------------------------------------------------------------
  -------------------------  proper  -----------------------------------------
  ----------------------------------------------------------------------------
  Return true (1) if the coloration is proper, and false (0) otherwise.
*/
     
int    	proper()
{
       	int  v, u;  /* vertex labels */
     
       	for ( v = 0;  v < numV;  ++v )
              if ( color[v] != NIL )
                   for ( u = firstadj(v);  u != NIL;  u = nextadj(v) )
                        if ( color[u] == color[v] )
                             return(0);
        return( 1 );
}
     
/*------------------------------------------------------------------------------
  ---------------------------  allcolored  -------------------------------------
  ------------------------------------------------------------------------------
  Return true (1) if the vertices of the graph with labels 0 .. n-1 have been
  assigned a color and false (0) otherwise.
*/
     
int    	allcolored( n )
     
    	int n;
{
    	int v;
     
    	for ( v = 0;  v < n;  ++v )
             if ( color[v] == NIL )
                  return(0);
        return(1);
}
     
/*----------------------------------------------------------------------------
  ------------------  firstmember and nextmember  ----------------------------
  ----------------------------------------------------------------------------
  Traverse a class list; look at each vertex in a class.  More than one class
  can be traversed at a time, but no list can be involved in more than one
  traversal.
*/
     
static int   member[ maxC ];
     
         /*--------------------------------------------------*/
     
int	nextmember( c )
     
      	int  c;  /* class label */
{
      	for ( ++member[c]; 
	      member[c] < numV && color[member[c]] != c; 
	      ++member[c] );
        if ( member[c] >= numV )
             return(NIL);
        else
             return(member[c]);
}
     
         /*--------------------------------------------------*/
     
int   	firstmember( c )
     
      	int  c;  /* class label */
{
      	member[ c ] = NIL;
      	return( nextmember( c ) );
}
     
/*----------------------------------------------------------------------------
  -------------------------  allmembers  -------------------------------------
  ----------------------------------------------------------------------------
*/
     
void  	allmembers( c, mlist )
     
      	int  c, *mlist;
{
      	int  i, v;
     
      	for ( i = 0, v = 0;  v != numV;  ++v)
       	     if ( color[v] == c )
                  *(mlist + i++) = v;
}
     
/*----------------------------------------------------------------------------
  -------------------------  allconflicts  -----------------------------------
  ----------------------------------------------------------------------------
  Return the number and vertex labels of all vertices colored c that are
  adjacent to v.
*/
     
void   	allconflicts( v, c, numc, clist )
     
       	int  v,       /* vertex label */
             c,       /* class label  */
             *numc,   /* retun the number of conflicts */
             *clist;  /* return a list of conflicts */
{
        int  w;
     
        for ( *numc = 0, w = firstadj(v);  w != NIL;  w = nextadj(v) )
             if ( color[w] == c )
                  *(clist + (*numc)++) = w;
}
     
/*----------------------------------------------------------------------------
  -------------------------  inconflict  -------------------------------------
  ----------------------------------------------------------------------------
  Return true ( 1 ) if v is adjacent to any vertex in class c; assumes that v
  is not a member of c.  Return false ( 0 ) otherwise.
*/
     
int  	inconflict( v, c )
     
     	int  v;    /* vertex label */
     	int  c;    /* class label  */
{
     	int  u;
     
     	for ( u = firstadj(v);  u != NIL;  u = nextadj(v) )
             if ( color[u] == c )
                  return(1);
        return( 0 );
}
