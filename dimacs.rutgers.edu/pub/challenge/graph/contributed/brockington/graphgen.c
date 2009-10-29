

/* Graph generator submitted by Mark Brockington (brock@cs.ualberta.ca) */
/* and Joe Culberson (joe@cs.ualberta.ca) for the 1993 DIMACS Challenge */
/* Pieces of code were borrowed from the conversion programs written    */
/* by Tamas Badics.                                                     */ 

/* To compile:
   On Sun systems: gcc graphgen.c -DSUN -lm
   On other systems: cc graphgen.c (link the math library for the pow
    function used in subroutine func1).
*/

/* This code is intended to hide an independent set of sizes larger than 
   the naturally occuring independent sets (as predicted by Matula's formula)
*/

/* Defender=i (0<=i<=4) means that after i vertices are selected from the
   independent set, with neighbours removed, the graph degree sequence looks
   like a random graph.  Details to follow in full paper.
*/ 

/* The random number generators have been abstracted in most of the 
   routines so that you should be able to add your favourite random 
   number generator after the #else token in the following three 
   routines.

    void set_generator() starts up the generator.
    void rand_seed(int) seeds the random number generator.
    int rand_num() returns a 32-bit "random" integer. 
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MAXGRAPH 4095   /* Maximum size of graph allowed to be    */
                        /* created and must be of the form 2^n-1. */

#define TRUE 1
#define FALSE 0

char *graph;
int graphcreated;
int type1[MAXGRAPH];

#ifdef SUN
static long randstate[32] = {
 3,
 0x9a319039, 0x32d9c024, 0x9b663182, 0x5da1f342,
 0x7449e56b, 0xbeb1dbb0, 0xab5c5918, 0x946554fd,
 0x8c2e680f, 0xeb3d799f, 0xb11ee0b7, 0x2d436b86,
 0xda672e2a, 0x1588ca88, 0xe369735d, 0x904f35f7,
 0xd7158fd6, 0x6fa6f051, 0x616e6b96, 0xac94efdc,
 0xde3b81e0, 0xdf0a6fb5, 0xf103bc02, 0x48f340fb,
 0x36413f93, 0xc622c298, 0xf5a42ab8, 0x8a88d77b,
 0xf5ad9d0e, 0x8999220b, 0x27fb47b9
};
#endif

void set_generator()  
{
  unsigned seed; 
  int n;
 
#ifdef SUN
  seed = 1;
  n = 128;
  initstate(seed, (char *) randstate, n);
  setstate(randstate);
#else

#endif
} /* set_generator */

void rand_seed(i_seed)
int i_seed;
{
#ifdef SUN
  setstate(randstate); 
  srandom(i_seed); 
#else
  srand(i_seed); 
#endif
} /* rand_seed */

int rand_num() 
{
#ifdef SUN
  return((int)(random()));
#else
  return(rand());
#endif
}

int creategraph(size,set1,prob12,prob22)
int size;
int set1;
float prob12;
float prob22;
{
  int rval,cw,cx,cy,cz;
  int iprob12,iprob22;
  int rowsize;
  int numedges;
  int t[MAXGRAPH];
  float eta,total;
  int delta;
  int vertype[MAXGRAPH];
  char *ngraph;
  int asize;

  asize = ((size>>3)+1) * size; 
  ngraph = (char *)malloc(asize);
  if (ngraph == NULL) { 
    fprintf(stderr,"*** malloc (for graph) failed\n");
    exit(-1);
  }
  bzero(ngraph,asize);
 
  for(cx=0;cx<size;cx++) {
    vertype[cx] = FALSE;
  }     
 
  /* Choose members of the independent set */

  delta = 0.0;
  eta = (2.0*delta)/((float)(set1-1));
  total = 0.0;
  cy = 0;
  while (cy<set1) {
    cx = rand_num() & MAXGRAPH;
    if ((cx<size) && (vertype[cx] == FALSE)) {
      type1[cy] = cx;
      total += prob12*((1.0-delta)+(eta*cy));
      t[cy] = (int)((total*1048576.0/((float)(set1*prob12)))+0.5);
      cy++; 
      vertype[cx] = TRUE;
    }
  }
  type1[cy] = -1;
  t[cy-1] = 1048576;

  /* Add edges to graph */
  
  rowsize = (size>>3)+1;
  iprob12 = (int)(prob12*1048576.0+0.5); 
  iprob22 = (int)(prob22*1048576.0+0.5);

  total = 0.0;

  for(cx=0;cx<size;cx++) {
   if (vertype[cx] == FALSE) {
    for(cy=0;cy<cx;cy++) {

      cz = rand_num() & 1048575;

      if (vertype[cy] == FALSE) {
       if(cz<iprob22) {
        ngraph[(cx)*rowsize+(cy>>3)] |= (1<<(cy&7));
        ngraph[(cy)*rowsize+(cx>>3)] |= (1<<(cx&7));
       }
      }
    } /* for loop through minor edges */

    /* Now we link to independent edge vertices */

    total += (set1*prob12);
    while (total >= 1.0) { 
      cz = rand_num() & 1048575;
      cw = 0;           
      while (t[cw] < cz) {cw++;}
      cy = type1[cw]; 
      if ((ngraph[(cx)*rowsize+(cy>>3)] & (1<<(cy&7))) == 0) {
        ngraph[(cx)*rowsize+(cy>>3)] |= (1<<(cy&7));
        ngraph[(cy)*rowsize+(cx>>3)] |= (1<<(cx&7));
        total -= 1.0;
      } /* if */
    } /* while */     
 
   } /* if not in independent set */
  } /* for loop through major edges */ 

  /* Now, invert the graph to change the ind. set to a clique */

  graph = (char *)malloc(asize);
  if (graph == NULL) { 
    fprintf(stderr,"*** malloc (for graph) failed\n");
    exit(-1);
  }
  bzero(graph,asize);

  numedges = 0;
  for(cx=0;cx<size;cx++) {
    for(cy=0;cy<cx;cy++) {
      if ((ngraph[(cx)*rowsize+(cy>>3)] & (1<<(cy&7))) == 0) {
        graph[(cx)*rowsize+(cy>>3)] |= (1<<(7-(cy&7)));
        numedges++;
      }
    }
  } /* for */

  free(ngraph);
  graphcreated = TRUE;
  return(numedges);

} /* creategraph */

