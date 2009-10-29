(* FILE : triangulate2.h *)               (* WRITTEN BY : Craig Morgenstern *)
     
(* This routine is a graph generator that randomly triangulates a given circuit
   of a planar graph so that the maximally planar subgraph embedded inside the
   circuit contains no vertex of degree less than 5.  It is assumed that the
   region "inside" the circuit is empty ( contains no vertices or edges ).
   The motivation for the method can be found in the following reference:
     
     G. Marble and D.W. Matula, "Computational Aspects of 4-Coloring Planar
     Graphs", Research Report, University of Wisconsin, 1972.
     
   The circuit is triangulated by adding additional vertices in such a way
   that both the circuit vertices and the new vertices all have degrees
   greater than or equal to 5.  We require that a number of contiguously
   labeled isolated vertices be provided for the construction of the
   triangulation.  Typical usage is for the caller to make vertices 1 .. N
   into a circuit and then to let the algorithm use vertices N+1 .. "infinity"
   to complete the triangulation.  There is no advance prediction on the
   size of the final graph as a function of the size of the initial circuit.
     
   The resulting graph is optionally 3 connected or 4 connected.
*)
     
(* COMMUNICATE BASIC VOCABULARY BETWEEN PACKAGES
  include  "map_globals.h";
  include  "memory_globals.h";
  include  "circuit_globals.h";
*)
     
(* CIRCUIT ROUTINES
  include  "circuit.h";
*)
     
(* --------------------------------------------------------------------------
   ------------------------  Triangulate2  ----------------------------------
   --------------------------------------------------------------------------
*)
     
procedure Triangulate2 ( var C : circuit;
                             LowIsolatedLabel, HighIsolatedLabel : vertexname;
                             connectivity4 : boolean;
                         var s : integer;
                         var HighNewLabel : vertexname;
                         var success : boolean;
     
                         procedure makeadjacent( v1, v2 : vertexname );
                         function  isadjacent( v1, v2 : vertexname ) : boolean;
                         function  degreeof( v : vertexname ) : integer   );
     
   external;
