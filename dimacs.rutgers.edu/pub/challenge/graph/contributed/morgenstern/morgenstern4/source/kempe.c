/* FILE       : kempe.c
   WRITTEN BY : Craig Morgenstern
*/
     
#    include    "graph_globals.h"
#    include    "shuffle.h"
#    include    "urand.h"
     
typedef	int 	(*PFI)();
typedef void	(*PFV)();;
     
static	PFI    	firstadj, nextadj, colorof, inconflict;
static  PFV    	setcolor, uncolor;
static  int    	linklist[numV];
static	char	touched[numV];
static	int    	seed = 1;

static	void	Kchainsearch( c0, c1, v, csz )
	
	int	c0, c1, v, *csz;
{
	int	oldlink0, oldlink1, newlink0, newlink1, w, i;

	linklist[0] = v;  touched[v] = 1;  oldlink0 = -1;  newlink0 = 0;
	oldlink1 = numV;  newlink1 = numV;

	while ( oldlink0 < newlink0 )
	{
	     do
	     {
		  v = linklist[ ++oldlink0 ];
		  for ( w = (*firstadj)(v);  w != NIL;  w = (*nextadj)(v) )
		       if ( (*colorof)(w) == c1  &&  !touched[w] )
		       {
			    touched[w] = 1;  linklist[ --newlink1 ] = w;
		       }
	     }
	     while ( oldlink0 < newlink0 );

	     while ( oldlink1 > newlink1 )
	     {
		  v = linklist[ --oldlink1 ];
		  for ( w = (*firstadj)(v);  w != NIL;  w = (*nextadj)(v) )
		       if ( (*colorof)(w) == c0  &&  !touched[w] )
		       {
			    touched[w] = 1;  linklist[ ++newlink0 ] = w;
		       }
	     }
	}

	for ( i = 0;  i <= oldlink0;  ++i )
	{
	     v = linklist[i];  touched[v] = 0;
	     (*uncolor)(v);  (*setcolor)(v, c1);
	}

	for ( i = numV-1;  i >= oldlink1;  --i )
	{
	     v = linklist[i];  touched[v] = 0;
	     (*uncolor)(v);  (*setcolor)(v, c0);
	}

	*csz = oldlink0 + 1 + (numV - oldlink1);
}
     
int	Kempe( v, n, chi, maxIC, numchains, cs, cspcent,
               fadj, nadj, scolor, ucolor, color, incon )
     
        int    v, n, chi, maxIC, *numchains, *cs, *cspcent;
        PFI    fadj, nadj, color, incon;
        PFV    scolor, ucolor;
{
        int    cnl[maxD], cnc, i, w, c, cw, ccnt, csz;
         
        firstadj = fadj;  nextadj = nadj;  inconflict = incon;
        setcolor = scolor;  uncolor = ucolor;  colorof = color;
     
        ccnt = 0;
	for ( cnc = 0, w = firstadj(v);  w != NIL;  w = nextadj(v) )
	    if ( (*colorof)(w) != NIL )
		cnl[cnc++] = w;

	for (;;)
	{
	     shuffle( cnl, cnc, &seed, urand );
	     for ( i = 0;  i < cnc;  ++i )
	     {
		  w = cnl[i];
		  for ( c = 0;  c < chi;  ++c )
		  {
		       cw = (*colorof)(w);
		       if ( c != cw )
		       {
			    Kchainsearch( cw, c, w, &csz );

			    *(cs + ccnt) = csz;
			    *(cspcent) = 
			     (int)((((double) csz) / ((double) n)) * 1000.0);
			    ++ccnt;

                            if ( !(*inconflict)(v,cw) )
			    {
			         (*setcolor)(v,cw);
				 *numchains = ccnt;
				 return(1);
			    }
		            if ( ccnt >= maxIC )
			    {
				 *numchains = ccnt;
				 return(0);
		            }
		       }
		  }
	     }
	}
}
