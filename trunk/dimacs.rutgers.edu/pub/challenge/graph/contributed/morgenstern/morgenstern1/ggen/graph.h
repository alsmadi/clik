/*  FILE       : graph.h
 *  WRITTEN BY : Craig Morgenstern
 */

/* Supported graph types */
# define SPARSE 0
# define DENSE 1

typedef struct {
   int maxn, maxe, gtype;
   int n, e, h, *D;
   char *M;  /* adjmatrix for dense graphs */
   int *A, *adjnode, *next, pristine, free;  /* adjlists for sparse graphs */
} graph_struct, *graph;

typedef struct {
   graph G;
   int p, more;
   char *start, *next;
} neighborhood_scan_struct, *neighborhood_scan;

/* ----------------------------------------------------------------------------
 * ------------------------  new_graph  ---------------------------------------
 * ----------------------------------------------------------------------------
 * Allocate an uninitialized graph variable.
 */

graph new_graph();

/* ----------------------------------------------------------------------------
 * ------------------------  dispose_graph  -----------------------------------
 * ----------------------------------------------------------------------------
 * Deallocate a graph variable.
 */

void dispose_graph(/* G */);
/*
 *   graph G;
 */

/* ----------------------------------------------------------------------------
 * -----------------------  create_graph  -------------------------------------
 * ----------------------------------------------------------------------------
 * Initialize G so that:
 *    0) G can support a graph of the indicated type.
 *    1) G contains n isolated vertices indexed from 0...n-1.
 *    2) G can potentially contain a maximum of e edges (e arcs if the graph
 *       type is directed).
 */

void create_graph(/* G, gtype, n, e */);
/*
 *   graph G;
 *   int gtype, n, e;
 */

/* ----------------------------------------------------------------------------
 * ----------------------------  add_edge  ------------------------------------
 * ----------------------------------------------------------------------------
 * Add an edge between vertices in v1 and v2 in G.
 */

void add_edge(/* G, v1, v2 */);
/*
 *   graph G;
 *   int   v1, v2; (* labels of the vertices to be made adjacent *)
 */

/* ----------------------------------------------------------------------------
 * ----------------------------  remove_edge  ---------------------------------
 * ----------------------------------------------------------------------------
 * Remove the edge between v1 and v2 in G.
 */

void remove_edge(/* G, v1, v2 */);
/*
 *   graph G;
 *   int   v1, v2; (* endpoints of the edge *)
 */

/* ----------------------------------------------------------------------------
 * ----------------------  adjacent and edge  ---------------------------------
 * ----------------------------------------------------------------------------
 * Does there exist an edge between v1 and v2?  If so, return 1, else return 0.
 */

# define adjacent(G, v1, v2) ((G->gtype == DENSE) ? \
			      (*(G->M + G->maxn * v1 + v2)) : \
			      (edge(G, v1, v2)))

int edge(/* G, v1, v2 */);
/*
 *  graph G;
 *  int   v1, v2; (* vertex labels *)
 */

/* ----------------------------------------------------------------------------
 * ----------------------------  graph_type  ----------------------------------
 * ----------------------------------------------------------------------------
 */

#define graph_type(G) (G->gtype)

/* ----------------------------------------------------------------------------
 * -----------------------------  vdeg  ---------------------------------------
 * ----------------------------------------------------------------------------
 * Return the vertex degree of v in G.
 */

# define vdeg(G, v) (*(G->D + v))

/* ---------------------------------------------------------------------------
 * ---------------------------  num_vertices  --------------------------------
 * ---------------------------------------------------------------------------
 * Return the number of unisolated vertices in the graph.
 */

# define num_vertices(G) (G->n)
 
/* --------------------------------------------------------------------------
 * --------------------------  high_vertex  ---------------------------------
 * --------------------------------------------------------------------------
 * Return the highest label that is a non-isolated vertex.
 */

# define high_vertex(G) (G->h)

/* ---------------------------------------------------------------------------
 * ---------------------------  num_edges  -----------------------------------
 * ---------------------------------------------------------------------------
 * Return the number of edges in the graph.
 */

# define num_edges(G) (G->e)

/* ----------------------------------------------------------------------------
 * ----------------------------  remove_vertex  -------------------------------
 * ----------------------------------------------------------------------------
 * Make v1 an isolated vertex.
 */

void remove_vertex(/* G, v1 */);
/*
 *   graph G;
 *   int   v1;   (* vertex label -- make v1 isolated *)
 */

/* ----------------------------------------------------------------------------
 * ---------------------------  all_neighbors  --------------------------------
 * ----------------------------------------------------------------------------
 * Place the labels of all neighbors of v into the nlist array, starting at
 * nlist[0].
 */

void all_neighbors(/* G, v, d, nlist */);
/*
 *   graph G;
 *   int   v;        (* vertex label *)
 *   int   *d;       (* return number of neighbors; degree of v *)
 *   int   *nlist;   (* return the labels of all neighbors of v *)
 */

/* ----------------------------------------------------------------------------
 * -------------------  neighborhood_scan routines  ---------------------------
 * ----------------------------------------------------------------------------
 * Scan the (conceptual) adjacency list of v in G.
 */

neighborhood_scan new_neighborhood_scan();

void dispose_neighborhood_scan(/* N */);
/*
 *  neighborhood_scan N;
 */

void create_neighborhood_scan(/* G, N, v */);
/*
 *  graph G;
 *  neighborhood_scan N;
 *  int   v;
 */

int new_neighbor(/* N, w */);
/*
 *  neighborhood_scan N;
 *  int *w;
 */
