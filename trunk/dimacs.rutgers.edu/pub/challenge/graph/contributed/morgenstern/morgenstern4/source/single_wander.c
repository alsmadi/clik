/* FILE       : single_wander.c
   WRITTEN BY : Craig Morgenstern
*/
     
#    include    "graph_globals.h"
     
typedef	int 	(*PFI)();
typedef void	(*PFV)();;
     
static	PFI    firstadj, nextadj, colorof, numAdjColors;
static  PFV    setcolor, uncolor;
static  int    chi, T, *swaps;
static	char   crossed[ numV ];
     
static  int    recursivewander( v, w )
     
        int    v, w;
{
        int    c, d, u, possible[maxC];
     
        for ( c = 0;  c < chi;  ++c )  possible[c] = NIL;
        for ( u = (*firstadj)(v);  u != NIL;  u = (*nextadj)(v) )
             if ( u != w )
             {
                  c = (*colorof)(u);
                  if ( c != NIL  &&  (*numAdjColors)(v,c) == 1 )
                   possible[c] = u;
             }
     
        /* Single level BFS wander (v's 1st neighborhood) */
        for ( c = 0;  c < chi;  ++c )
        {
            u = possible[c];
            if ( u != NIL )
                 for ( d = 0;  d < chi;  ++d )
                      if ( d != c  &&  (*numAdjColors)(u,d) == 0 )
                      {
                           /* impasse resolved */
                           ++*swaps;
                           (*uncolor)(u);
                           (*setcolor)(u,d);
                           (*setcolor)( v, c );
                           return(1);
                      }
        }
     
        /* Impasse not resolvable in v's 1st neighborhood -- DFS wander */
        for ( c = 0;  c < chi;  ++c )
        {
            u = possible[c];
            if ( u != NIL  &&  crossed[u] < T )
            {
                 ++*swaps;
                 ++crossed[u];
                 (*uncolor)(u);
                 (*setcolor)(v,c);
                 if ( ! recursivewander(u,v) )
                 {
                      --crossed[u];
                      (*uncolor)(v);
                      (*setcolor)(u,c);
                 }
                 else
                 {
                      --crossed[u];
                      return(1);
                 }
            }
        }
        return(0);
}
     
     
int	singleWander( w, chibnd, maxT, numswaps,
                      fadj, nadj, scolor, ucolor, color, nAC )
     
        int    w, chibnd, maxT, *numswaps;
        PFI    fadj, nadj, color, nAC;
        PFV    scolor, ucolor;
{
        int    success;
         
        chi = chibnd;  T = maxT;  swaps = numswaps;
        firstadj = fadj;  nextadj = nadj;
        setcolor = scolor;  uncolor = ucolor;  colorof = color;
        numAdjColors = nAC;
     
        *swaps = 0;
        crossed[w] = 1;
        success = recursivewander( w, w );
        crossed[w] = 0;
        return(success);
}
