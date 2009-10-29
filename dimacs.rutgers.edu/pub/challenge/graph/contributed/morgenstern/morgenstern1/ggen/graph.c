/* FILE      : graph.c
 * WRITTEN BY: Craig Morgenstern
 */

# include <stdio.h>
# include "alloc.h"
# include "graph.h"
void exit();

# define NIL -1

/* ----------------------------------------------------------------------------
 * ------------------  new_adjnode and dispose_adjnode  -----------------------
 * ----------------------------------------------------------------------------
 */

static void dispose_adjnode(G, p)

   graph G;
   int p;
{
   *(G->next + p) = G->free;  G->free = p;
}

static int new_adjnode(G)

   graph G;
{
   int p;

   if (G->free != NIL)
   {
      p = G->free;
      G->free = *(G->next + G->free);
   }
   else if (G->pristine < G->maxe - 1)
      p = (G->pristine)++;
   else
   {
      fprintf(stderr, "*** Out of adjacency nodes\n");
      exit(1);
   }
   return(p);
}

/* ----------------------------------------------------------------------------
 * ------------------------  new_graph  ---------------------------------------
 * ----------------------------------------------------------------------------
 */

graph new_graph()
{
   graph G;

   G = (graph) alloc(sizeof(graph_struct));
   G->maxn = 0;
   G->maxe = 0;
   G->gtype= NIL;
   G->D = NULL;
   G->M = NULL;
   G->A = NULL;
   G->adjnode = NULL;
   G->next = NULL;
   return(G);
}

/* ----------------------------------------------------------------------------
 * ------------------------  dispose_graph  -----------------------------------
 * ----------------------------------------------------------------------------
 */

void dispose_graph(G)

   graph G;
{
   if (G->D != NULL) free((char *) G->D);
   if (G->M != NULL) free((char *) G->M);
   if (G->A != NULL) free((char *) G->A);
   if (G->adjnode != NULL) free((char *) G->adjnode);
   if (G->next != NULL) free((char *) G->next);
   free((char *) G);
}

/* ----------------------------------------------------------------------------
 * -----------------------  create_graph  -------------------------------------
 * ----------------------------------------------------------------------------
 */

void create_graph(G, gtype, n, e)

   graph G;
   int gtype, n, e;

{
   int i;

   if (n > G->maxn  ||  e > G->maxe  ||  gtype != G->gtype)
   {
      if (G->D != NULL) free((char *) G->D);
      switch (G->gtype)
      {
         case DENSE:
	 {
            if (G->M != NULL) free((char *) G->M);
	    break;
	 }
         case SPARSE:
	 {
            if (G->A != NULL) free((char *) G->A);
            if (G->adjnode != NULL) free((char *) G->adjnode);
            if (G->next != NULL) free((char *) G->next);
	    break;
	 }
      }

      G->maxn = n;
      G->maxe = e;
      G->gtype = gtype;
      G->D = (int *) alloc(((unsigned) n) * sizeof(int));

      switch (gtype)
      {
         case DENSE:
	 {
            G->M = alloc(((unsigned) n*n));
	    break;
	 }
         case SPARSE:
	 {
            G->A = (int *) alloc(((unsigned) n) * sizeof(int));
            G->adjnode = (int *) alloc(((unsigned) e) * sizeof(int));
            G->next = (int *) alloc(((unsigned) e) * sizeof(int));
	    break;
	 }
      }
   }

   G->n = 0;
   G->e = 0;
   G->h = NIL;
   for (i = 0;  i < n;  ++i) *(G->D + i) = 0;
   switch(gtype)
   {
      case DENSE:
      {
	 for (i = 0;  i < n*n;  ++i) *(G->M + i) = 0;
	 break;
      }
      case SPARSE:
      {
	 for (i = 0;  i < n;  ++i) *(G->A + i) = NIL;
	 G->free = NIL;
	 G->pristine = 0;
	 break;
      }
   }
}

/* ----------------------------------------------------------------------------
 * -----------------------------  add_edge  -----------------------------------
 * ----------------------------------------------------------------------------
 */

void add_edge(G, v1, v2)

   graph G;
   int v1, v2;
{
   switch (G->gtype)
   {
      case DENSE:
      {
         *(G->M + G->maxn * v1 + v2) = 1;  
         *(G->M + G->maxn * v2 + v1) = 1;  
         break;
      }
      case SPARSE:
      {
         int  p;

         p = new_adjnode(G);  
	 *(G->next + p) = *(G->A + v1);  
	 *(G->A + v1) = p;
	 *(G->adjnode + p) = v2;

         p = new_adjnode(G);  
	 *(G->next + p) = *(G->A + v2);  
	 *(G->A + v2) = p;
	 *(G->adjnode + p) = v1;
         break;
      }
   }

   ++(G->e);
   if (++(*(G->D + v1)) == 1)
   { 
      ++(G->n); 
      if (v1 > G->h) G->h = v1; 
   }
   if (++(*(G->D + v2)) == 1)
   { 
      ++(G->n); 
      if (v2 > G->h) G->h = v2; 
   }
}

/* ----------------------------------------------------------------------------
 * ----------------------------  remove_edge  ---------------------------------
 * ----------------------------------------------------------------------------
 */

