/* FILE       : static_ops.h
   WRITTEN BY : Craig Morgenstern
     
   ASSUMES that the graph already exists and that the ClassOps package
       has been initialized.  Any change to the graph after calling
       create_static will not be known in this package and can result
       in incorrect results.
     
   ASSUMES that all vertices are contiguously labeled 0 ... n-1.
*/
     
void    create_static(/* n */);
void    static_setcolor(/* v, c */);
void    static_uncolor(/* v */);
int    	static_inconflict(/* v, c */);
int    	static_numAdjColors(/* v, c */);
