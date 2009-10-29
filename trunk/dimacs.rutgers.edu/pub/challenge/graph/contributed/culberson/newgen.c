/*
	Title:	Flat Graph Generator (Improved version - June 1994)
	Does:	Generates quasi-random k-colorable graphs that may be used
		to test coloring algorithms. These may or may not be harder
		than equi-partite graphs, but for values near the ridge
		they do give iterated greedy a rougher time.
	Author: Joseph Culberson
	email:	joe@cs.ualberta.ca
	
	Compilation: C Code

	Inputs: <from stdin - prompted>
		 Number of vertices (integer)
		 Edge Probability [0..1000] (integer)
				- probability in 10ths of a percent
		 Number of Partitions (integer)
		 Flatness (integer, 0 = max difficulty (??) )
		 Random Seed (integer)
	Outputs: creates a graph in DIMACS ASCII format
		 in a file flat###_##_#.col
		 where ### = number of vertices, 
			## = Number of partitions
			 # = flatness

	Copyright: Use and distribute freely.
	Warranty: NONE!
	Special Instructions: Have fun!
*/
		
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <malloc.h>

extern time_t time();
extern void bzero();

/* probabilities will be in tenths of a per cent */
#define MAX_DENSITY 1000.0

/* Definitions useful for checking and setting edges */

/*                     ***NOTE***
    set and clear are asymmetric - use setedge(i,j) setedge(j,i) etc.
*/
#define SHIFT 3
#define MASK 7
#define ROWSIZE (1+(size >> SHIFT))
#define GRAPHSIZE ROWSIZE * size

#define setedge(i,j)  graph[((i)*ROWSIZE) + ((j) >> SHIFT)] |= (1 << ((j) & MASK))
#define clearedge(i,j) graph[((i)*ROWSIZE) + ((j) >> SHIFT)] &= ~(1 << ((j) & MA SK))

#define isedge(i,j)    (graph[((i)*ROWSIZE)+((j) >> SHIFT)] & (1 << ((j) & MASK)))

/* for loops involving potential neighbors */
#define initnbr(x,i)  (x) = graph + ((i)*ROWSIZE)
#define isnbr(x,i)  (((x)[(i) >> SHIFT]) & (1 << ((i) & MASK)))

extern long random();
extern void srandom();

void create_and_shuffle(vset,size)
int *vset,size;
{
	int i,k,t;

	
	for(i=0;i<size;i++) {
		vset[i] = i;
	}
	/* shuffle */
	for(i=0;i<size-1;i++) {
		k = i+ (random() % (size - i));
		t = vset[k];
		vset[k] = vset[i];
		vset[i] = t;
	}
}

