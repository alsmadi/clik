/* FILE: subgraph.h	WRITTEN BY: Craig Morgenstern */

extern char *onqueue;
# define in_subgraph(v) (*(onqueue + (v)))

int initialize_subgraph(/* maxSGsz */);
void start_subgraph(/* impasse_v */);
void prep_subgraph();
int excise_subgraph(/* SizeIncr, Size */);
