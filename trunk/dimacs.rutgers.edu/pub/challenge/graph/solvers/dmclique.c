/* mclique.c (maxsize)
 * Semi-exhaustive Greedy Clique (for graph coloring)
 * from Matula and Johri
 *
 * written October 22, 1983 by DSJ
 * modified February 20, 1987 for checkpointing
 * modified August 1988 to find maximum independent sets
 * modified Summer 1993 by DSJ,DLA to speed up exhaustive part
 * modified September 1993 to accept DIMACS input .b format
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/times.h>
#define INT 32		/* computer word size */
#define CAND_NUM 1	/* default number of candidates for "random" choice */
#define TRIAL_NUM 1	/* default number of sets tried per color */
#define ITERATIONS 1	/* default number of iterations */
#define CHARBITS 8
#define edge(x,y) ((bitmap[y/CHARBITS][x] & (1<<(y%CHARBITS))))
/* for independent sets, add "!" before "bitma" in above macro */
#define SGI

#define NMAX 8400	/* maximum number of vertices handles */
#define MAX_NR_VERTICES		8400	/* = NMAX */
#define MAX_NR_VERTICESdiv8	1050	/* = NMAX/8 */
#define BOOL	char
#define MAX_PREAMBLE 10000
unsigned char bitmap[NMAX/CHARBITS+1][NMAX+1];
#define oldedge(x,y) (oldbitmap[x][(y-1)/INT+1] & mask[y%INT])
unsigned oldbitmap[NMAX+1][(NMAX+1)/INT+1];

int Nr_vert, Nr_edges;
BOOL Bitmap[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8];
static char Preamble[MAX_PREAMBLE];
char masks[ 8 ] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

double frand(), pow();

unsigned mask[INT] =
   {
   1, 1<<31, 1<<30, 1<<29, 1<<28, 1<<27, 1<<26, 1<<25, 1<<24,
   1<<23, 1<<22, 1<<21, 1<<20, 1<<19, 1<<18, 1<<17, 1<<16,
   1<<15, 1<<14, 1<<13, 1<<12, 1<<11, 1<<10, 1<<9, 1<<8,
   1<<7, 1<<6, 1<<5, 1<<4, 1<<3, 1<<2, 1<<1
   };  /* CAUTION - assumes 32 bit machine */

  /* graph input parameters */

int N;			/* number of vertices in graph */
int mattype;
double x_coord[NMAX+1], y_coord[NMAX+1];
int width;
double dens, maxdist, dens1, dens2;
int chrom_num;		/* for fixed-chrom-num graphs */

int setlim;	/* size at which exhaustive search begins */
int trialnum;	/* number of random starts per color class */
int candnum;	/* number of candidates for "random" vertices */
int iterations;	/* number of iterations */
int parity;	/* for checkpointing */

int vertex[NMAX];	/* array in which vertices reside and are moved */
int nvertex[NMAX];	/* array in which vertices reside and are moved */
int location[NMAX];	/* location[i] is address in array of vertex i */
int degree[NMAX];	/* vertex degrees with resp. to uncolored vertices */
int maxloc;		/* location of vertex with maximum degree */
int maxd, deg;		/* used for computing max, cand degrees */
int K;			/* number of colors used */
int uncolored;		/* pointer to last uncolored node in vertex array */
int gtop;		/* global top */

int set[NMAX];		/* non-zero entries are members of current set */
int startsize;		/* number of random vertices starting current set */
int bestsize,nbestsize;	/* size of current best set */
int setsize;		/* size of current set */
int setdegree;		/* cumulative vertex degree for current set */
int pointer[NMAX];	/* pointer[i] points to loc. of next cand. for set[i+1] */
int bestset[NMAX];	/* non-zero entries constitute currently best ind. set */
int bestdegree;		/* cumulative vertex degree for currently best set */
double bignum;		/* 2**31 */

void next_set();	/* constructs next color class (ind. set) */
void check();		/* checks to see that chosen color class is ind. set */
void readgraph();	/* reads input graph in bitmap form */

struct tms buffer;
int utimer, stimer, rtimer;
double utime, stime, rtime;

FILE *fopen(), *checkpoint;
char inputname[40];

