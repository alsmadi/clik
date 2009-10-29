{$nomain}
{$case}
{$double}

(* FILE : triangulate1.p *)           (* written by Craig Morgenstern *)
     
(* Construct a random triangulation of a circuit.  Care is taken not to
   add edges already in existence.  It is assumed that the graph containing
   the circuit can be embedded in the plane so that (1) the region bounded by
   the circuit is empty and (2) no edges cross.  Note that these requirements
   are stronger than just requiring planarity.  If (2) cannot be satisfied
   without violating (1) ( as can happen if we only required planarity ), then
   the circuit can still be triangulated by breaking it up into the necessary
   subcircuits and calling this routine on each subcircuit.
*)
     
(* COMMUNICATE BASIC VOCABULARY BETWEEN PACKAGES *)
%include 'graph_globals.h';
%include 'memory_globals.h';
%include 'circuit_globals.h';
%include 'circuit.h';
%include 'urand.h';
%include 'triangulate1.h';
     
(* --------------------------------------------------------------------------
   ------------------------   Triangulate1  ---------------------------------
   --------------------------------------------------------------------------
*)
     
procedure Triangulate1{( var C           : circuit;
                         var s           : integer;
                             minimizedeg : boolean;
     
                         procedure makeadjacent( v1,v2 : vertexname );
                         function  isadjacent( v1,v2 : vertexname ) : boolean;
                         function  degreeof( v : vertexname ) : integer;     )};
     
const    maxC = 50;    (* Max number of stacked circuits.  There are V-2
                         triangles, but by stacking the larger circuit,
                         at most lg( V-2 ) circuits can be stacked at
                         any time -- same trick as in quicksort. *)
     
type    circuitSTK = array[ 1..maxC ] of circuit;
     
