(* FILE : generate_map.h *)             (* WRITTEN BY : Craig Morgenstern *)
     
(* Generate a maximally planar graph by one of various methods.  The basic
   method proceeds as follows :
     
      (1) Construct an initial circuit, C, with numV nodes (labeled 1 .. numV).
      (2) Triangulate the "inside" of C.
      (3) Triangulate the "outside" of C.
      (4) Optionally, reduce the graph so that no vertices of degree < 5 exist.
     
   Steps 2 and 3 can each be done in one of 3 ways, giving 9 different ways to
   build a maximally planar graph.  One of the 3 methods adds vertices to the
   graph so that no vertex has degree < 5.  Thus, any combination using this
   method does not need to be reduced, because the other two methods only
   add edges.  Any combination not using this method can be reduced; and so
   this routine has a total of 13 methods possible.
*)
     
(* COMMUNCIATE BASIC VOCABULARY BETWEEN PACKAGES
     include "map_globals.h";
     include "circuit_globals.h";
     include "generate_map_globals.h";
     
   CIRCUIT ROUTINES
     include "circuit.h";
     
   TRIANGULATION ROUTINES
     include "triangulate1.h";
     include "triangulate2.h";
     
   REDUCTION ROUTINE
     include "mpg5.h";
*)
     
(* --------------------------------------------------------------------------
   -------------------------  GenerateMap  ----------------------------------
   --------------------------------------------------------------------------
*)
     
procedure GenerateMap (     initialCsize    : vertexname;
                            maxLabel        : vertexname;
                        var numV            : vertexname;
                        var randomseed      : integer;
                        var success         : boolean;
                            Inside, Outside : integer;
                            ReduceMap       : boolean;
     
                        procedure makeadjacent( v1, v2 : vertexname );
                        procedure removevertex( v : vertexname );
                        function  isadjacent( v1, v2 : vertexname ) : boolean;
                        function  degreeof( v : vertexname ) : integer;
                        function  firstadj( v : vertexname ) : vertexname;
                        function  nextadj ( v : vertexname ) : vertexname;
                        procedure swaplabels( v1, v2 : vertexname )         );
     
   external;
