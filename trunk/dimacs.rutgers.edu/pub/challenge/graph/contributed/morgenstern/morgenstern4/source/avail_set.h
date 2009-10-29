/* FILE       : avail_set.h
   WRITTEN BY : Craig Morgenstern
*/
     
void	create_avail(/* v */);
int     min_avail(/* v */);
int     is_avail(/* v, c */);
void    delete_avail(/* v, c */);
int     best_avail(/* v, k, d, nlist[] */);
