/* FILE       : graph_io.c
 * WRITTEN BY : Craig Morgenstern
 */

# include <stdio.h>
# include "alloc.h"
# include "graph.h"
# include "gen_graph.h"
# include "graph_io.h"
void exit();

/* ----------------------------------------------------------------------------
 * ---------------------------  write_edges  ----------------------------------
 * ----------------------------------------------------------------------------
 */

void write_edges(fp, G)

   FILE  *fp;  /* opened to write */
   graph G;    /* existing graph */
{
   int v, w;
   neighborhood_scan N;

   N = new_neighborhood_scan();
   for (v = 0;  v <= high_vertex(G);  ++v)
   {
      create_neighborhood_scan(G, N, v);
      while (new_neighbor(N, &w))
	  if (w > v) fprintf(fp, "e %d %d\n", v+1, w+1);
   }
   dispose_neighborhood_scan(N);
}

/* ----------------------------------------------------------------------------
 * -----------------------  print_graph_stats  --------------------------------
 * ----------------------------------------------------------------------------
 */

void print_graph_stats(fp, G)

   FILE  *fp; /* Output file pointer -- opened to write */
   graph G;
{
   int n, e, max, min, v, d;
   double avg;

   n = high_vertex(G);
   e = num_edges(G);
   max = 0;
   min = n;
   avg = 0.0;
   for (v = 0;  v <= n;  ++v)
   {
       d = vdeg(G, v);
       avg += (double) d;
       if ( d > max )  
           max = d;
       else if ( d < min )  
           min = d;
   }
   fprintf(fp, "c number of vertices  : %d\n", n+1);
   fprintf(fp, "c nonisolated vertices: %d\n", num_vertices(G));
   fprintf(fp, "c number of edges     : %d\n", e);
   fprintf(fp, "c edge density        : %1.6lf\n", 
					      ((double)(e))/((n*(n+1))/2));
   fprintf(fp, "c max degree          : %d\n", max);
   fprintf(fp, "c avg degree          : %1.2lf\n", avg / (double) (n+1));
   fprintf(fp, "c min degree          : %d\n", min);
}

/* ----------------------------------------------------------------------------
 * --------------------------  gen_graph  -------------------------------------
 * ----------------------------------------------------------------------------
 */

void gen_Gnp_graph(fp_in, fp_out, fp_log, G)

   FILE  *fp_in;  /* Input file pointer -- opened to read */
   FILE  *fp_out; /* Output file pointer -- opened to write */
   FILE  *fp_log; /* Output file pointer -- opened to write */
   graph G;       /* graph variable -- allocated, but not created */
{
   int      gtype, n, e;
   unsigned Gseed;
   double   p;

   fprintf(fp_out, "graph gen seed     : "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &Gseed);
   fprintf(fp_log, "c graph gen seed     : %d\n", Gseed); 

   fprintf(fp_out, "number of vertices : "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &n);
   fprintf(fp_log, "c number of vertices : %d\n", n); 

   fprintf(fp_out, "max number of edges: "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &e);
   fprintf(fp_log, "c max number of edges: %d\n", e); 

   fprintf(fp_out, "edge probability   : "); 
   fflush(fp_out);
   fscanf(fp_in, "%lf", &p);
   fprintf(fp_log, "c edge probability   : %1.6lf\n", p); 

   fprintf(fp_out, "sparse(0)/dense(1) : ");
   fflush(fp_out);
   fscanf(fp_in, "%d", &gtype);
   fprintf(fp_log, "c data structure     : %s\n", (gtype)?("dense"):("sparse"));
   create_graph(G, gtype, n, e);
   Gnp(G, n, p, &Gseed);
   fflush(fp_out);
}


void gen_Gknp_graph(fp_in, fp_out, fp_log, G)

   FILE  *fp_in;  /* Input file pointer -- opened to read */
   FILE  *fp_out; /* Output file pointer -- opened to write */
   FILE  *fp_log; /* Output file pointer -- opened to write */
   graph G;       /* graph variable -- allocated, but not created */
{
   int      gtype, k, n, e;
   unsigned Gseed;
   double   d, p;

   fprintf(fp_out, "graph gen seed     : "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &Gseed);
   fprintf(fp_log, "c graph gen seed     : %d\n", Gseed); 

   fprintf(fp_out, "number of vertices : "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &n);
   fprintf(fp_log, "c number of vertices : %d\n", n); 

   fprintf(fp_out, "max number of edges: "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &e);
   fprintf(fp_log, "c max number of edges: %d\n", e); 

   fprintf(fp_out, "number of classes  : "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &k);
   fprintf(fp_log, "c number of classes  : %d\n", k); 

   fprintf(fp_out, "edge density       : "); 
   fflush(fp_out);
   fscanf(fp_in, "%lf", &d);
   p = (d * k) / (k - 1);
   fprintf(fp_log, "c edge density       : %1.6lf\n", d); 
   fprintf(fp_log, "c edge probability   : %1.6lf\n", p); 

   fprintf(fp_out, "sparse(0)/dense(1) : ");
   fflush(fp_out);
   fscanf(fp_in, "%d", &gtype);
   fprintf(fp_log, "c data structure     : %s\n", (gtype)?("dense"):("sparse"));

   create_graph(G, gtype, n, e);
   Gknp(G, k, n, p, &Gseed);
   fflush(fp_out);
}

