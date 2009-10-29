/*  FILE: graph.c		Written by Craig Morgenstern */

# include <stdio.h>
# include <malloc.h>
# include "graph.h"
void exit();

int pristineV = 0; 
	/* unused vertex label */

int maxV = 0;
	/* max number of vertices in the graph */

int *Vdeg = NULL;
	/* Vertex degrees */

# define DEG(v) (*(Vdeg + (v)))

static FILE *mfp;

void	creategraph(n, mapfileptr)

	int n;
	FILE *mapfileptr; /* Already opened for writing */
{
	int v;

	if (Vdeg != NULL) free((char *) Vdeg);
	Vdeg = (int *) malloc((unsigned)(n) * sizeof(int));
	if (Vdeg == NULL)
	{
	    printf("*** malloc failed in creategraph(%d)\n", n);
	    exit(1);
	}
        for (v = 0;  v < n;  ++v) DEG(v) = 0;
	maxV = n;
	pristineV = 0;
	mfp = mapfileptr;
}


void	makeadjacent(v1, v2)

	int v1, v2;
{
	fprintf(mfp, "e %d %d\n", v1+1, v2+1);
	++DEG(v1);
	++DEG(v2);
	if (v1 >= pristineV) pristineV = v1+1;
	if (v2 >= pristineV) pristineV = v2+1;
}
