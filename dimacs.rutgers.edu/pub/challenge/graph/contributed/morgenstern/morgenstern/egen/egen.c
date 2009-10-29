/* FILE : egen.c 
 * written by craig morgenstern
 *
 * Generate a maximally planar graph by the expansion method.  The basic
 * method proceeds as follows :
 *   
 *    (1) Construct an initial circuit, C, with numV nodes (labeled 0..numV-1).
 *    (2) Triangulate the "inside" of C using routine Triangulate1.
 *    (3) Triangulate the "outside" of C using routine Triangulate2.
 *
 * The resulting graph is maximally planar with no vertices of degree < 5,
 * and the degree distribution has a longer tail than that of the reduction
 * generation method.  The graph produced has 3 times the number of vertices 
 * as the initial circuit size.
 */

# include <stdio.h>
# include <sys/time.h>
# include <sys/resource.h>
# include "graph.h"
# include "triangulate.h"
void exit();

# define Vfactor 3.5 /* Vfactor * initial_circuit_sz = estimate of graph size */

static	void printrusage()
{
	struct rusage rstats;
	getrusage(RUSAGE_SELF, &rstats);
	printf("c      current running time = %d secs\n",
			rstats.ru_utime.tv_sec);
	printf("c      maximum resident set size = %d bytes\n",
			rstats.ru_maxrss * getpagesize());
	printf("c      number of major page faults = %d\n",
			rstats.ru_majflt);
}

void	main()
{
	int N, numV;
	int d, i, v, degdist[100], numedges;
	unsigned seed;
	char mapfile[256];
	FILE *mfp;

	scanf("%d %d %s", &N, &seed, mapfile);
	if ((mfp = fopen(mapfile, "w")) == NULL)
	{
	    printf("*** cannot open %s to write\n", mapfile);
	    exit(1);
	}
  	printf("c MPG GENERATION (expansion method)\n");
	printf("c initial circuit size     : %d\n", N);
	printf("c random number stream seed: %d\n", seed);
	printf("c Graph output file        : %s\n", mapfile);
	printf("c \n");

	creategraph((int)(N * Vfactor), mfp);
        for (v = 0;  v < N-1;  ++v)  makeadjacent(v, v+1);
        makeadjacent(0, N-1);
	printf("c resource usage after graph initialization...\n");
	printrusage();
	printf("c \n");
     
        Triangulate1(N, &seed);
	printf("c resource usage after Triangulate1...\n");
	printrusage();
	printf("c \n");

        Triangulate2(N, &seed);
	printf("c resource usage after Triangulate2...\n");
	printrusage();
	printf("c \n");


        /* PRINT GENERATION DATA */
	numV = pristineV;
	printf("c Generation complete...\n");
	printf("c number of vertices: %d\n", numV);
	for (d = 0;  d < 100;  ++d)  degdist[d] = 0;
	numedges = 0;
  	for (i = 0;  i < numV;  ++i)
    	{
      	    d = degreeof(i);
      	    numedges += d;
      	    ++degdist[d];
    	}
    	printf("c number of edges: %d\n", numedges / 2);
	printf("c degree distribution:     degree   num of vertices\n");
  	printf("c                          ------   ---------------\n");
  	for (d = 0;  d < 100;  ++d)
     	    if (degdist[d] != 0) printf("c %29d  %15d\n", d, degdist[d]);
  	printf("p col %d %d\n", numV, numedges / 2);
}
