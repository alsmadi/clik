/* FILE       :  static_ops.c
   WRITTEN BY : Craig Morgenstern
     
   ASSUMES that the graph already exists and that the ClassOps package
       has been initialized.  Any change to the graph after calling
       create_static will not be known in this package and can result
       in incorrect results.
     
   ASSUMES that all vertices are contiguously labeled 0 ... n-1.
*/
     
#	include "graph_globals.h"
#    	include "graph_ops.h"
#    	include	"class_ops.h"
     
static  char	ACcnt[numV][maxC];  /* Adjacent Color count */
     
/*-----------------------------------------------------------------------------
  -------------------------  create_static  -----------------------------------
  -----------------------------------------------------------------------------
*/
     
void	create_static(n)
     
        int	n;  /* number of vertices */
{
        int	v,  /* vertex label */
                c;  /* color label */
     
        for ( v = 0;  v < n;  ++v )
             for ( c = 0;  c < maxC;  ++c )
                  ACcnt[v][c] = 0;
}
     
/*-----------------------------------------------------------------------------
  ------------------------  static_setcolor  ----------------------------------
  -----------------------------------------------------------------------------
*/
     
void    static_setcolor( v, c )
     
        int     v,  /* vertex label */
                c;  /* color label */
{
        int     w;  /* vertex label */
     
        for ( w = firstadj(v);  w != NIL;  w = nextadj(v) ) ++ACcnt[w][c];
        setcolor(v,c);
}
     
/*-----------------------------------------------------------------------------
  ------------------------  static_uncolor  -----------------------------------
  -----------------------------------------------------------------------------
*/
     
void	static_uncolor(v)
     
    	int	v;  /* vertex label */
{
    	int    	w,  /* vertex label */
        	c;  /* color label */
     
    	c = colorof(v);
    	for ( w = firstadj(v);  w != NIL;  w = nextadj(v) ) --ACcnt[w][c];
    	uncolor(v);
}
     
/*-----------------------------------------------------------------------------
  --------------------  static_inconflict  ------------------------------------
  -----------------------------------------------------------------------------
*/
     
int    	static_inconflict( v, c )
     
    	int    	v,  /* vertex label */
        	c;  /* color label */
{
    	return( ACcnt[v][c] > 0 );
}
     
/*-----------------------------------------------------------------------------
  ---------------------------  static_numAdjColors  ---------------------------
  -----------------------------------------------------------------------------
*/
     
int    	static_numAdjColors( v, c )
     
    	int    	v,  /* vertex label */
        	c;  /* color label */
{
    	return( ACcnt[v][c] );
}
