(* FILE : triangulate1.h *)           (* written by Craig Morgenstern *)
     
(* Construct a random triangulation of a circuit.  Care is taken not to
   add edges already in existence.  It is assumed that the graph containing
   the circuit can be embedded in the plane so that (1) the region bounded by
   the circuit is empty and (2) no edges cross.  Note that these requirements
   are stronger than just requiring planarity.  If (2) cannot be satisfied
   without violating (1) ( as can happen if we only required planarity ), then
   the circuit can still be triangulated by breaking it up into the necessary
   subcircuits and calling this routine on each subcircuit.
*)
     
(* COMMUNICATE BASIC VOCABULARY BETWEEN PACKAGES
     include "map_globals.h";
     include "circuit_globals.h";
     
   RANDOM NUMBER GENERATOR
     include "random.h";
     
   CIRCUIT PACKAGE
     include "circuit.h";
*)
     
     
(* --------------------------------------------------------------------------
   ------------------------   Triangulate1  ---------------------------------
   --------------------------------------------------------------------------
*)
procedure Triangulate1( var C           : circuit;
                        var s           : integer;
                            minimizedeg : boolean;
     
                        procedure makeadjacent( v1,v2 : vertexname );
                        function  isadjacent( v1,v2 : vertexname ) : boolean;
                        function  degreeof( v : vertexname ) : integer        );
   external;