/* This function is used by computehiding to obtain skewed probabilities */

double func1(x,p,n,s,i) 
double x;
double p;
int n;
int s;
int i;
{
  double f1,f2;
  
  f1 = pow((double)(1.0-x),(double)i); 
  f2 = x*f1*((double)(n-s)) - p*( ((double)(s-i-1)) + f1*((double)(n-s)) ); 
  return(f2);

} /* func1 */

int computehiding(size,set1,f_prob,defensive)
int size;
int set1;
float f_prob;
int defensive;
{
  int fl,idef;
  double min,mid,max;
  double fmin,fmid,fmax;
  double d_prob;
  double d_epsilon,eps;
  float prob12,prob22,prtemp;
 
  d_epsilon = 1.0e-12;
  d_prob = (double)f_prob;
  idef = defensive - 1;

  min = d_prob;
  max = (1.0 + ((double)idef)*((double)d_prob) );
  max /= (1.0 + ((double)idef));
  if (idef == 0) { max = 0.99; }
  fmin = func1(min,d_prob,size,set1,idef); 
  fmax = func1(max,d_prob,size,set1,idef);

  if (abs(fmax) < d_epsilon) {

    prob12 = (float) max;

  } else if (fmax < 0.0) {

    printf("Level of hiding and edge density chosen give a non-real probability.\n");
    exit(-1);

  } else {

    /* Use bisection to find the root of the function. */
  
    mid = (min+max)/2.0;
    fmid = func1(mid,d_prob,size,set1,idef); 
    while (fabs(fmid) >= d_epsilon) {
      if (fmid < 0) {
        min = mid;
      } else { 
        max = mid;
      }
      mid = (min+max)/2;
      fmid = func1(mid,d_prob,size,set1,idef); 
    } /* while */
    prob12 = (float) mid;

  }
  
  prtemp = (float)(pow((double)(1.0-prob12),(double)idef)) * ((float)(size-set1)); 
  prob22 = prob12*(prtemp-((double)(set1-idef)))/( ((float)prtemp) - 1.0 ); 

  fl = creategraph(size,set1,prob12,prob22);
  return(fl);

} /* computehiding */

/* removegraph:  Removes the graph from the system's memory.  Returns -1 if
   there is no graph currently available, and 0 if to indicate success at 
   removing a graph.                                                       */

int removegraph()
{
  if(graphcreated != TRUE) {
    return(-1);
  } else {
    free(graph);
    graphcreated = FALSE;
    return(0);
  } 
}

void parm_error(string)
char *string;
{
  fprintf(stderr,"Argument: %s ... Invalid Parameter\n",string); 
  fprintf(stderr,"GGES [arg1 [arg2 ...]]\n");
  fprintf(stderr,"arg is of the form: -gxxxx     (Graph Size)\n");
  fprintf(stderr,"                    -cxxxx     (Clique Size)\n");
  fprintf(stderr,"                    -px.xxxxxx (Edge Density Requested)\n");
  fprintf(stderr,"                    -dx        (Defender Type)\n");
  fprintf(stderr,"                    -sxxxx     (Seed Number)\n\n");
  fprintf(stderr,"Defaults: -g1000 -i16 -p0.500 -d0 -s0\n");
  exit(-1);
} /* parm_error */

