/* FILE: sat_ops.c	WRITTEN BY: Craig Morgenstern */

# include <stdio.h>
# include <malloc.h>
# include "static_graph.h"
# include "color_ops.h"
# include "sat_ops.h"

# define never_colored 0
# define previously_colored 1

static int K, Kplus, D, Dplus; /* K = max colors, D = max vertex degree */

char *which_heap = NULL;
unsigned char *AdjColorCnt = NULL;
# define whichH(v) (*(which_heap + (v)))
# define ACcnt(v,c) (*(AdjColorCnt + (v)*K + (c)))

static int *Sweight = NULL;
# define putSweight(v,c,d) (*(Sweight+(v)) = (c)*Dplus + (d))
# define getSdeg(v) (*(Sweight+(v)) / Dplus)
# define getUdeg(v) (*(Sweight+(v)) % Dplus)

static int *prev_vertex = NULL, *next_vertex = NULL;
# define prev(v) (*(prev_vertex + (v)))
# define next(v) (*(next_vertex + (v)))

static struct {
	int NEHLP, *heap;
	unsigned char *NEHLC;
} S[2] = {0, NULL, NULL, 0, NULL, NULL};
# define NEHlist_ptr(w) ((S+w)->NEHLP)
# define H(w,c,d) (*((S+w)->heap + (c)*Dplus + (d)))
# define NEHlist_cnt(w,c) (*((S+w)->NEHLC + (c)))

# define initQ(w,c,d) H(w, c, d) = novertex
# define emptyQ(w,c,d) (H(w, c, d) == novertex)

static	void enQ(w, v, c, d)
	int w, v, c, d;
{
	int h;
	if (emptyQ(w, c, d))
	{
	   prev(v) = v;  next(v) = v;  H(w, c, d) = v;
	}
	else
	{
	   h = H(w, c, d);  prev(v) = prev(h);  next(v) = h;  
	   next(prev(v)) = v;  prev(h) = v;
	}
}

static	void rmQ(w, v, c, d)
	int  w, v, c, d;
{
	if (v == H(w, c, d)) H(w, c, d) = next(v);
	if (v != H(w, c, d))
	{
	   next(prev(v)) = next(v);  prev(next(v)) = prev(v);
	}
	else
	   H(w, c, d) = novertex;
}

/* ------------------------  create_sat  --------------------------------------
 */

int	create_sat()
{
        int n; /* number of vertices */
        int v; /* vertex label */
        int c; /* color label */
	int d; /* vertex degree */
     
	K = numcc;
	Kplus = K + 1;
	D = maxdeg;
	Dplus = D + 1;
	n = numvertices;

	if (Sweight != NULL) free((char *) Sweight);
	Sweight = (int *) malloc((unsigned)(n) * sizeof(int));
	if (Sweight == NULL) return(0);

	if (AdjColorCnt != NULL) free((char *) AdjColorCnt);
	AdjColorCnt = (unsigned char *) malloc((unsigned)(n*K));
	if (AdjColorCnt == NULL) return(0);

	if (which_heap != NULL) free(which_heap);
	which_heap = malloc((unsigned)(n));
	if (which_heap == NULL) return(0);

	if (prev_vertex != NULL) free((char *) prev_vertex);
	prev_vertex = (int *) malloc((unsigned)(n) * sizeof(int));
	if (prev_vertex == NULL) return(0);

	if (next_vertex != NULL) free((char *) next_vertex);
	next_vertex = (int *) malloc((unsigned)(n) * sizeof(int));
	if (next_vertex == NULL) return(0);

	if (S[0].heap != NULL) free((char *) S[0].heap);
	S[0].heap = (int *) malloc((unsigned)(Kplus*Dplus) * sizeof(int));
	if (S[0].heap == NULL) return(0);

	if (S[1].heap != NULL) free((char *) S[1].heap);
	S[1].heap = (int *) malloc((unsigned)(Kplus*Dplus) * sizeof(int));
	if (S[1].heap == NULL) return(0);

	if (S[0].NEHLC != NULL) free((char *) S[0].NEHLC);
	S[0].NEHLC = (unsigned char *) malloc((unsigned)(Kplus));
	if (S[0].NEHLC == NULL) return(0);

	if (S[1].NEHLC != NULL) free((char *) S[1].NEHLC);
	S[1].NEHLC = (unsigned char *) malloc((unsigned)(Kplus));
	if (S[1].NEHLC == NULL) return(0);

	NEHlist_ptr(never_colored) = 0;
	NEHlist_ptr(previously_colored) = -1;
	for (c = 0;  c < Kplus;  ++c)  
	{
	     NEHlist_cnt(never_colored,c) = 0;
	     NEHlist_cnt(previously_colored,c) = 0;
	     for ( d = 0;  d < Dplus;  ++d )
	     {
		initQ(never_colored, c, d);
		initQ(previously_colored, c, d);
	     }
	}

        for (v = 0;  v < n;  ++v) 
        {
	     whichH(v) = never_colored;
             d = degreeof(v);
	     putSweight(v, 0, d);
	     if (emptyQ(never_colored, 0, d)) ++NEHlist_cnt(never_colored,0);
	     enQ(never_colored, v, 0, d);
             for (c = 0;  c < K;  ++c) ACcnt(v, c) = 0;
        }
	return(1);
}

/* ----------------------  max_saturated_vertex  ------------------------------
 */