void gen_Leighton_graph(fp_in, fp_out, fp_log, G)

   FILE  *fp_in;  /* Input file pointer -- opened to read */
   FILE  *fp_out; /* Output file pointer -- opened to write */
   FILE  *fp_log; /* Output file pointer -- opened to write */
   graph G;       /* graph variable -- allocated, but not created */
{
   int      gtype, n, e, k, c;
   int      La, Lc, Lm, *b;
   unsigned Gseed;

   fprintf(fp_out, "sparse(0)/dense(1) : ");
   fflush(fp_out);
   fscanf(fp_in, "%d", &gtype);
   fprintf(fp_log, "c data structure     : %s\n", (gtype)?("dense"):("sparse"));

   fprintf(fp_out, "graph gen seed     : "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &Gseed);
   fprintf(fp_log, "c graph gen seed     : %d\n", Gseed); 

   fprintf(fp_out, "number of vertices : "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &n);
   fprintf(fp_log, "c number of vertices : %d\n", n); 

   fprintf(fp_out, "max number of edges: "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &e);
   fprintf(fp_log, "c max number of edges: %d\n", e); 

   fprintf(fp_out, "number of classes  : "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &k);
   fprintf(fp_log, "c number of classes  : %d\n", k); 

   fprintf(fp_out, "a c m              : "); 
   fflush(fp_out);
   fscanf(fp_in, "%d%d%d", &La, &Lc, &Lm);
   fprintf(fp_log, "c a c m              : %d %d %d\n", La, Lc, Lm);

   b = (int *) alloc(((unsigned) k+1) * sizeof(int));
   fprintf(fp_out, "clique vector      : ");
   fflush(fp_out);
   fprintf(fp_log, "c clique vector      :     clique sz     num cliques\n");
   fprintf(fp_log, "c                          ---------     -----------\n");
   for ( c = 2;  c <= k;  ++c )
   {
      fscanf(fp_in, "%d", (b + c));
      if (*(b + c) > 0)
         fprintf(fp_log, "c                          %7d       %7d\n", c, b[c]);
   }

   create_graph(G, gtype, n, e);
   LeightonGraph(G, n, Gseed, k, La, Lc, Lm, b);
   fprintf(fp_out, "Leighton's proof : ");
   fprintf(fp_log, "c Leighton's proof : ");
   if (LeightonProof(G, n, Gseed, k, La, Lc, Lm))
   {
      fprintf(fp_out, "%d coloring\n", k);
      fprintf(fp_log, "%d coloring\n", k);
   }
   else
   {
      fprintf(fp_out, "** FAILED **\n");
      fprintf(fp_log, "** FAILED **\n");
   }
   fflush(fp_out);
   free((char *) b);
}

void gen_Und_graph(fp_in, fp_out, fp_log, G)

   FILE  *fp_in;  /* Input file pointer -- opened to read */
   FILE  *fp_out; /* Output file pointer -- opened to write */
   FILE  *fp_log; /* Output file pointer -- opened to write */
   graph G;       /* graph variable -- allocated, but not created */
{
   int      gtype, n, e, complement;
   unsigned Gseed;
   double   d;

   fprintf(fp_out, "graph gen seed     : "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &Gseed);
   fprintf(fp_log, "c graph gen seed     : %d\n", Gseed); 

   fprintf(fp_out, "number of vertices : "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &n);
   fprintf(fp_log, "c number of vertices : %d\n", n); 

   fprintf(fp_out, "max number of edges: "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &e);
   fprintf(fp_log, "c max number of edges: %d\n", e); 

   fprintf(fp_out, "Euclidean distance : "); 
   fflush(fp_out);
   fscanf(fp_in, "%lf", &d);
   fprintf(fp_log, "c Euclidean distance : %1.6lf\n", d); 

   fprintf(fp_out, "complement? [0/1]  : "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &complement);
   if (complement) fprintf(fp_log, "c U(n,d) complement\n");

   fprintf(fp_out, "sparse(0)/dense(1) : ");
   fflush(fp_out);
   fscanf(fp_in, "%d", &gtype);
   fprintf(fp_log, "c data structure     : %s\n", (gtype)?("dense"):("sparse"));

   create_graph(G, gtype, n, e);
   Und(G, n, d, &Gseed, complement);
   fflush(fp_out);
}

void gen_graph(fp_in, fp_out, fp_log, G)

   FILE  *fp_in;  /* Input file pointer -- opened to read */
   FILE  *fp_out; /* Output file pointer -- opened to write */
   FILE  *fp_log; /* Output file pointer -- opened to write */
   graph G;       /* graph variable -- allocated, but not created */
{
   int     Gtype;

   fprintf(fp_out, "graph choices are  : 0. G(n,p)\n");
   fprintf(fp_out, "                     1. G(k,n,p)\n");
   fprintf(fp_out, "                     2. Leighton\n");
   fprintf(fp_out, "                     3. U(n,d)\n");
   fprintf(fp_out, "\n");
   fprintf(fp_out, "graph choice number: "); 
   fflush(fp_out);
   fscanf(fp_in, "%d", &Gtype);

   switch (Gtype)
   {
      case 0:
	 fprintf(fp_log, "c \nc G(n,p) graph\n");
         gen_Gnp_graph(fp_in, fp_out, fp_log, G);
	 break;
      case 1:
	 fprintf(fp_log, "c \nc G(k,n,p) graph\n");
         gen_Gknp_graph(fp_in, fp_out, fp_log, G);
	 break;
      case 2:
	 fprintf(fp_log, "c \nc Leighton graph\n");
         gen_Leighton_graph(fp_in, fp_out, fp_log, G);
	 break;
      case 3:
	 fprintf(fp_log, "c \nc U(n,d) graph\n");
         gen_Und_graph(fp_in, fp_out, fp_log, G);
	 break;
      default:
         fprintf(fp_out, "unknown\n");
	 exit(1);
   }

   fprintf(fp_log, "c\n");
   fprintf(fp_log, "c           Graph Stats\n");
   print_graph_stats(fp_log, G);
}