var     top : integer;
        S : circuitSTK;
     
        (* -----------------  circuit stack routines  ----------------------- *)
     
    procedure initializeSTK;
           begin
             top := 0
           end;
     
    function emptySTK : boolean;
       begin
         emptySTK := (top = 0)
       end;
     
    procedure push( var C : circuit );
       begin
         top := top + 1;
         (* No chance of overflow with maxC set to 50. *)
         MoveCircuit( S[ top ], C )
         (* C is now the empty circuit *)
       end;
     
    procedure pop( var C : circuit );
       begin
         (* Stack cannot be empty at time of call. *)
         MoveCircuit( C, S[ top ] );
         (* S[ top ] is now the empty circuit *)
         top := top - 1
       end;
     
    (* -----------------------  SplitCircuit1  -------------------------- *)
     
    procedure SplitCircuit1( var C, C1, C2 : circuit );
      (* Assumes that C has at least 4 nodes.  The circuit is empty on
         the inside.  Randomly choose two nonadjacent nodes in C
         ( they must exist ), and split C along these nodes
         into C1 and C2.
      *)
     
    var v1, v2, v1right : vertexname;
     
    begin
      (* randomly choose 1st circuit node label *)
      v1 := RandomCircuitNode( C, s );
      (* save v1's right ( clockwise ) neighbor in C *)
      v1right := RotateRight( C, 1 );
     
      (* randomly choose another circuit node label that is not adjacent
         to the circuit node labeled v1.  Note that the circuit window
         is one to the right of v1 here. *)
      v2 := RotateRight( C, 1 + trunc( urand(s) * ( CircuitSize(C)-3 ) ) );
      if isadjacent( v1,v2 )
             then begin
            (* v1 and v2 are already adjacent (on the outside of C)
               in the graph being constructed.  But by planarity,
               we can easily find two nodes in C that are not
               adjacent in the graph. *)
                    v1 := v1right;
                    v2 := RotateRight( C, 1 )
                    (* This edge cannot exist by planarity *)
                  end;
      makeadjacent( v1,v2 );
      NodeSplit( C, C1, C2, v1, v2 )
      (* C is now the empty circuit.  C1 and C2 are both circuits
         with 3 or more nodes. *)
    end;
     
     
    (* ---------------------  SplitCircuit2  ---------------------------- *)
     
    procedure SplitCircuit2( var C, C1, C2 : circuit );
    (* Same as SplitCitcuit1, except we choose a nonadjacent pair of nodes
       of minimal degree so as to minimize the number of vertices with
       degree < 5. *)
     
    label 11, 22;
     
    const floordeg = 2;    (* every vertex has degree of at least 2 *)
          maxdeg   = maxV; (* largest vertex degree possible *)
     
    var mindeg1, mindeg2, deg        : integer;
        v1, v2, start, rover : vertexname;
     
    begin
      (* Choose a random node on C from which to start a search for a
         vertex of minimal degree. *)
      start := RandomCircuitNode( C, s );
     
      (* Search for a vertex of minimal degree in a clockwise direction. *)
      mindeg1 := maxdeg;
      rover := start;
      repeat
        deg := degreeof( rover );
        if deg < mindeg1
           then begin
                  v1 := rover;
                  mindeg1 := deg;
                  if deg = floordeg      (* No smaller deg possible;     *)
                     then goto 11        (* quit the minimal deg search. *)
                end;
        rover := RotateRight( C, 1 )
      until rover = start;
     
    11:
      (* Choose a random vertex from which to start a search for a vertex
         of minimal degree not adjacent to v1. *)
      start := RandomCircuitNode( C, s );
     
      (* Search for a vertex of minimal degree in a clockwise direction. *)
      mindeg2 := maxdeg;
      rover := start;
      repeat
        deg := degreeof( rover );
        if deg < mindeg2
           then if rover <> v1 (* and then not isadjacent( v1, rover ) *)
                   then if not isadjacent( v1, rover )
                              then begin
                                     v2 := rover;
                                     mindeg2 := deg;
                                     if deg = mindeg1   (* No smaller deg
                                                           possible; *)
                                        then goto 22    (* quit searching. *)
                                   end;
        rover := RotateRight( C, 1 )
      until rover = start;
     
    22:
      if mindeg2 = maxdeg
         then begin
                (* v1 is already adjacent to all other circuit vertices. *)
     
                (* The following vertices cannot already be adjacent. We
                   could repeat the above computation from a different
                   starting point, but this case isn't likely to occur
                   except in small circuits where the following edge
                   would be likely to be chosen anyway.  *)
     
                (* Set C's window to the node labeled v1 *)
                while RotateRight( C, 1 ) <> v1 do (* null *);
                    v2 := RotateRight( C, 1 );
                v1 := RotateLeft( C, 2 )
              end;
     
      makeadjacent( v1,v2 );
      NodeSplit( C, C1, C2, v1, v2 )
    end;
     
    (* ---------------------  triangulate  ---------------------------- *)
     
    procedure triangulate( var C : circuit;
                           procedure SplitCircuit( var C, C1, C2 : circuit )
                         );
     
       label 99;
     
       var   C1, C2 : circuit;
     
       begin
             while true do
                   begin
                     SplitCircuit( C, C1, C2 );
                     (* C is now the empty circuit *)
     
                     (* always push the larger circuit for storage efficiency *)
                     if CircuitSize( C1 ) > 3
                        then if CircuitSize( C2 ) > 3
                                then if CircuitSize( C1 ) >= CircuitSize( C2 )
                                        then begin
                                               push( C1 );
                                               (* C1 is now the empty circuit *)
                                               MoveCircuit( C, C2 )
                                               (* C2 is now the empty circuit *)
                                             end
                                        else begin
                                               push( C2 );
                                               (* C2 is now the empty circuit *)
                                               MoveCircuit( C, C1 )
                                               (* C1 is now the empty circuit *)
                                             end
                                else begin
                                       MoveCircuit( C, C1 );
                                       (* C1 is now the empty circuit *)
                                       DestroyCircuit( C2 )
                                       (* C2 is now the empty circuit *)
                                     end
                         else if CircuitSize( C2 ) > 3
                                then begin
                                       MoveCircuit( C, C2 );
                                       (* C2 is now the empty circuit *)
                                       DestroyCircuit( C1 )
                                       (* C1 is now the empty circuit *)
                                     end
                     else begin
                            DestroyCircuit( C1 );
                            DestroyCircuit( C2 );
                            (* C1 and C2 are now empty circuits. *)
                            if not emptySTK
                               then pop( C )
                               else goto 99  (* done *)
                          end
               end;
    99:
       end;
     
(* -------------------------  MAIN -- Triangulate1  ------------------------- *)
     
begin
  if CircuitSize( C ) > 3
     then begin
            initializeSTK;
            if minimizedeg
               then triangulate( C, SplitCircuit2 )
               else triangulate( C, SplitCircuit1 )
          end
end;