main (argc,argv)
int argc;
char *argv[];
{
	int i,j;
	int count;
	FILE *inputfile;

	setbuf(stdout,NULL);
	times(&buffer);
	utimer = buffer.tms_utime;
	stimer = buffer.tms_stime;
	rtimer = time(0);

	/* initialize random number generator */
#ifdef SGI
	srandom((int)time(0));
	/* Calculate bignum */
	bignum = (float)1024*1024;
	bignum *= 2048;
#else
	srand((int)time(0));
#endif

	/* read input */
	if (argc < 2) {
		printf("Usage: dmclique <filename> [setlim][trialnum][candnum][iter]\n");
		exit(1);
	}
	strcpy(inputname,argv[1]);
	if ((inputfile = fopen(argv[1], "r")) == NULL) {
		printf("Input graph does not exist");
		exit(1);
	}
	readgraph(inputfile);

	times(&buffer);
	utime = (buffer.tms_utime - utimer)/60.0;
	stime = (buffer.tms_stime - stimer)/60.0;
	rtime = (time(0) - rtimer)/1.0;

	/* initialize algorithmic parameters */
	setlim = N;
	if (argc > 2) setlim = atoi(argv[2]);
	trialnum = TRIAL_NUM;
	if (argc > 3) trialnum = atoi(argv[3]);
	candnum = CAND_NUM;
	if (argc > 4) candnum = atoi(argv[4]);
	iterations = ITERATIONS;
	if (argc > 5) iterations = atoi(argv[5]);
	printf("\nDMCLIQUE(%s) setlim=%d trialnum=%d candnum=%d iterations = %d\n",argv[1],setlim,trialnum,candnum,iterations);

	printf("Input Time:  %8.2f (user)  %8.2f (sys)  %8.2f (real)\n", utime, stime, rtime);


	for (count = 1; count <= iterations; count++ ) {

		utimer = buffer.tms_utime;
		stimer = buffer.tms_stime;
		rtimer = time(0);

		/* initialize vertex and degree arrays */
		for (i=1;i<=N;i++) {
			vertex[i] = i;
			degree[i] = 0;
			for (j=1;j<=N;j++)
				if (!edge(i,j)) degree[i]++;
		}

		next_set();

		times(&buffer);
		utime = (buffer.tms_utime - utimer)/100.0;
		stime = (buffer.tms_stime - stimer)/100.0;
		rtime = (time(0) - rtimer)/1.0;
		printf("%8.2f (user)  %8.2f (sys)  %8.2f (real)\n", utime, stime, rtime);
	}
}

void next_set()
{
	register int top;	/* position of highest currently unassigned vertex */
	register int i, temp;
	double degreesum;
	int j,h, loc;
	int first;
	int totd;
	int trials;
	int k;		/* iteration counter */
	int v;		/* next candidate for membership in ind. set */
	int next;	/* "randomly" chosen location for v in part I */
	int candloc;	/* location of candidate for next "random" choice */
	int cand,newcand;	/* candidate for next "random" choice */
	int c;		/* choice counter */
	int lastsize,dmax,a;
	double rndm;	/* used to compute next */


	/* initialize current set generation */
	for (i=1;i<=N;i++) {
		set[i] = 0;
		bestset[i] = 0;
	}
	bestsize = 0;

	/* loop for trialnum iterations */
	for (h=1;h<=trialnum;h++) {

		first = 1;

		pointer[0] = top = N;
		startsize = 0;

		/* random independent set generation */
		while (top > setlim) {
			maxd = N;
			if (first == 1) {
				next = (int)(frand()*top) + 1;
				first = 0;
			}
			else {
			    for (c = 1; c <= candnum; c++) {
				candloc = (int)(frand()*top) + 1;
				cand = vertex[candloc];
				deg = 0;
				for (i = 1; i <= top; i++)
					if (!edge(cand,vertex[i])) deg++;
				if (deg <= maxd ) {
					maxd = deg;
					next = candloc;
				}
			    }
			}
			v = vertex[next];
			vertex[next] = vertex[top];
			vertex[top--] = v;
			set[++startsize] = v;
			for (i=1;i<=top;) {
				if (!edge(v,vertex[i])) {
					temp = vertex[i];
					vertex[i] = vertex[top];
					vertex[top--] = temp;
				}
				else i++;
			}
			pointer[startsize] = top;
		}

		degreesum = 0.0;
		for (i=1;i<=top;i++) {
			a = vertex[i];
			degree[i] = 0;
			for (j=1;j<=top;j++)
				if ((i != j)&&(!edge(a,vertex[j]))) {
					degree[i]++;
					degreesum++;
				}
		}
	
/*
		for (j=1;j<=top;j++) printf("%3d ",vertex[j]);
		printf("\n");
		for (j=1;j<=top;j++) printf("%3d ",degree[j]);
		printf("\n");
*/
		dmax = -1;
		for (i=1;i<=top;i++)
			if (degree[i] > dmax) {
				dmax = degree[i];
				cand = i;
			}
		nvertex[top] = vertex[cand];
	
		for (i=top-1;i>=1;i--) {
			degree[cand] = -9;
			dmax = -1;
			a = vertex[cand];
			for (j=1;j<=top;j++) {
				if ((cand != j)&&(!edge(a,vertex[j]))) degree[j]--;
				if (degree[j] > dmax) {
					dmax = degree[j];
					newcand = j;
				}
			}
			cand = newcand;
			nvertex[i] = vertex[cand];

/*
		for (j=1;j<=top;j++) printf("%3d ",degree[j]);
		printf("\n");
*/
		}
	
		nbestsize = 0;
		gtop = top;
/*
		for (j=1;j<=top;j++) printf("%d ",nvertex[j]);
		printf("dens = %.5f\n",degreesum/(top*(top-1)));
		printf("startsize = %d top = %d\n",startsize,top);
*/
		setsize = startsize + maxind(top,1,nvertex,1);
		if (setsize > bestsize) bestsize = setsize;

	}


	printf ("Size = %d", bestsize);
		
}

