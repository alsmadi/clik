/* FILE: color_ops.c		WRITTEN BY : Craig Morgenstern */

# include <stdio.h>
# include <malloc.h>
# include "static_graph.h"
# include "color_ops.h"

unsigned char *color = NULL;
int *CCcnt = NULL,  numcc = 0;

/* -----------------------  create_colors  -----------------------------------
 * Initialize all vertices to be uncolored.  Assumes the graph has already 
 * been initialized.
 */

int	create_colors(k)

	int k;
{
	int v, c;
     
	if (color != NULL) free((char *) color);
        color = (unsigned char *) malloc((unsigned) numvertices);
	if (color == NULL) return(0);

	if (CCcnt != NULL) free((char *) CCcnt);
        CCcnt = (int *) malloc((unsigned)(k) * sizeof(int));
	if (CCcnt == NULL) return(0);

	numcc = k;
        for (v = 0;  v < numvertices;  ++v) colorof(v) = nocolor;
	for (c = 0;  c < numcc;  ++c) ccsz(c) = 0;
	return(1);
}

/* -----------------------  chivalue  ----------------------------------------
 */

int	chivalue()
{
	int chi, v, c;

	for (chi = 0, v = 0;  v < numvertices;  ++v)
	{
	     c = colorof(v);
	     if (c != nocolor  &&  c > chi)  chi = c;
	}
	return(chi+1);
}

/* -------------------------  proper  ---------------------------------------- 
 * Return 1 if the (partial) coloration is proper; 0 otherwise.
 */

int    	proper()
{
       	int  c, v, *p, *q;

       	for (v = 0;  v < numvertices;  ++v)
	{
	     c = colorof(v);
             if (c != nocolor)
	     {
		 for (p = adjstart(v), q = adjstop(v);  p != q;  ++p)
                      if (colorof(*p) == c) return(0);
	     }
	}
        return(1);
}

/* -------------------------  allcolored  -------------------------------------
 * Return 1 if all vertices have been assigned a color; 0 otherwise.
 */

int    	allcolored()
{
    	int v;

    	for ( v = 0;  v < numvertices;  ++v )
             if (colorof(v) == nocolor) return(0);
        return(1);
}

/* ------------------------  inconflict  -------------------------------------
 * Return 1 if v is adjacent to any vertex in colored c; 0 otherwise.
 */

int  	inconflict( v, c )

     	int  v, c;
{
     	int  *p, *q;

	for (p = adjstart(v), q = adjstop(v);  p != q;  ++p)
             if (colorof(*p) == c ) 
	         return(1);
        return(0);
}
