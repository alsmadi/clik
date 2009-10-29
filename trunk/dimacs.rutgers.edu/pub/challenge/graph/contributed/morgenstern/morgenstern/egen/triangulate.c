/* FILE: triangulate.c
 * written by craig morgenstern
 */

# include <stdio.h>
# include "graph.h"
# include "circuit.h"
# include "urand.h"
void exit();

# define Xrefsz_factor 1.5  /* Seems to be safe for the Triangulate2 algor */

/* ------------------------  Triangulate1  -----------------------------------
 * Construct a random triangulation of a circuit.  ASSUMES THAT NO EDGES
 * EXCEPT CIRCUIT EDGES EXIST.
 */

# define maxC 50 
		 /* Max number of stacked circuits.  There are V-2
		  * triangles, but by stacking the larger circuit,
		  * at most lg(V-2) circuits can be stacked at
		  * any time -- same trick as in quicksort. 
		  */

static circuit S[maxC];
static int top;

# define initializeSTK() top = -1
# define emptySTK() (top == -1)
# define push(C) MoveCircuit(S[++top], C) 
					  /* No chance of overflow */
					  /* C is now an undefined circuit */
# define pop(C) MoveCircuit(C, S[top--])
					  /* No check for underflow */
					  /* S[top] is now undefined */

static	void SplitCircuit(C, C1, C2, s)
/* Assumes that C has at least 4 nodes.  The circuit is empty on the inside. 
 * Randomly choose two nonadjacent nodes in C (they must exist), and split C
 * along these nodes into C1 and C2.  C1 and C2 are assumed to be undefined,
 * and C becomes undefined.
 */

	circuit C, *C1, *C2;
	unsigned *s;
{
	int v1, v2;
     
      	/* randomly choose 1st circuit node label */
      	v1 = RandomCircuitNode(C, s);
     
      	/* randomly choose another circuit node label that is not adjacent
         * to the circuit node labeled v1.
	 */
      	v2 = RotateRight(C, 2 + (int)(drand(s) * (CircuitSize(C)-3)));

	/* Add the edge */
        makeadjacent(v1,v2);

        /* C is now the empty circuit.  C1 and C2 are both circuits
         * with 3 or more nodes. 
	 */
        NodeSplit(C, C1, C2, v1, v2);
}

void	Triangulate1(N, seed)

	int N;
	unsigned *seed;
{
       	circuit C, C1, C2;

  	if (N <= 3) return;
	C = CreateCircuit(0, N-1, 0);
        initializeSTK();

        for (;;)
	{
            SplitCircuit(C, &C1, &C2, seed);
            /* C is now the empty circuit */
     
            /* always push the larger circuit for storage efficiency */
            if (CircuitSize(C1) > 3)
                if (CircuitSize(C2) > 3)
                    if (CircuitSize(C1) >= CircuitSize(C2))
                    {
                            push(C1);
                            /* C1 is now the empty circuit */
                            MoveCircuit(C, C2);
                            /* C2 is now the empty circuit */
                    }
		    else
		    {
                            push(C2);
                            /* C2 is now the empty circuit */
                            MoveCircuit(C, C1);
                            /* C1 is now the empty circuit */
                    }
                else
		{
                    MoveCircuit( C, C1 );
                    /* C1 is now the empty circuit */
                    DestroyCircuit(C2);
                    /* C2 is now the empty circuit */
                }
            else
		if (CircuitSize( C2 ) > 3)
		{
                    MoveCircuit(C, C2);
                    /* C2 is now the empty circuit */
                    DestroyCircuit(C1);
                    /* C1 is now the empty circuit */
		}
                else
		{
                    DestroyCircuit(C1);
                    DestroyCircuit(C2);
                    /* C1 and C2 are now empty circuits. */
                    if (!emptySTK())
                        pop(C);
                    else
			return;
                }
	}
}

/* -------------------------  Triangulate2  -----------------------------------
 * This routine is a graph generator that randomly triangulates a given circuit
 * of a planar graph so that the maximally planar subgraph embedded inside the
 * circuit contains no vertex of degree less than 5.  It is assumed that the
 * region "inside" the circuit is empty (contains no vertices or edges).
 * The motivation for the method can be found in:
 *   
 *   G. Marble and D.W. Matula, "Computational Aspects of 4-Coloring Planar
 *   Graphs", Research Report, University of Wisconsin, 1972.
 *   
 * The circuit is triangulated by adding additional vertices in such a way
 * that both the circuit vertices and the new vertices all have degrees
 * greater than or equal to 5.  We require that a number of contiguously
 * labeled isolated vertices be provided for the construction of the
 * triangulation.  Usage requires that the vertices 0..N-1 form a circuit
 * in the graph and that either the interior or the exterior of the circuit
 * be empty.  There is no guarranteed prediction on the size of the final 
 * graph as a function of the size of the initial circuit.
 */

void	Triangulate2(N, seed)

	int N;
	unsigned *seed;
{
	circuit C;
	int i, w, v1, v2;
     
	if (N < 4) return;
	C = CreateCircuit(0, N-1, (int)(N * Xrefsz_factor));
	for (;;)
        {
            if (pristineV >= maxV)
	    {
                /* Graph has grown too large. */
		printf("*** Out of vertex labels in Triangulate2\n");
		exit(1);
	    }
     
	    w = pristineV;
                /* Randomly choose adjacent nodes on the circuit. */
            v1 = RandomCircuitNode(C, seed);
	    v2 = RotateRight(C,1);
                /* Insert w into the circuit between v1 and v2. */
            makeadjacent(v1, w);
            makeadjacent(v2, w);
            ExpandCircuit(C, w, v1, v2);
		/* Move C's window back to v2 */
            v2 = RotateRight(C, 1);

            while (degreeof(v2) >= 5  &&  CircuitSize(C) > 5)
            {
		   /* Force circuit nodes of deg 5 or greater into interior as
		    * far as possible in a clockwise direction.
		    * Stop at circuit size 5 for special checking (below)
		    */
                v1 = v2;
                v2 = RotateRight(C, 1);
                makeadjacent(v2, w);
		   /* ShrinkCircuit moves C's window to the clockwise
		    * neighbor of v1; i.e., v2.
		    */
                ShrinkCircuit(C, v1);
            }

	    if (CircuitSize(C) == 5)
	    {
		    /* All nodes on the circuit must have deg >= 4 in order
		     * to finish.  Heuristic: Don't shrink C below 5 nodes
		     * until it is okay to finish.
		     */

		for (i = 0, v1 = RotateRight(C,1);
		     i < 5  &&  degreeof(v1) >= 4;
		     ++i, v1 = RotateRight(C,1));
		
		if (i == 5)
		{
		    /* Okay to finish triangulation with a deg 5 vertex. */
                    if (pristineV >= maxV)
	            {
                        /* Graph has grown too large. */
		        printf("*** Out of vertex labels in Triangulate2\n");
		        exit(1);
	            }

	            w = pristineV;
		    for (i = 0;  i < 5;  ++i, v1 = RotateRight(C,1))
			makeadjacent(v1,w);

		    /* Done */
     		    DestroyCircuit(C);
		    return;
		}
	    }
        }  
}
