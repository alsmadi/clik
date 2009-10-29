/*  FILE       : GraphOps.h
    WRITTEN BY : Craig Morgenstern
     
    Routines to support operations on a general graph; i.e., an undirected
    graph without loops or multiple edges.  Compile the program using these
    operations with  Dense.c  for operations on dense graphs, and with
    Sparse.c  for operations on sparse graphs.
     
    The following values are set in the file GraphGlobals.h  and are
    included in  Dense.c  and  Sparse.c.
     
    define  numV  ?   All vertex labels must be in the range 0 .. numV-1.
    define  NIL   -1  A non-label value (-1) denoting the "null" vertex.
*/
     
void    createvertices(/* n */);
       /* Initalizes the package so that vertices 0 .. n-1 are isolated.
          Returns 1 if the package can support n vertices and 0 otherwise. */
     
void   addedge(/* v1, v2 */);
       /* Takes two distinct vertex labels, v1 and v2, and creates an edge
          between them.  Assumes that the vertices are not already adjacent
          and that v1 <> v2.  */
     
void   removeedge(/* v1, v2 */);
       /* Takes two distinct vertex labels, v1 and v2, and removes the edge
          between them.  Assumes that the vertices are adjacent.  */
     
int    adjacent(/* v1, v2 */);
       /* Takes two distinct vertex labels, v1 and v2, and returns true (1)
          if the vertices are adjacent and false (0) otherwise.  Assumes
          that v1 <> v2.  */
     
int    degreeof(/* v */);
       /* Takes a vertex label, v, and returns the degree of v.  */
     
int    numedges();
       /* Returns the number of edges in the graph.  */
     
int    numvertices();
       /* Returns the number of nonisolated vertices in the graph.  */
     
int    highvertex();
       /* Returns the largest label amongst all nonisolated vertices */
     
void   removevertex(/* v */);
       /* Takes a vertex label, v, and makes v isolated.  */
     
void   allneighbors(/* v, *d, *nlist */);
       /* nlist is an integer array. The labels of all vertices adjacent to
          v are placed in nlist, starting at nlist[0].  Integer d is set
          to the number of neighbors of v. */
     
int    firstadj(/* v */);
int    nextadj(/* v */);
       /* Scan v's adjacency "list". NIL is returned when scan is complete.
          A call to firstadj restarts the scan and nextadj is repeatedly
          called to continue the scan.  At most one scan can be in progess
          for v at any time.  */
     
/* IN Sparse.c ONLY */
void    swaplabels(/* v1, v2 */);
       /* Swap adjacency lists */