void main(argc,argv)
int argc;
char *argv[];
{
 
  int cx,cy;
  int gsize;
  int isize;
  float input_prob;
  int defender;
  int seed;
  int numedges;

  FILE *fp;
  char filename[255];
  char mm[255];
  char preamble[2000];
  int prsize;
  char *gptr;

  /* Set the random number generator */
  set_generator();

  /* Set default parameters */
  gsize = 1000;
  isize = 16;
  input_prob = 0.5;
  defender = 0;
  seed = 0;

  /* Read command line arguments */
  cx = 1;
  while(cx < argc) {

    if (argv[cx][0] != '-') { parm_error(argv[cx]); } 

    switch(argv[cx][1]) {

     case 'g':
     case 'G':
      sscanf(argv[cx],"%*c%*c%d",&gsize);
      break;

     case 'c': 
     case 'C': 
      sscanf(argv[cx],"%*c%*c%d",&isize);
      break;

     case 'p':
     case 'P':
      sscanf(argv[cx],"%*c%*c%f",&input_prob);
      break;

     case 'd':
     case 'D':
      sscanf(argv[cx],"%*c%*c%d",&defender);
      break;

     case 's':
     case 'S':
      sscanf(argv[cx],"%*c%*c%d",&seed);
      break;

     default:
      parm_error(argv[cx]);
      break;

    } /* switch */
    cx++;
  }

  if (gsize > MAXGRAPH) {
    fprintf(stderr,"Graph is too large (maximum: %d) \n",MAXGRAPH);
    exit(-1);
  } 

  if (isize > gsize) {
    fprintf(stderr,"Clique is larger than graph.\n");
    exit(-1);
  }

  if ((defender > 4) || (defender<0)) {
    fprintf(stderr,"Defender Unknown (must be in between 0 and 4)\n");
    exit(-1);
  }  

  input_prob = 1.0 - input_prob;

  rand_seed(seed);

  switch(defender) {
    case 0:
      numedges = creategraph(gsize,isize,input_prob,input_prob);
      break; 
    case 1:
    case 2:
    case 3:
    case 4:
      numedges = computehiding(gsize,isize,input_prob,defender);
      break;
  }

  input_prob = 1.0 - input_prob;

  printf("Input file name to write file to: (\".b\" will be appended)\n");
  gets(filename);
 
  strcat(filename,".b"); 
  fp = fopen(filename,"w");
  if (fp == NULL) {
    fprintf(stderr,"Error in creating %s: aborting.\n", filename);
    exit(-1);
  } /* if */

  /* Write preamble */

  if (gsize < 1000) {
    prsize = (int)(numedges/10485.76 + 0.5);
  } else if (gsize < 1400) {
    prsize = (int)(numedges/9532.51 + 0.5);
  } else { 
    prsize = (int)(numedges/8738.11 + 0.5);
  } /* if */

  sprintf(mm,"c Graph Generator\nc\n");
  strcpy(preamble,mm);
  sprintf(mm,"c By: Mark Brockington (brock@cs.ualberta.ca)\n");
  strcat(preamble,mm);
  sprintf(mm,"c and Joe Culberson (joe@cs.ualberta.ca)\nc\n");
  strcat(preamble,mm);
  sprintf(mm,"c Graph Size:%d, Clique Size: %d\n",gsize,isize);
  strcat(preamble,mm);
  sprintf(mm,"c Seed:%d, Edge Density: %8.5f\n",seed,input_prob);
  strcat(preamble,mm);
  sprintf(mm,"c Depth %d Hiding\nc\n",defender);
  strcat(preamble,mm);
  sprintf(mm,"c Clique Elements are:");
  strcat(preamble,mm);
  for(cx=0;cx<isize;cx++) {
    if(cx%10 == 0) {
      sprintf(mm,"\nc "); 
      strcat(preamble,mm);
    }
    sprintf(mm,"%d ",type1[cx]);
    strcat(preamble,mm);
  } /* for */
 
  sprintf(mm,"\nc\nc ");
  strcat(preamble,mm);
  if (prsize>20) {
    sprintf(mm,"CAUTION! ");
    strcat(preamble,mm);
  }

  sprintf(mm,"Estimated Size of Uncompressed File: %5.1fMB\nc\n",(prsize/10.0));
  strcat(preamble,mm);
  sprintf(mm,"p edge %d %d\n",gsize,numedges); 
  strcat(preamble,mm);
  
  printf("%s",preamble);

  fprintf(fp,"%d\n",strlen(preamble));
  fprintf(fp,preamble);
 
  for(cx=0;cx<gsize;cx++) { 
    gptr = graph + cx*((gsize>>3)+1);
    fwrite(gptr, 1, (int)((cx + 8)/8), fp);
  }

  fclose(fp);

} /* main */