int maxind(top,goal,array,depth)
register int top, goal,depth;
int *array;
{
	int newarray[NMAX];
	int i,v,u,w,z;
	int best, restbest, newgoal;
	unsigned *bitloc;
	int *pnew, *pold;
	int canthrow;

/* 	printf("maxind(%d,%d,array,%d)\n",top,goal,depth); */
	if (top <= 1) {
		if (top == 0) depth--;
		if (depth > nbestsize) {
			nbestsize = depth;
			if (top == 1) set[nbestsize] = array[top];
			for (i=1;i<=nbestsize;i++) bestset[i] = set[i];
			check();
			/* printf("Size = %2d found\n",nbestsize); */
		}
		return(top);
	}
	best = 1;
	newgoal = goal-1;
	if (newgoal <= 1) newgoal = 1;
	for (i = top; i >= goal; i--) {
		pnew = newarray;
		w = array[i];
		set[depth] = w;
		canthrow = i - goal;
		pold = array+1;
		while (pold<array+i) {
			z = *pold++;
			if (edge(z,w)) {
				 *++pnew = z;
			} else {
			    if (canthrow == 0) goto breakout;
			    canthrow--;
			}
		}
		restbest = maxind(pnew-newarray,newgoal,newarray,depth+1);
		if (restbest >= newgoal) {
			best = newgoal = restbest+1;
			goal = best+1;
		}
		if (top == gtop) {
			times(&buffer);
			utime = (buffer.tms_utime - utimer)/100.0;
			stime = (buffer.tms_stime - stimer)/100.0;
			rtime = (time(0) - rtimer)/1.0;
/*
			printf("N = %3d best = %2d %8.2f (user)  %8.2f (sys)  %8.2f (real)\n",i,best, utime, stime, rtime);
*/
		}
    breakout:;
	}
	return(best);
}

BOOL get_edge( i, j )
int i,j;
{
	int byte, bit;
	char mask;
	int k;

	if (i<j) {
		k = i;
		i = j;
		j = k;
	}
	
	bit  = 7-(j & 0x00000007);
	byte = j >> 3;
	
	mask = masks[bit];
	return( (Bitmap[i][byte] & mask)==mask );
}


void readgraph(fp) 
FILE *fp;
{
   int i,j;
   int length = 0;
   unsigned temp;

	if (!fscanf(fp, "%d\n", &length))
	  { printf("ERROR: Corrupted preamble.\n"); exit(10); }

	if(length >= MAX_PREAMBLE)
	  { printf("ERROR: Too long preamble.\n"); exit(10); }
		   
	fread(Preamble, 1, length, fp);
	Preamble[length] = '\0';
	
	if (!get_params())
		  { printf("ERROR: Corrupted preamble.\n"); exit(10); }

	if (Nr_vert >NMAX) {
		printf("Too many vertices! Recompile with NMAX > %d\n",
			Nr_vert);
		exit(0);
	}

	for ( i = 0
		 ; i < Nr_vert && fread(Bitmap[i], 1, (int)((i + 8)/8), fp)
		 ; i++ );

	fclose(fp);

   N = Nr_vert;

   for (i = 0; i < N; i++)
       for (j = 0; j < N; j++)
	   if (get_edge(i, j))
	       bitmap[(j+1)/CHARBITS][i+1] |= (1 << ((j+1) % CHARBITS));

}

int get_params()
                      /* getting Nr_vert and Nr_edge from the preamble string,
			containing Dimacs format "p ??? num num" */
{
	char c, *tmp;
	char * pp = Preamble;
	int stop = 0;
	tmp = (char *)calloc(100, sizeof(char));
	
	Nr_vert = Nr_edges = 0;
	
	while (!stop && (c = *pp++) != '\0'){
		switch (c)
		  {
			case 'c':
			  while ((c = *pp++) != '\n' && c != '\0');
			  break;
			  
			case 'p':
			  sscanf(pp, "%s %d %d\n", tmp, &Nr_vert, &Nr_edges);
			  stop = 1;
			  break;
			  
			default:
			  break;
		  }
	}
	
	free(tmp);
	
	if (Nr_vert == 0 || Nr_edges == 0)
	  return 0;  /* error */
	else
	  return 1;
	
}


void check ()
{
	/* checks that the vertices with color K are an independent set */
	register int i,j;

	for (i=1;i<=nbestsize;i++)
		for (j=i+1;j<=nbestsize;j++)
			if (!edge(bestset[i],bestset[j]))
				printf ("edge (%d,%d) in set\n",
					bestset[i],bestset[j]);
}

#ifdef SGI
double frand()
{
	double x;

	x = (double)random();
	x /= bignum;
	return(x);	
}
#endif