void assign1(graph,vset,size,nedges,b1,b2,large,part,flat)
char *graph;
int *vset,size,nedges,b1,b2,large,part;
int flat;
{

	int nv, npairs,f1,f2,l1,l2;
	int i,j,v,w,vp;
	int *psetx, *psety;

	int deg1,deg2;
	int *dg1, *dg2;

	nv = (int) (size/part);

	/* compute first and last vertices of block b1 */
	if (b1 < large) 
		f1 = (nv+1) * b1;
	else 
		f1 = (nv+1)*large + (b1-large)*nv;

	if ((b1+1) < large)
		l1 = (nv+1)*(b1+1);
	else
		l1 = (nv+1)*large +((b1+1-large)*nv);

	/* compute first and last vertices of block b2 */
	if (b2 < large) 
		f2 = (nv+1) * b2;
	else 
		f2 = (nv+1)*large + (b2-large)*nv;

	if ((b2+1) < large)
		l2 = (nv+1)*(b2+1);
	else
		l2 = (nv+1)*large +((b2+1 -large)*nv);

	npairs = (l1-f1)*(l2-f2);
	psetx = (int *) calloc(npairs, sizeof(int));
	psety = (int *) calloc(npairs, sizeof(int));
	dg1 = (int *) calloc(l1,sizeof(int));
	dg2 = (int *) calloc(l2,sizeof(int));

	deg1 = nedges/(l1-f1);
	if ( (deg1*(l1-f1)) < nedges ) deg1++;
	deg1 += flat;

	deg2 = nedges/(l2-f2);
	if ( (deg2*(l2-f2)) < nedges ) deg2++;
	deg2 += flat;

	for(i=f1;i<l1;i++) dg1[i] =0;
	for(i=f2;i<l2;i++) dg2[i] =0;

	for(i=f1;i<l1;i++) for(j=f2;j<l2;j++) {
		psetx[(i-f1)*(l2-f2)+(j-f2)] = i;
		psety[(i-f1)*(l2-f2)+(j-f2)] = j;
	}

	for(i=0;i<nedges; ){
		vp = random() % npairs;
		v = psetx[vp];
		w = psety[vp];
		if ( (dg1[v] <= deg1) && (dg2[w] <= deg2) ) {
			setedge(vset[v],vset[w]);
			setedge(vset[w],vset[v]);
			i++;
			dg1[v]++;
			dg2[w]++;
		}
		/* reduce the set of pairs */
		npairs--;
		if (npairs <= 0) {
			if ( i < nedges) {
				printf("ERROR: Cannot allocate edges\n");
				exit(10);
			}
		} else {
			psetx[vp] = psetx[npairs];
			psety[vp] = psety[npairs];
		}
	}

	free(psetx);
	free(psety);
	free(dg1);
	free(dg2);
	
}
void create_graph(graph,vset,size,prob,part,flat,edges)
char * graph;
int *vset, size,prob,part,flat,*edges;
{
	int large,nvert,npairs,nedges,residue;
	int i,j;

	large = size % part; /* for 0<= i< large, each partition element 
				has ceil(size/part) vertices,
				and large <=i<part has floor(size/part) */

	/*************************************/
	/* assign edges between large blocks */
	/*************************************/

   if (large > 1) {
	/* number of vertices in large block */
	nvert = 1+ (int) (size/part);

	/* number of pairs of blocks */
	npairs = (large*(large-1))/2;

	/* Expected number of edges assigned between large pairs */
	nedges = (int) ( npairs *nvert *nvert *(((double) prob)/MAX_DENSITY));

	/* to distribute, need to know how many take ceiling */
	residue = nedges % npairs;
	nedges = nedges / npairs;

	for(i=0;i<large;i++) for(j=i+1;j<large;j++) {
		if ( (random() % npairs) < residue){
		  assign1(graph,vset,size,nedges+1,i,j,large,part,flat);
		  npairs--;
		  residue--;
		  *edges += nedges+1;
		} else {
		  assign1(graph,vset,size,nedges,i,j,large,part,flat);
		  npairs--;
		  *edges += nedges;
		}
	}
   }
   if (large > 0) {
		
	/*************************************/
	/*assign edges large and small blocks */
	/*************************************/

	/* number of vertices in large block */
	nvert = (int) (size/part);

	/* number of pairs of blocks */
	npairs = large*(part-large);

	/* Expected number of edges assigned between all pairs */
	nedges = (int) ( npairs *nvert *(nvert+1)
			*(((double) prob)/MAX_DENSITY));

	/* to distribute, need to know how many take ceiling */
	residue = nedges % npairs;

	/* and how many per pair (floor) */
	nedges = nedges / npairs;

	for(i=0;i<large;i++) for(j=large;j<part;j++) {
		if ( (random() % npairs) < residue){
		  assign1(graph,vset,size,nedges+1,i,j,large,part,flat);
		  npairs--;
		  residue--;
		  *edges += nedges+1;
		} else {
		  assign1(graph,vset,size,nedges,i,j,large,part,flat);
		  npairs--;
		  *edges += nedges;
		}
	}
   }

   if ((part > 1) && (large<(part-1))) {
	/*************************************/
	/* assign edges between small blocks */
	/*************************************/

	/* number of vertices in large block */
	nvert = (int) (size/part);

	/* number of pairs of blocks */
	npairs = ((part-large)*(part-1-large))/2;

	/* Expected number of edges assigned between all pairs */
	nedges = (int) ( npairs *nvert *nvert
			*(((double) prob)/MAX_DENSITY));

	/* to distribute, need to know how many take ceiling */
	residue = nedges % npairs;

	/* and how many per pair (floor) */
	nedges = nedges / npairs;

	for(i=large;i<part-1;i++) for(j=i+1;j<part;j++) {
		if ( (random() % npairs) < residue){
		  assign1(graph,vset,size,nedges+1,i,j,large,part,flat);
		  npairs--;
		  residue--;
		  *edges += nedges+1;
		} else {
		  assign1(graph,vset,size,nedges,i,j,large,part,flat);
		  npairs--;
		  *edges += nedges;
		}
	}
  }
}

void print_order(vset,size,fp)
int *vset,size;
FILE *fp;
{
	
	int i,fmt;

	fprintf(fp,"c\nc For verification purposes apply greedy to the\n");
	fprintf(fp,"c following permutation:\n");

	fmt = 0;
	fprintf(fp,"c X: ");
	for(i=0;i<size;i++) {
		if (fmt>19) {
			fmt = 0;
			fprintf(fp,"\nc X: ");
		}
		fprintf(fp,"%d ",vset[i]+1);
		fmt++;
	}
	fprintf(fp,"\nc\n");
}
	
void print_graph(graph,size,edges,fp)
char *graph;
int edges;
int size;
FILE *fp;
{
	int i,j;

	fprintf(fp,"p edge %d %d\n",size,edges);

	for(i=0;i<size-1;i++)
		for(j=i+1;j<size;j++) 
			if (isedge(i,j) ) 
				fprintf(fp,"e %d %d\n",j+1,i+1);
}