void remove_edge(G, v1, v2)

   graph G;
   int v1, v2;
{
   switch (G->gtype)
   {
      case DENSE:
      {
         *(G->M + G->maxn * v1 + v2) = 0;
         *(G->M + G->maxn * v2 + v1) = 0;  
         break;
      }
      case SPARSE:
      {
         int p, q;

         q = NIL;
	 p = *(G->A + v1);
	 while (*(G->adjnode + p) != v2)
	 {
	    q = p;
	    p = *(G->next + p);
	 }
         if (q == NIL)
            *(G->A + v1) = *(G->next + p);
         else
            *(G->next + q) = *(G->next + p);
         dispose_adjnode(G, p);

         q = NIL;
	 p = *(G->A + v2);
	 while (*(G->adjnode + p) != v1)
	 {
	    q = p;
	    p = *(G->next + p);
	 }
         if (q == NIL)
            *(G->A + v2) = *(G->next + p);
         else
            *(G->next + q) = *(G->next + p);
         dispose_adjnode(G, p);
         break;
      }
   }

   --(G->e);
   if (--(*(G->D + v1)) == 0) --(G->n); 
   if (--(*(G->D + v2)) == 0) --(G->n); 
   while (G->h != NIL && *(G->D + G->h) == 0)  --(G->h);
}

/* ----------------------------------------------------------------------------
 * ----------------------------  edge  ----------------------------------------
 * ----------------------------------------------------------------------------
 */

int edge(G, v1, v2)

   graph G;
   int v1, v2;
{
   switch (G->gtype)
   {
      case DENSE:
      {
         return(*(G->M + G->maxn * v1 + v2));
      }
      case SPARSE:
      {
         int  p, t;

         if (*(G->D + v1) < *(G->D + v2))
	 {
	    t = v1;
	    v1 = v2;
	    v2 = t; 
	 }
         p = *(G->A + v1);
	 while (p != NIL && *(G->adjnode + p) != v2) p = *(G->next + p);
         return(p != NIL);
      }
      default: /* to make lint happy */
	 return(0);
   }
}

/* ----------------------------------------------------------------------------
 * -----------------------------  remove_vertex  ------------------------------
 * ----------------------------------------------------------------------------
 */

void remove_vertex(G, v1)

   graph G;
   int   v1;
{
   if (*(G->D+v1) == 0)  return;

   switch (G->gtype)
   {
      case DENSE: 
      {
         int  v2, e, newe;

         e = G->e;
         newe = e - *(G->D + v1);
         for (v2 = 0;  e > newe;  ++v2)
            if (*(G->M + G->maxn * v1 + v2))
            {
               --e;
               *(G->M + G->maxn * v1 + v2) = 0;
               *(G->M + G->maxn * v2 + v1) = 0;  
               if (--(*(G->D + v2)) == 0)  --(G->n);
            }
         break;
      }

      case SPARSE:
      {
         int  v2, p, q;

         do 
         { 
            p = *(G->A + v1); 
            v2 = *(G->adjnode + p);
            *(G->A + v1) = *(G->next + p); 
            dispose_adjnode(G, p);

            q = NIL;
	    p = *(G->A + v2);
	    while (*(G->adjnode + p) != v1)
	    {
	       q = p;
	       p = *(G->next + p);
	    }
            if (q == NIL)
               *(G->A + v2) = *(G->next + p);
            else
               *(G->next + q) = *(G->next + p);
            dispose_adjnode(G, p);
            if (--(*(G->D + v2)) == 0)  --(G->n);
         } 
         while (*(G->A + v1) != NIL);
         break;
      }
   }

   G->e -= *(G->D + v1);
   *(G->D + v1) = 0;  
   --(G->n);
   while (G->h != NIL && *(G->D + G->h) == 0)  --(G->h);
}

/* ----------------------------------------------------------------------------
 * ---------------------------  all_neighbors  --------------------------------
 * ----------------------------------------------------------------------------
 */

void all_neighbors(G, v, d, nlist)

   graph G;
   int   v, *d, *nlist;
{
   switch (G->gtype)
   {
      case DENSE:
      {
         char *a;
         int  w, Dv;

         a = G->M + G->maxn * v;
	 Dv = *(G->D + v);
         for (w = 0, *d = 0; *d != Dv; ++w) if (*(a++)) *(nlist + (*d)++) = w;
         break;
      }

      case SPARSE:
      {
         int    p;

         for (*d = 0, p = *(G->A + v);  p != NIL;  p = *(G->next + p))
            *(nlist + (*d)++) = *(G->adjnode + p);
         break;
      }
   }
}

/* ----------------------------------------------------------------------------
 * -------------------  neighborhood_scan routines  ---------------------------
 * ----------------------------------------------------------------------------
 */

neighborhood_scan new_neighborhood_scan()
{
   neighborhood_scan N;

   N = (neighborhood_scan) alloc(sizeof(neighborhood_scan_struct));
   return(N);
}

void dispose_neighborhood_scan(N)

   neighborhood_scan N;
{
   free((char *) N);
}

void create_neighborhood_scan(G, N, v)

   graph G;
   neighborhood_scan N;
   int   v;
{
   N->G = G;
   switch (G->gtype)
   {
      case DENSE:
      {
	 N->start = G->M + G->maxn * v;
	 N->next = N->start;
	 N->more = *(G->D + v);
	 break;
      }
      case SPARSE:
      {
	 N->p = *(G->A + v);	
	 N->more = *(G->D + v);
	 break;
      }
   }
}

int new_neighbor(N, w)

   neighborhood_scan N;
   int *w;
{
   graph G;

   if (N->more <= 0) return(0);

   G = N->G;
   switch (G->gtype)
   {
      case DENSE:
      {
	 for (;;)
	 {
	    if (*(N->next))
	    {
	       *w = N->next - N->start;
	       --(N->more);
	       ++(N->next);
	       return(1);
	    }
	    else
	       ++(N->next);
	 }
      }
      case SPARSE:
      {
	 *w = *(G->adjnode + N->p);
	 N->p = *(G->next + N->p);
         --(N->more);
	 return(1);
      }
      default:  /* to make lint happy */
	 return(0);
   }
}
