/* FILE : circuit.h 
 * written by craig morgenstern
 *
 * An incomplete package to support the operations on circuits of labeled 
 * nodes that are needed by the triangulation routines.
 */

typedef struct cnode {
     struct cnode *l, *r;
     int v, i;
} circuitnode, *circuitptr;

typedef struct chead {
     circuitptr window, *Xref;
     int size, Xrefsz;
} circuithead, *circuit;

/* --------------------------------------------------------------------------
 * ------------------------  CreateCircuit  ---------------------------------
 * --------------------------------------------------------------------------
 * Create a circuit of contiguously labeled nodes.  Assumes at least 3 nodes
 * are to be created.  The circuit window is set to the node
 * with label  "LowLabel".  When re-creating a circuit, i.e., reusing
 * a circuit variable, be sure to pass it to DestroyCircuit first.
 * 
 * If the circuit will not ever be split, then optionally create and maintain
 * a table to allow for fast random node access.  The parameter Xrefsz should
 * be set to the largest number of nodes that the circuit is expected to
 * contain.  Set Xrefsz to 0 if a table is not to be used.
 */
     
circuit CreateCircuit(/* int LowLabel, HighLabel, Xrefsz */);

/* --------------------------------------------------------------------------
 * ------------------------  DestroyCircuit  --------------------------------
 * --------------------------------------------------------------------------
 * Call C = DestroyCircuit(C) to dispose of the circuit storage for later use.
 * C becomes undefined and must be recreated before it can be used again.
 */

void DestroyCircuit(/* circuit C */);

/* --------------------------------------------------------------------------
 * ------------------------  CircuitSize  -----------------------------------
 * --------------------------------------------------------------------------
 * Return the number of nodes in the circuit.
 */

# define CircuitSize(C) (C -> size)

/* --------------------------------------------------------------------------
 * ------------------------  ExpandCircuit  ---------------------------------
 * --------------------------------------------------------------------------
 * Expand the circuit edge ( v1,v2 ) in C to include a node labeled v; i.e.,
 * ( v1,v2 ) becomes ( v1,v ) and ( v,v2 ).  The circuit window is set to v.
 * For efficiency, set the C's window to v1 before calling this routine.
 */

void ExpandCircuit(/* circuit C;  int v, v1, v2 */);

/* --------------------------------------------------------------------------
 * ------------------------  ShrinkCircuit  ---------------------------------
 * --------------------------------------------------------------------------
 * Remove the node labeled v from C.  The window is set to v's clockwise
 * neighbor.  For efficiency, set C's window to v before calling this routine.
 */

void ShrinkCircuit(/* circuit C; int v */);

/* --------------------------------------------------------------------------
 * --------------------------  MoveCircuit  ---------------------------------
 * --------------------------------------------------------------------------
 * Transfer the circuit from C2 to C1 (C2 should then be considered to be
 * undefined, and must be recreated again).
 */

# define MoveCircuit(C1,C2) C1 = C2

/* --------------------------------------------------------------------------
 * ------------------------  RotateRight  -----------------------------------
 * --------------------------------------------------------------------------
 * Return the label of the node that is n nodes from the current circuit window
 * in a clockwise direction.  The circuit window is set ro the node whose
 * label is * returned.  0 <= n <= circuit_size is assumed.  Walk the circuit
 * in the shortest way.
 */

int RotateRight(/* circuit C; int n */);

/* --------------------------------------------------------------------------
 * ------------------------  NodeSplit  -------------------------------------
 * --------------------------------------------------------------------------
 * Split C into two circuits, C1 and C2.  C becomes an undefined circuit.
 * C1 and C2 should be the addresses of undefined circuits on entry.
 * C1 becomes the circuit containing all nodes from v1 to v2 inclusive in a
 * clockwise direction.  C2 becomes the circuit containing v2 to v1 inclusive
 * in a clockwise direction.  Note that both C1 and C2 will both contain
 * nodes labeled v1 and v2 (i.e., nodes labeled v1 and v2 in C are "split",
 * and then joined by a circuit edge).  The window of C1 is set to the node
 * labeled v1 and the window of C2 is set to the node labeled v2.  It is
 * assumed that both are present in the circuit and v1 != v2 and they are
 * not neighbors.
 *
 * C1 and C2 will not contain Xreference tables even if C has one.
 */

void NodeSplit(/* circuit C, *C1, *C2;  int v1, v2 */);

/* --------------------------------------------------------------------------
 * --------------------------  RandomCircuitNode  ---------------------------
 * --------------------------------------------------------------------------
 * Randomly choose a node on C.  Set C's window to this node, and return the
 * vertex label contained in the node.
 */

int RandomCircuitNode(/* circuit C; unsigned *seed */);
