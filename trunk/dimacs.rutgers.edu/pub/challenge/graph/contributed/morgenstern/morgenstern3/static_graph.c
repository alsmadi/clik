/* FILE: static_graph.c		WRITTEN BY : Craig Morgenstern */

# include <stdio.h>
# include <malloc.h>
# include "static_graph.h"

int     *Alist = NULL,	/* adjacency lists */
	*Ahead = NULL,	/* adjacency list heads */
	numvertices = 0,
	numedges = 0,
	maxdeg = 0;

/* --------------------------  ReadGraph  -----------------------------------
 * Graphs in an external file are assumed to be in DIMACS format.
 */

# define maxln 256

int	read_static_graph(graph_file)
     
	char *graph_file; /* name of the file containing the graph */
{
	FILE *fp;
        int v, w, e; /* vertex labels */
	int *deg; /* array of vertex degrees */
	char col[16], line[maxln];
     
	fp = fopen(graph_file, "r");
	if (fp == NULL) return(0);
	fgets(line, maxln, fp);
	while (line[0] != 'p')
	{
	    fputs(line, stdout);
	    fgets(line, maxln, fp);
	}
	sscanf(line+1, "%s %d %d", col, &numvertices, &numedges);
	maxdeg = 0;

	if (Ahead != NULL) free((char *) Ahead);
	Ahead = (int *) malloc( (unsigned)(numvertices + 1) * sizeof(int) );
	if (Ahead == NULL) return(0);

	deg = (int *) malloc( (unsigned)(numvertices) * sizeof(int) );
	if (deg == NULL) return(0);

	/* pass1: get vertex degrees */
        for (v = 0;  v < numvertices;  ++v) *(deg + v) = 0;
        for (e = 0;  e < numedges;  ++e)
	{
	    fgets(line, maxln, fp);
	    while (line[0] != 'e') fgets(line, maxln, fp);
	    sscanf(line+1, "%d %d", &v, &w);
	    ++(*(deg+v-1));
	    ++(*(deg+w-1));
	}
        for (v = 0;  v < numvertices;  ++v)
	     if (*(deg + v) > maxdeg) maxdeg = *(deg + v);
	fclose(fp);
	fp = fopen(graph_file, "r");

	/* pass2: read in edges */
	if (Alist != NULL) free((char *) Alist);
	Alist = (int *) malloc( (unsigned)(2*numedges) * sizeof(int) );
	if (Alist == NULL) return(0);

        for (*Ahead = *deg, v = 1;  v < numvertices;  ++v)
	    *(Ahead + v) = *(Ahead + v-1) + *(deg + v);
	*(Ahead + numvertices) = *(Ahead + numvertices-1);
        for (e = 0;  e < numedges;  ++e)
	{
	    fgets(line, maxln, fp);
	    while (line[0] != 'e') fgets(line, maxln, fp);
	    sscanf(line+1, "%d %d", &v, &w);
	    --v;
	    --w;
	    --(*(Ahead + v));
	    --(*(Ahead + w));
	    *(Alist + *(Ahead + v)) = w;
	    *(Alist + *(Ahead + w)) = v;
	}
	fclose(fp);
	free((char *) deg);
	return(1);
}

/* ---------------------------  adjacent  ------------------------------------
 */

int	adjacent(v1, v2)

        int v1, v2; /* vertex labels */
{
    	int d1, d2, i, *p;

	d1 = degreeof(v1);
	d2 = degreeof(v2);
	if (d1 > d2)
	{
           for (p = adjstart(v1), i = 0;   *(p++) != v2 && i++ < d1;);
           return(i < d1);
	}
	else
	{
           for (p = adjstart(v2), i = 0;   *(p++) != v1 && i++ < d2;);
           return(i < d2);
	}
}
     
/* ---------------------------  allneighbors  --------------------------------
 */
     
void	allneighbors(v, nlist)

	int v, /* vertex label */
            *nlist; /* array of vertex labels returned */
{
        int i, d, *p;

        for (d = degreeof(v), i = 0, p = adjstart(v);  
	     i < d;
	     *(nlist++) = *(p++));
}
