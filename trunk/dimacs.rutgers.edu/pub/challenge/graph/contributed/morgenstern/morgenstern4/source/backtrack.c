
/* FILE       : backtrack.c
   WRITTEN BY : Craig Morgenstern
*/
     
#    include    "graph_globals.h"
     
typedef	int	(*PFI)();	/* Pointer to Function returning Integer */
typedef void    (*PFV)();       /* Pointer to Function returning Void */
     
#    	define  untouched	numV
     
#       define  createQ         Qsz = 0;  Qhead = 0;  Qtail = 0
#       define  emptyQ         	( Qsz == 0 )
#       define  deQ        	( (--Qsz >= 0) ? Q[ ++Qhead % numV ] : NIL )
#       define  enQ( v )        if (++Qsz <= numV)  Q[ ++Qtail % numV ] = v
     
        /* all static ints are automatically initialized to 0's */
static  int     Q[ numV ], Qsz, Qhead, Qtail;

static	int	oldcolor[numV];
     
static  char    CR[ numV ][ maxC ],  /* Color Rotation */
                RC[ numV ];          /* Rotation Color (curser into CR) */

static  int     CP[ numV ],          /* Current Predecessors */
        	AP[ maxC ],          /* Adjacent Predecessors */
        	SV[ numV ],          /* State Vector */
        	VL[ numV ];          /* Vertex Level (in state vector) */
     
     
/*-----------------------------------------------------------------------------
  -------------------------  initializeBT  ------------------------------------
  -----------------------------------------------------------------------------
  INVARIENT FOR PACKAGE : Upon entry to any backtracking routine, all entries
  of the VL array are set to  untouched.  All entries if the CP array are set
  to false (0) except CP[0], which remains true (1) to act as a sentinal.
  Since each backtracking routine resets VL and CP so that this invarient
  is maintained, this routine need only be called once prior to the first call 
  to any of the following backtracking routines.
*/
     
void	initializeBT( n )

	int    n;
{
    	int    v;
     
    	for ( v = 0;  v < numV;  ++v )
        {
             VL[v] = untouched;
             CP[v] = 0;
        }
        CP[0] = 1;
}
     
/*-----------------------------------------------------------------------------
  ---------------------------  localized_deepBT  ------------------------------
  -----------------------------------------------------------------------------
*/
     
int    	localized_deepBT( n, mindepth, depth, maxBT, numBT, chi, color,
              		  impasse_vertex, inconflict, firstadj, nextadj, 
			  setcolor, uncolor )
     
    	int    	n,     	     /* there are  n  colored vertices */
        	impasse_vertex, /* impasse vertex label */
        	mindepth,    /* min depth of search; 0 < mindepth < n */
        	*depth,      /* return the depth of the search */
        	maxBT,       /* maximum allowed number of backtrack steps */
        	*numBT,      /* return the number of backtrack steps taken */
        	chi;         /* Only color labels 0 .. chi-1 are allowed */
     
    	PFI    	color,       /* (*color)(v) gives the color assigned to v */
        	inconflict;  /* (*inconflict)(v,c) returns 1 when a neighbor
                		of v is colored c, and 0 if no neighbors of
                		v are colored c  */
     
    	PFI    	firstadj, nextadj;  /* traverse adjacency lists */
     
    	PFV    	setcolor,    /* (*setcolor)(v,c) assigns color c to v */
        	uncolor;     /* (*uncolor)(v) gives v the NIL color */
{
    	int     floor,       /* lowest level touched */
            	d,           /* color label */
    		r,           /* state vector curser */
            	c,           /* color label */
            	CPcurser,    /* curser into current pred array */
            	v, w;        /* vertex labels */
     
        v = impasse_vertex;
        for ( c = 0;  c < chi;  ++c )  CR[v][c] = c;
        RC[v] = chi;
         
        createQ;
        enQ(v);
        r = n;
        SV[r] = v;
        VL[v] = r;
        while ( ! emptyQ )
        {
             v = deQ;
             for ( w = (*firstadj)(v);  w != NIL;  w = (*nextadj)(v) )
                  if ( VL[w] == untouched  &&  (*color)(w) != NIL )
                  {
                       --r;
		       d = (*color)(w);
		       oldcolor[r] = d;
                       SV[r] = w;
                       VL[w] = r;
                       RC[w] = 0;
                       for ( c = 0; c < chi;  ++c  )
                       {
                            CR[w][c] = d;
                            d = ++d % chi;
                       }
                       if ( mindepth <= r )  enQ(w);
                  }
        }
     
        floor = r;
        if ( r > mindepth ) mindepth = r;
        CPcurser = 0;
        *numBT = 0;
        *depth = n;
        r = n;
        v = SV[r];
         
        for (;;)
        {
             for (++RC[v]; RC[v]<chi && (*inconflict)(v,(int)CR[v][RC[v]]); ++RC[v]);
             if ( RC[v] < chi )
             {
                  /* ADVANCE */
                  (*setcolor)(v,(int)CR[v][RC[v]]);
                  if ( ++r > n )
                  {
                       /* IMPASSE RESOLVED */
                       for ( r = floor;  r <= n;  ++r )
                       {
                            CP[r] = 0;
                            VL[ SV[r] ] = untouched;
                       }
                       return(1);
              }
              else
              {
                   v = SV[r];
                   RC[v] = NIL;
              }
         }
         else
         {
              /* BACKTRACK */
              for ( c = 0;  c < chi;  ++c ) AP[c] = untouched;
              for ( w = (*firstadj)(v); w != NIL; w = (*nextadj)(v) )
                   if ( VL[w] < r )
                   {
                        c = (*color)(w);
                        if ( AP[c] > VL[w] )  AP[c] = VL[w];
                   }
              for ( c = 0;  c < chi;  ++c )
                   if ( AP[c] != untouched )
                   {
                        CP[ AP[c] ] = 1;
                        if ( CPcurser < AP[c] ) CPcurser = AP[c];
                   }
              while ( !CP[CPcurser]  &&  CPcurser >= mindepth )  --CPcurser;
              while ( --r > CPcurser ) (*uncolor)(SV[r]);
     
              if ( r <= mindepth  ||  ++(*numBT) > maxBT )
              {
                   /* FAILED TO RESOLVE IMPASSE */
                   for ( r = floor;  r <= n;  ++r )
                   {
                        CP[r] = 0;
                        VL[ SV[r] ] = untouched;
                   }
                   for ( r = floor;  r < n;  ++r )
		   {
			if ( (*color)( SV[r] ) != NIL )
			     (*uncolor)(SV[r]);
			(*setcolor)( SV[r], oldcolor[r] );
		   }
                   return(0);
              }
              else
              {
                   if ( r < *depth ) *depth = r;
                   CP[r] = 0;
                   v = SV[r];
                   (*uncolor)(v);
              }
         }
    }
}
