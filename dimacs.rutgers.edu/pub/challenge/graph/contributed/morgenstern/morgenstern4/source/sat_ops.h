/* FILE       : sat_ops.h
   WRITTEN BY : Craig Morgenstern
     
   ASSUMES that the graph already exists and that the ClassOps package
       has been initialized.  Any change to the graph after calling
       create_sat will not be known in this package and can result
       in incorrect results.
     
   ASSUMES that all vertices are contiguously labeled 0 ... n-1.
*/
     
void    create_sat(/* n */);
int    	max_saturated_vertex();
void    sat_setcolor(/* v, c */);
void    sat_uncolor(/* v */);
int    	sat_inconflict(/* v, c */);
int    	sat_numAdjColors(/* v, c */);
