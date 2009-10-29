/* FILE : circuit.c 
 * written by craig morgenstern
 *
 * A package to support some needed operations on circuits of labeled nodes.
 */

# include <stdio.h>
# include <malloc.h>
# include "circuit.h"
# include "urand.h"
void exit();

static	void shrinkXref(C, c, s)

	circuit C;
	circuitptr c;
	int s;
{
	circuitptr p;

	if (C->Xref != NULL)
	{
	    p = *(C->Xref + s);
	    *(C->Xref + c->i) = p;
	    p->i = c->i;
	}
}

static	void expandXref(C, c, s)

	circuit C;
	circuitptr c;
	int s;
{
	if (C->Xref != NULL)
	{
	   if (s >= C->Xrefsz)
	   {
	       printf("*** Xrefsz too small for circuit\n");
	       exit(1);
	   }
	   *(C->Xref + s) = c;
	   c->i = s;
	}
}

circuitptr getXnode(C, i)

	circuit C;
	int i;
{
	return((C->Xref != NULL) ? (*(C->Xref + (i))) : (NULL));
}

# define DisposeCircuit(C) free((char *) (C))
# define DisposeCircuitNode(c) (c->r = F, F = c)

static	circuitptr F = NULL;
static	circuitptr NewCircuitNode()
{
	circuitptr p;

	if (F == NULL)
	{
	    if ((p = (circuitptr) malloc(sizeof(circuitnode))) == NULL)
	    {
	        printf("*** malloc failed in NewCircuitNode\n");
	        exit(1);
	    }
	}
	else
	{
	    p = F;
	    F = F->r;
	}
	return(p);
}

static	circuit NewCircuit(Xrefsz)

	int Xrefsz;
{
	circuit p;

	if ((p = (circuit) malloc(sizeof(circuithead))) == NULL)
	{
	    printf("*** malloc failed in NewCircuit\n");
	    exit(1);
	}

	p->Xref = NULL;
	p->Xrefsz = Xrefsz;
        p->size = 0;
	if (Xrefsz > 0)
	{
	    /* All routines in this pkg assume that Xref is large enough.
	     * No checking for overflow is done.  
	     */
	     
	    p->Xref =
		(circuitptr *) malloc((unsigned)(Xrefsz) * sizeof(circuitptr));
	    if (p->Xref == NULL)
	    {
		printf("*** malloc failed in NewCircuit\n");
		exit(1);
	    }
	}
	return(p);
}

void	DestroyCircuit(C)

	circuit C;
{
	circuitptr c;

	if (C->size > 0)
	{
            for (c = C->window->r;  c->r != C->window;  c = c->r)
	        DisposeCircuitNode(c->l);
	    DisposeCircuitNode(c);
	}
	if (C->Xref != NULL) free((char *) C->Xref);
        DisposeCircuit(C);
}

circuit	CreateCircuit(LowLabel, HighLabel, Xrefsz)

	int LowLabel, HighLabel, Xrefsz;
{
	int v;
	circuitptr c1, c2;
	circuit C;

        /* Create a nonempty circuit with labeled nodes */
	C = NewCircuit(Xrefsz);
        C->size = HighLabel - LowLabel + 1;
        C->window = NewCircuitNode();
        c1 = C->window;
	c1->v = LowLabel;
	expandXref(C,c1,0);
        for (v = 1;  v <= HighLabel - LowLabel;  ++v)
        {
	    c2 = NewCircuitNode();
	    expandXref(C,c2,v);
	    c2->v = v + LowLabel;
	    c2->l = c1;
	    c1->r = c2;
	    c1 = c2;
	}
	c1->r = C->window;
	C->window->l = c1;
	return(C);
}

/* --------------------------------------------------------------------------
 * ------------------------  ExpandCircuit  ---------------------------------
 * --------------------------------------------------------------------------
 * Expand the circuit edge ( v1,v2 ) in C to include a node labeled v; i.e.,
 * ( v1,v2 ) becomes ( v1,v ) and ( v,v2 ).  The circuit window is set to v.
 * For efficiency, set the C's window to v1 or v2 before calling this routine.
 */

void	ExpandCircuit(C, v, v1, v2)

	circuit C;
	int v, v1, v2;
{
	circuitptr c;
     
	c = C->window;
	if (c->v == v1  ||  c->v == v2)
	{
	    if (c->l->v == v1  ||  c->l->v == v2)
		c = c->l;
	}
	else
	{
	    /* have to search the circuit */
	    while (c->v != v1)  c = c->r;
	    if (c->l->v == v2)  c = c->l;
	}

        /* circuit edge to be expanded is (c->v, c->r->v ) */
        C->window = NewCircuitNode();
	expandXref(C, C->window, C->size);
	C->window->i = C->size;
	C->window->v = v;
	C->window->l = c;
	C->window->r = c->r;
	c->r->l = C->window;
	c->r = C->window;
        C->size += 1;
}