int     max_saturated_vertex()
{
        int wh, c, i, d;

	wh = previously_colored;
	for (c = NEHlist_ptr(wh); c >= 0 && NEHlist_cnt(wh,c) == 0;  --c);
	NEHlist_ptr(wh) = c;
	if (NEHlist_ptr(wh) == -1) 
	{
	     wh = never_colored;;
	     for (c = NEHlist_ptr(wh); c >= 0 && NEHlist_cnt(wh,c) == 0; --c);
	     NEHlist_ptr(wh) = c;
	     if (NEHlist_ptr(wh) == -1) return(novertex);
	}
	d = -1;  i = NEHlist_cnt(wh,c);
	do 
	{
	     ++d;
	     if (! emptyQ(wh, c, d))  --i;
	}
	while (i > 0);
	return(H(wh,c,d));
}

/* --------------------------  sat_makedirty  ---------------------------------
 */
     
void    sat_makedirty(v)
     
        int v;
{
	int Sdeg, Udeg;

	if (whichH(v) == never_colored)
	{
            Sdeg = getSdeg(v);  Udeg = getUdeg(v);
	    rmQ(never_colored, v, Sdeg, Udeg);
	    if (emptyQ(never_colored, Sdeg, Udeg)) 
		--NEHlist_cnt(never_colored, Sdeg);
	    whichH(v) = previously_colored;
	    if (emptyQ(previously_colored, Sdeg, Udeg)) 
		++NEHlist_cnt(previously_colored, Sdeg);
	    if (Sdeg > NEHlist_ptr(previously_colored))
		NEHlist_ptr(previously_colored) = Sdeg;
	    enQ(previously_colored, v, Sdeg, Udeg);
	}
}

/* --------------------------  sat_setcolor  ----------------------------------
 */
     
void    sat_setcolor(v, c)
     
        int v, c;
{
        int *p, *q, w, wh, Sdeg, Udeg, newSdeg, newUdeg;
 
	for (p = adjstart(v), q = adjstop(v);  p != q;  ++p)
        {
	     w = *p;
             /* maintain saturation and uncolored degrees for all vertices */
             Sdeg = getSdeg(w);  Udeg = getUdeg(w);
             newUdeg = Udeg-1;   newSdeg = Sdeg;
             if ( (++ACcnt(w,c)) == 1 ) ++newSdeg;
	     putSweight(w, newSdeg, newUdeg);
             if (colorof(w) == nocolor)  /* w is on a heap */
	     {
		  wh = whichH(w);
		  rmQ(wh, w, Sdeg, Udeg);
		  if (emptyQ(wh,Sdeg,Udeg)) --NEHlist_cnt(wh,Sdeg);
		  if (emptyQ(wh,newSdeg,newUdeg))
		  {
		      ++NEHlist_cnt(wh,newSdeg);
		      if (newSdeg > NEHlist_ptr(wh)) NEHlist_ptr(wh) = newSdeg;
		  }
		  enQ(wh, w, newSdeg, newUdeg);
	     }
        }

        /* Assumes v is in the heap (colorof(v) == nocolor) */
        setcolor(v,c);
        Sdeg = getSdeg(v);  Udeg = getUdeg(v);
	wh = whichH(v);
	rmQ(wh, v, Sdeg, Udeg);
	if (emptyQ(wh, Sdeg, Udeg)) --NEHlist_cnt(wh,Sdeg);
	whichH(v) = previously_colored;
}

/* --------------------------  sat_uncolor  -----------------------------------
 */
     
void    sat_uncolor(v)
     
        int v;
{
        int *p, *q, w, wh, c, Sdeg, Udeg, newSdeg, newUdeg;
     
        c = colorof(v);
	for (p = adjstart(v), q = adjstop(v);  p != q;  ++p)
	{
	     w = *p;
             /* maintain saturation and uncolored degrees for all vertices */
             Sdeg = getSdeg(w);  Udeg = getUdeg(w);
             newUdeg = Udeg+1;   newSdeg = Sdeg;
             if ((--ACcnt(w,c)) == 0) --newSdeg;
	     putSweight(w, newSdeg, newUdeg);
     
             if (colorof(w) == nocolor)  /* w is on a heap */
	     {
		 wh = whichH(w);
		 rmQ(wh, w, Sdeg, Udeg);
		 if (emptyQ(wh,Sdeg,Udeg)) --NEHlist_cnt(wh,Sdeg);
		 if (emptyQ(wh,newSdeg,newUdeg)) ++NEHlist_cnt(wh,newSdeg);
		 enQ(wh, w, newSdeg, newUdeg);
	     }
        }
     
        /* Assumes v is not in the heap (colorof(v) != nocolor) */
        uncolor(v);
	wh = previously_colored;
        Sdeg = getSdeg(v);  Udeg = getUdeg(v);
	if (emptyQ(wh, Sdeg, Udeg)) 
	{
	    ++NEHlist_cnt(wh,Sdeg);
	    if (Sdeg > NEHlist_ptr(wh)) NEHlist_ptr(wh) = Sdeg;
	}
	enQ(wh, v, Sdeg, Udeg);
}
 
/* ----------------------  sat_inconflict  ------------------------------------
 */

int	sat_inconflict1(v, c)
     
        int v, c;
{
	int *p, *q, w;

	if (ACcnt(v,c) > 0)  return(1);
	for (p = adjstart(v), q = adjstop(v);  p != q;  ++p)
	{
	     w = *p;
	     if (ACcnt(w,c) == 0 && colorof(w) == nocolor && getSdeg(w) >= K-1)
		 return(1);
	}
        return(0);
}

