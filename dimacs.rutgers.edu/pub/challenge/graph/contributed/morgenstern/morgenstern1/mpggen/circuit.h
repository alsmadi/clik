(* FILE : circuit.h *)               (* written by Craig Morgenstern *)
     
(* A package to support operations on circuits of labeled nodes.  It is
   assumed that all the nodes belonging to a circuit are distinctly labeled.
   It is possible to specify a sequence of operations that results in a circuit
   which has duplicate node labels.  It is the user's responsibility to avoid
   such circuits.  It is further assumed that all node labels are of the
   type  vertexname  given in  MAP_globals.h  .
     
   The Pascal assignment operator ( := ) must not be used on circuits.  The
   user should call  CopyCircuit  if another distinct copy of a circuit is
   desired, and should call  MoveCircuit  to move a circuit from one variable
   to another.
*)
     
(*      include  "map_globals.h";
     
        const    maxV = ???;    /* Largest valid vertex label. */
                 NILvertex = 0; /* Flag to indicate end of adjacency list in
                                   calls to firstadj and nextadj. */
     
        type     vertexname = NILvertex .. maxV;  /* Only 1..maxV are valid
                                                     vertex labels. */
*)
     
(*	include  "memory_globals.h";
     
        const 	maxaddress = ???;
                niladdress = -1;
     
    	type  address = niladdress..maxaddress;
     
    	var   memory : array[ address ] of integer;
*)
     
(*      include  "circuit_globals.h";
     
    	const   maxC = ???;    /* Largest valid circuit label. */
        	nilC = 0;      /* Flag to indicate the "nil" circuit. */
     
    	type    circuitname = nilC .. maxC;
        	circuit = record
                	    window : circuitname;
                	    size   : integer;
                	    addr   : address
              		  end;
*)
     
(* --------------------------------------------------------------------------
   ------------------------  InitializeCircuitPkg  --------------------------
   --------------------------------------------------------------------------
   Call this routine exactly once before any of the routines in this package
   are used.
*)
     
procedure InitializeCircuitPkg;
   external;
     
(* --------------------------------------------------------------------------
   ------------------------  CreateCircuit  ---------------------------------
   --------------------------------------------------------------------------
   Create a circuit of contiguously labeled nodes.  If LowLabel > HighLabel,
   then an empty circuit is created.  The circuit window is set to the node
   with label  "LowLabel".  If the user is re-creating a circuit, i.e., reusing
   a circuit variable, it is his responsbility to call DestroyCircuit first.
*)
     
procedure CreateCircuit ( var C 		  : circuit;
                  	      LowLabel, HighLabel : vertexname );
   external;
     
(* --------------------------------------------------------------------------
   ------------------------  DestroyCircuit  --------------------------------
   --------------------------------------------------------------------------
   Make C an empty circuit and dispose of the circuit storage for later use.
   This routine should not be called with a circuit variable that has
   not be created, but it can be an empty circuit.
*)
     
procedure DestroyCircuit ( var C : circuit );
   external;
     
(* --------------------------------------------------------------------------
   ------------------------  CircuitSize  -----------------------------------
   --------------------------------------------------------------------------
   Return the number of nodes in the circuit.
*)
     
function CircuitSize ( var C : circuit ) : integer;
   external;
     
(* --------------------------------------------------------------------------
   ------------------------  ExpandCircuit  ---------------------------------
   --------------------------------------------------------------------------
   Expand the circuit edge ( v1,v2 ) in C to include a node labeled v; i.e.,
   ( v1,v2 ) becomes ( v1,v ) and ( v,v2 ).  The circuit window is set to v.
   For efficiency, set C's window to v1 before calling this routine.
*)
     
procedure ExpandCircuit ( var C         : circuit;
                  	      v, v1, v2 : vertexname );
   external;
     
(* --------------------------------------------------------------------------
   ------------------------  ShrinkCircuit  ---------------------------------
   --------------------------------------------------------------------------
   Remove the node labeled v from C.  The window is set to v's clockwise
   neighbor.  It is the user's responsibility to insure that after shrinking
   the circuit is still a meaningful object, i.e., has at least three edges.
   For efficiency, set C's window to v before calling this routine.
*)
     