/* --------------------------------------------------------------------------
 * ------------------------  ShrinkCircuit  ---------------------------------
 * --------------------------------------------------------------------------
 * Remove the node labeled v from C.  The window is set to v's clockwise
 * neighbor.  For efficiency, set C's window to v before calling this routine.
 */
     
void	ShrinkCircuit(C, v)

	circuit C;
	int v;
{
	circuitptr c;
     
	c = C->window;
	/* look left and right before searching for circuit node w/ label v. */
	if (c->l->v == v)  c = c->l;
	if (c->r->v == v)  c = c->r;
	while (c->v != v)  c = c->r;
        C->size -= 1;
	shrinkXref(C, c, C->size);
	c->r->l = c->l;
	c->l->r = c->r;
	C->window = c->r;
        DisposeCircuitNode(c);
}

/* --------------------------------------------------------------------------
 * ------------------------  RotateRight  -----------------------------------
 * --------------------------------------------------------------------------
 * Return the label of the node that is n nodes from the current circuit window
 * in a clockwise direction.  The circuit window is set ro the node whose 
 * label is returned.  0 <= n <= circuit_size is assumed.  Walk the circuit
 * in the shortest way.
 */

int	RotateRight(C, n)

	circuit C;
	int n;
{
	int i;

	if (n <= C->size/2)
            for (i = 0;  i < n;  ++i)
                C->window = C->window->r;
        else 
	    for (i = C->size;  i > n;  --i)
                C->window = C->window->l;
	return(C->window->v);
}

/* --------------------------------------------------------------------------
 * ------------------------  NodeSplit  -------------------------------------
 * --------------------------------------------------------------------------
 * Split C into two circuits, C1 and C2.  C becomes an empty circuit.  C1
 * becomes the circuit containing all node from v1 to v2 inclusive in a
 * clockwise direction.  C2 becomes the circuit containing v2 to v1 inclusive
 * in a clockwise direction.  Note that both C1 and C2 will contain
 * nodes labeled v1 and v2 (i.e., nodes labeled v1 and v2 in C are "split",
 * and then joined by a circuit edge).  The window of C1 is set to the node
 * labeled v1 and the window of C2 is set to the node labeled v2.  It is
 * assumed that both are present in the circuit and v1 != v2 and they are
 * not neighbors.
 */

void	NodeSplit(C, newC1, newC2, v1, v2)

	circuit C, *newC1, *newC2;
	int v1, v2;
{
	circuit C1, C2;
	circuitptr c1, c2;
	int length; /* Num edges in C from v1 to v2 in a clockwise direction */

	/* Find the nodes on C that are to be split */
        for (c1 = C->window;  c1->v != v1 && c1->v != v2;  c1 = c1->r);
	if (c1->v == v1)
	    for (c2 = c1->r, length = 1;  c2->v != v2;  c2 = c2->r,  ++length);
        else /* c1->v = v2 */
	{
            c2 = c1;
	    for (c1 = c2->r, length = 1;  c1->v != v1;  c1 = c1->r,  ++length);
            length = C->size - length;
            /* The length of the path from v1 to v2 in the clockwise
             * direction is the circuit size minus the length of the path
             * from v2 to v1 in the clockwise direction.
             */
	}

        /* Now split C into C1 and C2.  Both C1 and C2 will have their own
	 * copies of v1 and v2.
	 */

	 C1 = NewCircuit(0);
	 C2 = NewCircuit(0);
         C1->window = NewCircuitNode();
         C2->window = NewCircuitNode();
         C1->window->v = v1;
         C2->window->v = v2;
         C1->window->r = c1->r;
         c1->r->l = C1->window;
         C2->window->r = c2->r;
         c2->r->l = C2->window;
         c2->r = C1->window;
         C1->window->l = c2;
         c1->r = C2->window;
         C2->window->l = c1;
         C1->size = length + 1;
         C2->size = C->size - length + 1;
	 DisposeCircuit(C);
	 *newC1 = C1;
	 *newC2 = C2;
}

/* --------------------------------------------------------------------------
 * --------------------------  RandomCircuitNode  ---------------------------
 * --------------------------------------------------------------------------
 * Randomly choose a node on C.  Set C's window to this node, and return the
 * vertex label contained in the node.
 */

int	RandomCircuitNode(C, seed)

	circuit C;
	unsigned *seed;
{
	int i, dist;
	circuitptr p;

	dist = drand(seed) * C->size;

	p = getXnode(C, dist);
	if (p != NULL)
	{
	    C->window = p;
	    return(p->v);
	}

	if (dist < C->size / 2)
	    for (i = 0;  i < dist;  ++i)  C->window = C->window->r;
	else
	    for (i = C->size;  i > dist;  --i)  C->window = C->window->l;
	return(C->window->v);
}
