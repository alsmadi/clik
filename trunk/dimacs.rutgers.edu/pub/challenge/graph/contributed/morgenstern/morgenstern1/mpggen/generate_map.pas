{$nomain}
{$double}
{$case}

(* FILE : generate_map.p *)             (* WRITTEN BY : Craig Morgenstern *)
     
(* Generate a maximally planar graph by one of various methods.  The basic
   method proceeds as follows :
     
      (1) Construct an initial circuit, C, with numV nodes (labeled 1 .. numV).
      (2) Triangulate the "inside" of C.
      (3) Triangulate the "outside" of C.
      (4) Optionally, reduce the graph so that no vertices of degree < 5 exist.
*)
     
(* COMMUNCIATE BASIC VOCABULARY BETWEEN PACKAGES *)
%include 'graph_globals.h';
%include 'memory_globals.h';
%include 'circuit_globals.h';
     
(* CIRCUIT ROUTINES *)
%include 'circuit.h';
     
(* TRIANGULATION ROUTINES *)
%include 'triangulate1.h';
%include 'triangulate2.h';
     
(* REDUCTION ROUTINE *)
%include 'mpg5.h';
     
(* RANDOM NUMBER GENERATOR -- multi-streamed *)
%include 'urand.h';
     
(* --------------------------------------------------------------------------
   -------------------------  GenerateMap  ----------------------------------
   --------------------------------------------------------------------------
*)
%include 'generate_map.h';
     
procedure GenerateMap{(     initialCsize    : vertexname;
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
                        procedure swaplabels( v1, v2 : vertexname )          )};
     
   label 999;
     
   var   C : circuit;
         v1, v2 : integer;
     
   function FakeDegreeOf( v : vertexname ) : integer;
      var s : integer;
      begin
        if v <= initialCsize
           then begin
                  s := v;
                  FakeDegreeOf := degreeof( v ) + 1 + trunc( urand( s ) * 3 )
                end
           else FakeDegreeOf := degreeof( v )
      end;
     
   begin
     success := true;
     numV := initialCsize;
     CreateCircuit( C, 1, initialCsize );
     for v1 := 1 to numV - 1 do makeadjacent( v1, v1 + 1 );
     makeadjacent( 1, numV );
     
     case Inside of
       1: Triangulate1( C, randomseed, false,
                        makeadjacent, isadjacent, degreeof );
       2: Triangulate1( C, randomseed, true,
                        makeadjacent, isadjacent, degreeof );
       3: begin
            Triangulate2( C, numV + 1, maxLabel, false, randomseed, numV,
                          success, makeadjacent, isadjacent, degreeof );
            if not success then goto 999;
          end;
       4: begin
            Triangulate2( C, numV + 1, maxLabel, true, randomseed, numV,
                          success, makeadjacent, isadjacent, FakeDegreeOf );
            if not success then goto 999;
          end
     end;
     
     (* The original circuit has been destroyed *)
     CreateCircuit( C, 1, initialCsize );
     
     case Outside of
       1: Triangulate1( C, randomseed, false,
                        makeadjacent, isadjacent, degreeof );
       2: Triangulate1( C, randomseed, true,
                        makeadjacent, isadjacent, degreeof );
       3: begin
            Triangulate2( C, numV + 1, maxLabel, false, randomseed, numV,
                          success, makeadjacent, isadjacent, degreeof );
            if not success then goto 999;
          end;
       4: begin
            Triangulate2( C, numV + 1, maxLabel, true, randomseed, numV,
                          success, makeadjacent, isadjacent, degreeof );
            if not success then goto 999;
          end
     end;
     
     if ReduceMap
        then begin
               MPG5( numV, makeadjacent, removevertex, isadjacent,
                     degreeof, firstadj, nextadj, success          );
     
               (* make all non-isolated vertices contiguously labeled *)
               v1 := 1;
               v2 := numV;
               while degreeof( v2 ) = 0 do v2 := v2 - 1;
               while v1 <= v2 do
                  begin
                    if degreeof( v1 ) = 0
                       then begin
                              swaplabels( v1, v2 );
                              repeat
                                v2 := v2 - 1;
                              until degreeof( v2 ) <> 0
                            end;
                    v1 := v1 + 1
                  end;
               numV := v2
             end;
999:
   end;
