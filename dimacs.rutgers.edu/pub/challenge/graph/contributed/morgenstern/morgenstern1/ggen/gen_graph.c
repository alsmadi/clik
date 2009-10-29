/* FILE       : gen_graph.c
 * WRITTEN BY : Craig Morgenstern
 */

# include <math.h>
# include "alloc.h"
# include "random.h"
# include "graph.h"

/* ----------------------------------------------------------------------------
 * ----------------------------------  Und  -----------------------------------
 * ----------------------------------------------------------------------------
 * Geometric graph (or complement).  Select 2n numbers in (0,1) uar, and view 
 * them as coordinates of n points on the unit square.  Each point represents a
 * vertex, and two vertices are joined by an edge if their Euclidean distance
 * is d or smaller.  Assumes that initially the graph consists of only isolated
 * vertices ( i.e., that the graph data structure has been initialized to be
 * empty ).
 */

void Und(G, n, d, s, complement)

   graph     G;     /* Empty graph (eg., newly created) */
   int       n;     /* number of vertices */
   double    d;     /* Euclidean distance,  0 < d < 1 */
   unsigned *s;     /* seed for drand  */
   int       complement;  /* 1 ==> generate complement */
{
   int    i, j;
   double Dx, Dy, *X, *Y;

   X = (double *) alloc((unsigned) n * sizeof(double));
   Y = (double *) alloc((unsigned) n * sizeof(double));

   for (i = 0;  i < n;  ++i)  *(X + i) = drand(s);
   for (i = 0;  i < n;  ++i)  *(Y + i) = drand(s);

   if (!complement)
      for (i = 0;  i < n - 1;  ++i)
         for (j = i + 1;  j < n;  ++j)
	 {
	    Dx = *(X + i) - *(X + j);
	    Dy = *(Y + i) - *(Y + j);
            if (sqrt(Dx * Dx + Dy * Dy) <= d) add_edge(G, i, j);
	 }
   else
      for (i = 0;  i < n - 1;  ++i)
         for (j = i + 1;  j < n;  ++j)
	 {
	    Dx = *(X + i) - *(X + j);
	    Dy = *(Y + i) - *(Y + j);
            if (sqrt(Dx * Dx + Dy * Dy) > d) add_edge(G, i, j);
	 }

   free((char *) X);
   free((char *) Y);
}

/* ----------------------------------------------------------------------------
 * --------------------------------  Gnp  -------------------------------------
 * ----------------------------------------------------------------------------
 * Assumes that vertices are labeled  0 .. n-1, and that 0 < p < 1.  Generates
 * a general graph in which each possible edge is made to be present with
 * probability p.  Assumes that initially the graph consists of only isolated
 * vertices (i.e., that the graph data structure has been initialized to be
 * empty).
 */

void Gnp(G, n, p, s)
     
   graph     G;     /* Empty graph (eg., newly created) */
   int       n;     /* number of vertices */
   double    p;     /* edge probability */
   unsigned *s;     /* seed for drand  */
{
   int i, j;

   for (i = 0;  i < n-1;  ++i)
      for (j = i+1;  j < n;  ++j)
         if (drand(s) <= p)
            add_edge(G, i, j);
}

/* ----------------------------------------------------------------------------
 * ----------------------------------  Gknp  ----------------------------------
 * ----------------------------------------------------------------------------
 *  Construct a random K-partite graph as follows :
 *    0. Assume that the graph initially contains only isolated vertices.
 *    1. Assign vertices with labels 0 .. K-1 to the class with the same label.
 *    2. Randomly assign each vertex labeled K .. n-1 to one of the K classes.
 *    3. If vertices v1 and v2 are assigned to different classes, then
 *       add edge (v1,v2) to the graph with probability p.
 *    4. Do not add edges between vertices that belong to the same class.
 *
 * Assumes that initially the graph consists of only isolated vertices 
 * (i.e., that the graph data structure has been initialized to be empty).
 */

void Gknp(G, k, n, p, s)

   graph     G;     /* Empty graph (eg., newly created) */
   int       k;     /* number of classes to be constructed  */
   int       n;     /* number of vertices; labeled 0 .. n-1 */
   double    p;     /* edge density between classes         */
   unsigned *s;     /* seed for drand and urand */
{
   int v1, v2;      /* vertex labels */
   int *class;      /* class labels  */

   class = (int *) alloc((unsigned) n * sizeof(int));
   for (v1 = 0;  v1 < n;  ++v1) *(class + v1) = urand(s) % k;
   for (v1 = 0;  v1 < n-1;  ++v1)
      for (v2 = v1 + 1;  v2 < n;  ++v2)
         if (*(class + v1) != *(class + v2) && drand(s) <= p)
            add_edge(G, v1, v2);
   free((char *) class);
}

/* ----------------------------------------------------------------------------
 * --------------------------  LeightonGraph  ---------------------------------
 * ----------------------------------------------------------------------------
 * An implementation of the graph generation algorithm given by F.T. Leighton
 * in the Journal of Research of the National Bureau of Standards,  vol. 84,
 * no. 6,  Nov-Dec 1979, pp 489-505.  The variable names closely follow those
 * given in the paper.
 */

void LeightonGraph(G, n, X, K, a, c, m, b)

   graph    G;     /* Empty graph (eg., newly created) */
   unsigned X;
   int      n, K, a, c, m;
   int      b[];
{
   int      L1, L2, L3, v1, v2;
   int     *Y;
     
   Y = (int *) alloc(((unsigned) n) * sizeof(int));
   for (L1 = K;  L1 >= 2;  --L1)
      for (L2 = 1;  L2 <= b[L1];  ++L2)
      {
         for (L3 = 1;  L3 <= L1;  ++L3)
         {
            X = (a * X + c) % m;
            *(Y + L3) = X % n;
         }
     
         for (v1 = 1;  v1 < L1;  ++v1)
            for (v2 = v1 + 1;  v2 <= L1;  ++v2)
               if (!edge(G, *(Y+v1), *(Y+v2)))
                  add_edge(G, *(Y+v1), *(Y+v2));
       }
   free((char *) Y);
}

/* ----------------------------------------------------------------------------
 * --------------------------  LeightonProof  ---------------------------------
 * ----------------------------------------------------------------------------
 * Proof of the K colorability of the Leighton graph generated with the given
 * parameters.  Returns true (1) if the graph is K-colorable as per 
 * Leighton's proof method, and returns false (0) otherwise.
 */

int LeightonProof(G, n, X, K, a, c, m)

   graph G;
   unsigned X;
   int   n, K, a, c, m;
{
   int      *color;
   int      colored, Y, i, j;

   color = (int *) alloc((unsigned) n * sizeof(int));

   colored = 0;
   i = 0;
   for (j = 0;  j < n;  ++j)  *(color + j) = -1;
     
   while (colored < n)
   {
      X = (a * X + c) % m;
      Y = X % n;
      ++i;
      if (*(color + Y) == -1)
      {
         ++colored;
         *(color+Y) = i % K;
         for (j = 0;  j < n ;  ++j)
            if (j != Y && *(color + Y) == *(color + j) && edge(G, Y, j))
               return(0);
      }
      else
         if ((i % K) != *(color+Y))  return(0);
   }

   free((char *) color);
   return(1);
}
