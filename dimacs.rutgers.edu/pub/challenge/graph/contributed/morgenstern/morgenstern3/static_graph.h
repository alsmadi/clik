/* FILE: static_graph.h		WRITTEN BY: Craig Morgenstern */

extern int *Alist, *Ahead, numvertices, numedges, maxdeg;

# define novertex -1
# define degreeof(v) (*(Ahead + (v) + 1) - *(Ahead + (v)))
# define adjstart(v) (Alist + *(Ahead + (v)))
# define adjstop(v) (Alist + *(Ahead + (v) + 1))

/*
for (p = adjstart(v), q = adjstop(v);  p != q;  ++p)  w = *p;
*/

int read_static_graph(/* graph_file_name */);
void allneighbors(/* v, neighbor_array */);
int adjacent(/* v1, v2 */);
