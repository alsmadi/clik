/*  FILE: graph.h		Written by Craig Morgenstern
 */

extern int maxV; 
	/* largest valid vertex label */
	/* Only 0..maxV-1 are valid vertex labels */

extern int pristineV; 
	/* unused vertex label (never been part of an edge) */

extern int *Vdeg;
 
# define degreeof(v) (*(Vdeg + (v)))
void creategraph(/* int numV;  FILE *mapfile */);
void makeadjacent(/* int v1, v2 */);
 