void verify_color(graph,size,part,vset,fp)
char *graph;
int size,part,*vset;
FILE *fp;
{

	char *x;
	int *start,*vertex,*next;

	int i,k,clr,v,maxclr,freeid;

	start = (int *) calloc(size+1,sizeof(int));
	vertex = (int *) calloc(size+1,sizeof(int));
	next = (int *) calloc(size+1,sizeof(int));

	/* All color sets are empty initially */
	for(i=1; i<=size;i++) start[i]=0;
	freeid = 1; /* next free position */

	maxclr = 0; 

	/* greedy color in the order of vset */
	for(i=0;i<size;i++) {
		clr = 1;
		k = start[clr];
		v = vset[i]; /* choosing in permuted order */
		initnbr(x,v);
		while (k) {
                        /* no conflicts with clr but vertex at k untested */
                        if (isnbr(x, vertex[k] )) {
                                /* conflict with clr */
                                clr++; /* next clr */
                                k = start[clr];
                        }
                        else k = next[k];  /* next vertex position to test */
                }
                /* k=0 means clr had no conflicts so use it */
                if (clr >maxclr) maxclr = clr;

                /* add vertex to appropriate color list */
                vertex[freeid] = v;
                next[freeid] = start[clr];
                start[clr] = freeid;
                freeid++;
      }

	if (maxclr < part) {
		fprintf(fp,"c WARNING: Coloring better than partition\n");
		printf("WARNING: Coloring better than partition\n");
		fprintf(fp,"c This means that due to the choice of probability\n");
		fprintf(fp,"c and partition there exists better colorings than\n");
		fprintf(fp,"c the one specified\n");
		fprintf(fp,"c Color = %d Partition = %d\n",maxclr,part);
	} else if (maxclr == part) {
		fprintf(fp,"c COLOR VERIFICATION: Using the permuted order\n");
		fprintf(fp,"c under simple greedy yields the specified\n");
		fprintf(fp,"c coloring number\n");
		fprintf(fp,"c Color = %d specified partitions = %d\n",
			maxclr,part);
	} else {
		printf("ERROR: Coloring greater than partition number\n");
		printf("This indicates a program error and should not occur\n");
		printf("Color = %d Partition = %d\n",maxclr,part);
		fprintf(fp,"ERROR: Coloring greater than partition number\n");
		fprintf(fp,"c This indicates a program error and should not occur\n");
		fprintf(fp,"c Color = %d Partition = %d\n",maxclr,part);
	}
	free(start);
	free(vertex);
	free(next);
}

void main()
{
	FILE *fp;
	time_t clock;

	int size,prob,part,flat,edges;
	char name[100];

	long seed;

	int *vset;

	char *graph;

	int i,j,deg,maxd,mind;
	
	/* INPUT PARAMETERS */

	printf("Enter\n");
	printf("\tnumber of vertices ");
	scanf("%d",&size);
	printf("\tdensity(10ths of percent) ");
	scanf("%d",&prob);
	printf("\tnumber of partitions ");
	scanf("%d",&part);
	if (part<1) {
		printf("Must have at least one color!!\n");
		exit(1);
	}
	printf("\tflatness ");
	scanf("%d",&flat);
	if (flat <0) {
		printf("Flatness must be greater than or equal to 0\n");
		exit(1);
	}

	printf("\trandom seed ");
	scanf("%d",&seed);
	srandom(seed);
	
	/* OPEN FILE AND PRINT PREAMBLE */

	sprintf(name,"flat%d_%d_%d.col",size,part,flat);
        if ( (fp=fopen(name,"w"))==NULL )
          { printf("ERROR: Cannot open outfile\n"); exit(10); }
	fprintf(fp,"c File: flat%d_%d_%d.col\nc\n",size,part,flat);
	fprintf(fp,"c SOURCE: Joseph Culberson (joe@cs.ualberta.ca)\n");
	fprintf(fp,"c DESCRIPTION: Quasi-random coloring problem\n");
	fprintf(fp,"c              generated with flatness = %d\n", flat);
	fprintf(fp,"c                             probability = %5.3f\n", 
							prob/MAX_DENSITY);
	fprintf(fp,"c                             known coloring = %d\n", part);
	fprintf(fp,"c                             random seed = %d\n",seed);
	fprintf(fp,"c              Creation Date:  ");
	clock =    time(NULL);
	fprintf(fp,"%s",ctime(&clock));


	/* GENERATE THE GRAPH */
	vset = (int *) calloc(size,sizeof(int));
	create_and_shuffle(vset,size);

	graph = (char *) malloc((unsigned) GRAPHSIZE);
	bzero(graph, GRAPHSIZE);
	edges=0;

	create_graph(graph,vset,size,prob,part,flat,&edges);

	/* print degree sequence */

	mind = size; maxd = 0;
	for(i=0;i<size;i++) {
		deg = 0;
		for(j=0;j<size;j++) 
			if (isedge(i,j) ) deg++;
		/* printf("%d ",deg); */
		if (maxd < deg) maxd = deg;
		if (mind > deg) mind = deg;
	}
	fprintf(fp,"c Maximum degree = %d Minimum degree = %d\n",maxd,mind);

	/* verify the coloring */
	verify_color(graph,size,part,vset,fp);

	/* LIST THE CREATED COLORS FOR VERTICES FROM 1 to N */
	/* print_order(vset,size,fp); */

	/* OUTPUT THE GRAPH */
	print_graph(graph,size,edges,fp);


	/* CLEAN UP */
	free(vset);
	free(graph);
	fclose(fp);
}
