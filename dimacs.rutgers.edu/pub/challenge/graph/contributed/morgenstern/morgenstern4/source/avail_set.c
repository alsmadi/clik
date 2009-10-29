/* FILE       : avail_set.c
   WRITTEN BY : Craig Morgenstern
*/
     
# 	include "graph_globals.h"
 
static 	unsigned char avail_set[numV];
static 	unsigned char mask[8] = { 1<<7, 1<<6, 1<<5, 1<<4, 1<<3, 1<<2, 1<<1, 1};


void	create_avail(v)

    	int	v;
{
    	avail_set[v] = ~0;
}
     
     
int	min_avail(v)

    	int    v;
{
    	int    c;

    	for ( c = 0;  !( avail_set[v] & mask[c] );  ++c );
    	return( c );
}
     
     
int	is_avail(v,c)

    	int	v, c;
{
    	return( avail_set[v] & mask[c] );
}
     
     
void    delete_avail(v,c)

        int    v, c;
{
    	avail_set[v] &= ~mask[c];
}
     
     
int	best_avail( v, k, d, nlist )
     
    	int	v, k, d, *nlist;
{
    	int	c, i, best, acnt[8];
     
    	for ( c = 0;  c < k;  ++c ) *(acnt + c) = 0;
        for ( i = 0;  i < d;  ++i )
             for ( c = 0;  c < k;  ++c )
                  if ( avail_set[*(nlist +i)] & mask[c] ) ++acnt[c];
        best = min_avail(v);
        for ( c = best+1;  c < k;  ++c )
             if ( (avail_set[v] & mask[c])  &&  acnt[c] < acnt[best] ) best = c;
        return( best );
}
