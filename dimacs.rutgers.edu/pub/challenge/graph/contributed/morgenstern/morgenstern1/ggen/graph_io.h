/* FILE       : graph_io.h
 * WRITTEN BY : Craig Morgenstern
 */

/* ----------------------------  read_edges  ----------------------------------
 * Get graph edges from an external file.  Assumes that the file has already 
 * been opened to read.  Graphs are assumed to be in dimacs challenge format.
 */

void read_edges(/* FILE *fp; graph G; int e */);

/* ---------------------------  write_edges  ----------------------------------
 * Dump graph edges in dimacs format to a file already opened to write.
 */

void write_edges(/* FILE *fp; graph G */);

/* -----------------------  print_graph_stats  --------------------------------
 * Print a few graph stats to a file as dimacs comments.
 */

void print_graph_stats(/* FILE *fp; graph G */);

/* --------------------------  gen_graph  -------------------------------------
 * Invoke various generators, query to fp_out, read from fp_in, and
 * dump graph to fp_log.
 */

void gen_graph(/* FILE *fp_in, *fp_out, *fp_log; graph G */);

void gen_Gnp_graph(/* FILE *fp_in, *fp_out, *fp_log; graph G */);
void gen_Gknp_graph(/* FILE *fp_in, *fp_out, *fp_log; graph G */);
void gen_Leighton_graph(/* FILE *fp_in, *fp_out, *fp_log; graph G */);
void gen_Und_graph(/* FILE *fp_in, *fp_out, *fp_log; graph G */);
