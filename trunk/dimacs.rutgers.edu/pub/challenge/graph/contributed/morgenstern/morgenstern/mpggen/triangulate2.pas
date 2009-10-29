{$nomain}
{$case}
{$double}

(* FILE : triangulate2.p *)               (* WRITTEN BY : Craig Morgenstern *)
     
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
     
   If the outside of the circuit is a chordless triangulation with no
   separating triangle, then the resulting graph can optionally be 4-connected.
*)
     
(* COMMUNICATE BASIC VOCABULARY BETWEEN PACKAGES *)
%include  'graph_globals.h';
%include  'memory_globals.h';
%include  'circuit_globals.h';
     
(* CIRCUIT ROUTINES *)
%include  'circuit.h';
     
(* --------------------------------------------------------------------------
   ------------------------  Triangulate2  ----------------------------------
   --------------------------------------------------------------------------
*)
%include  'triangulate2.h';
     
procedure Triangulate2{( var C : circuit;
                             LowIsolatedLabel, HighIsolatedLabel : vertexname;
                             connectivity4 : boolean;
                         var s : integer;
                         var HighNewLabel : vertexname;
                         var success : boolean;
     
                         procedure makeadjacent( v1, v2 : vertexname );
                         function  isadjacent( v1, v2 : vertexname ) : boolean;
                         function  degreeof( v : vertexname ) : integer   )};
     
   label 99, 999;
     
   var   v1, v2, v3, startv : vertexname;
         w : integer;
     
   begin
     w := LowIsolatedLabel - 1 ;
     success := true;
     while CircuitSize( C ) > 3 do
        begin
          w := w + 1;
          if w > HighIsolatedLabel
             then begin
                    (* Graph has grown too large. *)
                    success := false;
                    HighNewLabel := HighIsolatedLabel;
                    goto 999
                  end;
     
          (* Randomly choose a node on the circuit. *)
          v3 := RandomCircuitNode( C, s );
          (* v3 is the counterclockwise neighbor of startv, and is a
             "circuit wraparound" sentinal. *)
          startv := RotateRight( C, 1 );
          (* Get the label of the clockwise neighbor of startv. *)
          v2 := RotateRight( C, 1 );
          (* Set C's window back to startv -- to make ExpandCircuit efficient *)
          v1 := RotateLeft( C, 1 );
          (* Insert w into the circuit between v1 and v2. *)
          makeadjacent( v1, w );
          makeadjacent( v2, w );
          (* ExpandCircuit sets the circuit window to w *)
          ExpandCircuit( C, w, v1, v2 );
          (* Reset circuit window back to v2 *)
          v2 := RotateRight( C, 1 );
     
          while ( degreeof( v2 ) >= 5 ) and ( CircuitSize( C ) > 3 ) do
             begin
               v1 := v2;
               if CircuitSize( C ) = 5
                  then begin
                         repeat
                           if degreeof( v1 ) < 4
                              then goto 99;
                           v1 := RotateRight( C, 1 )
                         until v1 = v2;
                         w := w + 1;
                         if w > HighIsolatedLabel
                            then begin
                                   (* Graph has grown too large. *)
                                   success := false;
                                   HighNewLabel := HighIsolatedLabel;
                                   goto 999
                                 end;
                         repeat
                           makeadjacent( v1, w );
                           v1 := RotateRight( C, 1 )
                         until v1 = v2;
                         HighNewLabel := w;
                         goto 999
                       end;
     
               v2 := RotateRight( C, 1 );
     
               if connectivity4
                  then if isadjacent( v2, startv ) and ( v2 <> v3 )
                          then goto 99;  (* adding edge (v2,w) would create
                                            a separating triangle *)
     
               makeadjacent( v2, w );
               (* C's window <-- v1; make ShrinkCircuit efficient *)
               v1 := RotateLeft( C, 1 );
               (* ShrinkCircuit sets the circuit window to clockwise neighbor
                  of  v1  which is  v2  *)
               ShrinkCircuit( C, v1 )
             end;
99:
         end;
     HighNewLabel := w;
999:
     DestroyCircuit( C )
   end;