procedure ShrinkCircuit ( var C : circuit;
                  	      v : vertexname );
   external;
     
(* --------------------------------------------------------------------------
   ------------------------  CopyCircuit  -----------------------------------
   --------------------------------------------------------------------------
   Copy C1 from C2 ( i.e., the "circuit assignment" operation : C1 <-- C2 ).
   The circuit window of C2 is left unchanged and C1's window is set to be the
   same as C2.  C1 should be an uncreated or empty, but if C1 was a valid
   circuit it should be destroyed with DestroyCircuit first.
*)
     
procedure CopyCircuit ( var C1, C2 : circuit );
   external;
     
(* --------------------------------------------------------------------------
   --------------------------  MoveCircuit  ---------------------------------
   --------------------------------------------------------------------------
   Transfer the circuit from C2 to C1 and make C1 empty.  Basically a
   relabelling operation.  Note: The user should never have two different
   circuit variables pointing to the same circuit, though, through use of
   CopyCircuit, he may have two distinct copies of the same circuit.  I.e.,
   the Pascal := operator should never be applied by the user to variables
   of type  circuit .
*)
     
procedure MoveCircuit ( var C1, C2 : circuit );
   external;
     
(* --------------------------------------------------------------------------
   ---------------------------  IdenticalCircuits  --------------------------
   --------------------------------------------------------------------------
   Are C1 and C2 identical?
*)
     
function IdenticalCircuits ( var C1, C2 : circuit ) : boolean;
   external;
     
(* --------------------------------------------------------------------------
   ------------------------  RotateRight  -----------------------------------
   --------------------------------------------------------------------------
   Return the label of the node that is n nodes from the current circuit window
   in a clockwise direction.  The circuit window is set the node whose label is
   returned.  0 <= n <= circuit_size is assumed.  Walk the circuit in the
   shortest way.
*)
     
function RotateRight ( var C : circuit;
               		   n : integer ) : vertexname ;
   external;
     
(* --------------------------------------------------------------------------
   ------------------------  RotateLeft  ------------------------------------
   --------------------------------------------------------------------------
   Return the label of the node that is n nodes from the current circuit window
   in a counterclockwise direction.  The circuit window is set the node whose
   label is returned.  0 <= n <= circuit_size is assumed.  Walk the circuit in
   the shortest way.
*)
     
function RotateLeft ( var C : circuit;
                          n : integer ) : vertexname;
   external;
     
(* --------------------------------------------------------------------------
   ------------------------  NodeSplit  -------------------------------------
   --------------------------------------------------------------------------
   Split C into two circuits, C1 and C2.  C becomes an empty circuit.  C1
   becomes the circuit containing all node from v1 to v2 inclusive in a
   clockwise direction.  C2 becomes the circuit containing v2 to v1 inclusive
   in a clockwise direction.  Note that both C1 and C2 will contain
   nodes labeled v1 and v2 (i.e., nodes labeled v1 and v2 in C are "split",
   and then joined by a circuit edge).  The window of C1 is set to the node
   labeled v1 and the window of C2 is set to the node labeled v2.  It is
   assumed that both are present in the circuit and v1 <> v2 and they are
   not neighbors.
*)
     
procedure NodeSplit ( var C, C1, C2 : circuit;
                          v1, v2    : vertexname );
   external;
     
(* --------------------------------------------------------------------------
   ------------------------  NodeJoin  --------------------------------------
   --------------------------------------------------------------------------
   The inverse operation of NodeSplit.  It is assumed that both C1 and C2 have
   circuit edges labeled ( v1,v2 ) and that v1 <> v2.  C's window is set v1.
*)
     
procedure NodeJoin ( var C1, C2, C : circuit;
                         v1, v2    : vertexname );
   external;
     
(*    include  "random.h";
*)
     
(* --------------------------------------------------------------------------
   --------------------------  RandomCircuitNode  ---------------------------
   --------------------------------------------------------------------------
   Randomly choose a node on C.  Set C's window to this node, and return the
   vertex label contained in the node. *)
     
function RandomCircuitNode( var C     : circuit;
                            var rseed : integer ) : vertexname;
   external;
