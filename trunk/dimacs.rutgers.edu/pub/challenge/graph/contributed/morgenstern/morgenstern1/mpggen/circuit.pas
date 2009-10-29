{$nomain}
{$double}
{$case}

(* FILE : circuit.p *)               (* written by Craig Morgenstern *)
     
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
     
%include 'halt.h';
%include 'graph_globals.h';
%include  'memory_globals.h';
%include 'circuit_globals.h';
%include 'memory.h'
%include 'circuit.h';
     

(* --------------------------------------------------------------------------
   --------------------------  INTERNAL DECLARATIONS  -----------------------
   --------------------------------------------------------------------------
   The following declarations are supposed to be local to this package and
   are not known to the user.  These are routines to efficiently manage the
   memory ( i.e., the circuitnode pool ).
*)
{$own}
type   circuitnode = record
                       l, r     : circuitname;
                       v        : vertexname;
                       randcell : address
                     end;
     
var    CN             : array [ circuitname ] of circuitnode;
       CP, pristineCP : circuitname;
     
     
            (* ----------------------------------------- *)
     
function NewCircuitNode : circuitname;
     
   begin
     if CP = nilC
        then begin
               if pristineCP > maxC
                  then begin
                         writeln('** ERROR in CIRCUIT.p - out of circuitnodes');
                         exitst(1)
                       end;
               NewCircuitNode := pristineCP;
               pristineCP := pristineCP + 1;
             end
        else begin
               NewCircuitNode := CP;
               CP := CN[ CP ].r
             end
   end; (* NewCircuitNode *)
     
            (* ----------------------------------------- *)
     
procedure DisposeCircuitNode( c : circuitname );
     
   begin
     CN[c].r := CP;
     CP := c
   end; (* DisposeCircuitNode *)
     
            (* ----------------------------------------- *)
     
procedure DisposeCircuit( var C : circuit );
     
   var c : circuitname;
     
   begin
     c := CN[ C.window ].r;
     CN[ C.window ].r := CP;
     CP := c
   end; (* DisposeCircuit *)
     
            (* ----------------------------------------- *)
     
     
(* --------------------------------------------------------------------------
   ----------------------  ADT CIRCUIT : visible routines  ------------------
   --------------------------------------------------------------------------
   The following routines are the user visible part of the package.
*)
     
(* --------------------------------------------------------------------------
   ------------------------  InitializeCircuitPkg  --------------------------
   --------------------------------------------------------------------------
   Call this routine exactly once before any of the routines in this package
   are used.
*)
     
procedure InitializeCircuitPkg;
     
   begin
     CP := nilC;
     pristineCP := 1;
     InitializeMEMORY
   end; (* InitializeCircuitPkg *)
     
(* --------------------------------------------------------------------------
   ------------------------  CreateCircuit  ---------------------------------
   --------------------------------------------------------------------------
   Create a circuit of contiguously labeled nodes.  If LowLabel > HighLabel,
   then an empty circuit is created.  The circuit window is set to the node
   with label  "LowLabel".  If the user is re-creating a circuit, i.e., reusing
   a circuit variable, it is his/her responsbility to call DestroyCircuit first.
*)
     
procedure CreateCircuit{( var C : circuit;
                  LowLabel, HighLabel : vertexname )};
     
   var v      : vertexname;
       c1, c2 : circuitname;
     
   begin
     if LowLabel <= HighLabel
        then begin
               (* Create a nonempty circuit with labeled nodes *)
               C.size := HighLabel - LowLabel + 1;
               C.window := NewCircuitNode;
               getsp( C.addr, C.size );
               if C.addr = niladdress
                  then begin
                         writeln('** ERROR : getsp failed in CreateCircuit');
                         exitst(1)
                       end;
     
               c1 := C.window;
               CN[ c1 ].v := LowLabel;
               CN[ c1 ].randcell := 0;
               memory[ C.addr ] := c1;
         
               for v := 1  to  HighLabel - LowLabel do
                  begin
                    c2 := NewCircuitNode;
                    CN[ c1 ].r := c2;
                    CN[ c2 ].v := v + LowLabel;
                    CN[ c2 ].l := c1;
                    CN[ c2 ].randcell := v;
                    memory[ C.addr + v ] := c2;
                    c1 := c2
                  end;
     
               CN[ c1 ].r := C.window;
               CN[ C.window ].l := c1
             end
        else begin
               C.window := nilC;
               C.size := 0;
               C.addr := niladdress
             end
   end; (* CreateCircuit *)
     
(* --------------------------------------------------------------------------
   ------------------------  DestroyCircuit  --------------------------------
   --------------------------------------------------------------------------
   Make C an empty circuit and dispose of the circuit storage for later use.
   This routine should not be called with a circuit variable that has
   not be created, but it can be an empty circuit.
*)
     
procedure DestroyCircuit{( var C : circuit )};
     
   begin
     if C.size <> 0
        then begin
               DisposeCircuit( C );
               C.window := nilC;
               C.size := 0;
               freesp( C.addr )
             end
   end; (* DestroyCircuit *)
     
(* --------------------------------------------------------------------------
   ------------------------  CircuitSize  -----------------------------------
   --------------------------------------------------------------------------
   Return the number of nodes in the circuit.
*)
     
function CircuitSize{( var C : circuit ) : integer};
     
   begin
     CircuitSize := C.size
   end; (* CircuitSize *)
     
(* --------------------------------------------------------------------------
   ------------------------  ExpandCircuit  ---------------------------------
   --------------------------------------------------------------------------
   Expand the circuit edge ( v1,v2 ) in C to include a node labeled v; i.e.,
   ( v1,v2 ) becomes ( v1,v ) and ( v,v2 ).  The circuit window is set to v.
   For efficiency, set the C's window to v1 before calling this routine.
*)
     
procedure ExpandCircuit{( var C         : circuit;
                              v, v1, v2 : vertexname )};
     
   var c : circuitname;
       success : boolean;
     
   begin
     c := C.window;
     while CN[ c ].v <> v1 do c := CN[ c ].r;
     if CN[ CN[ c ].l ].v = v2 then c := CN[ c ].l;
     
     (* circuit edge to be expanded is ( CN[ c ].v, CN[ CN[ c ].r ].v ) *)
     C.size := C.size + 1;
     C.window := NewCircuitNode;
     
     CN[ C.window ].v := v;
     CN[ C.window ].l := c;
     CN[ C.window ].r := CN[ c ].r;
     CN[ C.window ].randcell := C.size - 1;
     
     CN[ CN[ c ].r ].l := C.window;
     CN[ c ].r := C.window;
     extendsp( C.addr, 1, success );
     if not success
        then begin
               writeln('extendsp failed in ExpandCircuit');
               exitst(1)
             end;
     memory[ C.addr + CN[ C.window ].randcell ] := C.window
   end; (* ExpandCircuit *)
     
(* --------------------------------------------------------------------------
   ------------------------  ShrinkCircuit  ---------------------------------
   --------------------------------------------------------------------------
   Remove the node labeled v from C.  The window is set to v's clockwise
   neighbor.  It is the user's responsibility to insure that after shrinking
   the circuit is still a meaningful object, i.e., has at least three edges.
   For efficiency, set C's window to v before calling this routine.
*)
     
procedure ShrinkCircuit{( var C : circuit;
                              v : vertexname )};
     
   var c : circuitname;
     
   begin
     c := C.window;
     while CN[ c ].v <> v do c := CN[ c ].r;
     
     memory[ C.addr + CN[ c ].randcell ] := memory[ C.addr + C.size - 1 ];
     CN[ memory[ C.addr + C.size - 1 ] ].randcell := CN[ c ].randcell;
     shrinksp( C.addr, 1 );
     
     CN[ CN[ c ].r ].l := CN[ c ].l;
     CN[ CN[ c ].l ].r := CN[ c ].r;
     C.window := CN[ c ].r;
     DisposeCircuitNode( c );
     C.size := C.size - 1
   end; (* ShrinkCircuit *)
     
(* --------------------------------------------------------------------------
   ------------------------  CopyCircuit  -----------------------------------
   --------------------------------------------------------------------------
   Copy C1 from C2 ( i.e., the "circuit assignment" operation : C1 <-- C2 ).
   The circuit window of C2 is left unchanged and C1's window is set to be the
   same as C2.  C1 should be an uncreated or empty circuit. If C1 was a valid
   circuit it should be destroyed with DestroyCircuit first.
*)
     
procedure CopyCircuit{( var C1, C2 : circuit )};
     
   label 99;
   var   c1, c2, c3 : circuitname;
     
   begin
     if C2.size = 0
        then begin
               (* C2 is the empty circuit *)
               C1.window := nilC;
               C1.size := 0;
               C1.addr := niladdress;
               goto 99
             end;
     
     C1.size := C2.size;
     C1.window := NewCircuitNode;
     getsp( C1.addr, C1.size );
     if C1.addr = niladdress
        then begin
               writeln('** ERROR : getsp failed in CopyCircuit');
               exitst(1)
             end;
     
     c1 := C1.window;
     CN[ c1 ].v := CN[ C2.window ].v;
     CN[ c1 ].randcell := 0;
     memory[ C1.addr ] := c1;
     
     c3 := CN[ C2.window ].r;
     while c3 <> C2.window do
        begin
          c2 := NewCircuitNode;
          CN[ c2 ].v := CN[ c3 ].v;
          CN[ c2 ].randcell := CN[ c3 ].randcell;
          memory[ C1.addr + CN[ c2 ].randcell ] := c2;
          CN[ c2 ].l := c1;
          CN[ c1 ].r := c2;
          c1 := c2;
          c3 := CN[ c3 ].r
        end;
     
     CN[ c1 ].r := C1.window;
     CN[ C1.window ].l := c1;
99:
   end; (* CopyCircuit *)
     
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
     
procedure MoveCircuit{( var C1, C2 : circuit )};
     
   begin
     C1 := C2;
     C2.window := nilC;
     C2.size := 0;
     C2.addr := niladdress
   end; (* MoveCircuit *)
     
(* --------------------------------------------------------------------------
   ---------------------------  IdenticalCircuits  --------------------------
   --------------------------------------------------------------------------
   Are C1 and C2 identical?
*)
     
function IdenticalCircuits{( var C1, C2 : circuit ) : boolean };
     
   label 99;
   var   c1, c2 : circuitname;
     
   begin
     IdenticalCircuits := false;
     if C1.size <> C2.size then goto 99;   (* Can't be identical *)
     
     c1 := C1.window;
     c2 := C2.window;
     while CN[ c2 ].v <> CN[ c1 ].v do
        begin
          c2 := CN[ c2 ].r;
          if c2 = C2.window
             then goto 99 (* C2 has no node with the same label as C1^.window *)
        end;
     (* c1 has not moved, though maybe c2 has *)
     repeat
       if CN[ c1 ].v <> CN[ c2 ].v
         then goto 99   (* Nonidentical nodes in C1 and C2 *)
         else begin
                c1 := CN[ c1 ].r;
                c2 := CN[ c2 ].r
              end;
     until c1 = C1.window;
     IdenticalCircuits := true;
99:
   end; (* IdenticalCircuits *)
     
(* --------------------------------------------------------------------------
   ------------------------  RotateRight  -----------------------------------
   --------------------------------------------------------------------------
   Return the label of the node that is n nodes from the current circuit window
   in a clockwise direction.  The circuit window is set the node whose label is
   returned.  0 <= n <= circuit_size is assumed.  Walk the circuit in the
   shortest way.
*)
     
function RotateRight{( var C : circuit;
                           n : integer ) : vertexname };
     
   var i : integer;
     
   begin
     if n <= C.size div 2
        then for i := 1 to n do
                C.window := CN[ C.window ].r
        else for i := C.size - 1 downto n do
                C.window := CN[ C.window ].l;
     RotateRight := CN[ C.window ].v
   end; (* RotateRight *)
     
(* --------------------------------------------------------------------------
   ------------------------  RotateLeft  ------------------------------------
   --------------------------------------------------------------------------
   Return the label of the node that is n nodes from the current circuit window
   in a counterclockwise direction.  The circuit window is set the node whose
   label is returned.  0 <= n <= circuit_size is assumed.  Walk the circuit in
   the shortest way.
*)
     
function RotateLeft{( var C : circuit;
                          n : integer ) : vertexname };
     
   begin
     RotateLeft := RotateRight( C, C.size - n )
   end; (* RotateLeft *)
     
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
     
procedure NodeSplit{( var C, C1, C2 : circuit;
                          v1, v2    : vertexname )};
     
   var   c1, c2 : circuitname;
         i      : integer;
         length : integer;    (* Distance ( edge count ) between v1 and v2 in a
                                 clockwise direction ( right ) in C. *)
     
   begin
     (* Find the nodes on C that are to be split *)
     c1 := C.window;
     while ( CN[ c1 ].v <> v1 ) and ( CN[ c1 ].v <> v2 ) do c1 := CN[ c1 ].r;
     
     if CN[ c1 ].v = v1
        then begin
               c2 := CN[ c1 ].r;
               length := 1;
               (* Look for v2 *)
               while CN[ c2 ].v <> v2 do
                  begin
                    c2 := CN[ c2 ].r;
                    length := length + 1
                  end
             end
        else begin  (* oops -- CN[ c1 ].v = v2 *)
               c2 := c1;
               c1 := CN[ c2 ].r;
               length := 1;
               (* Look for v1 *)
               while CN[ c1 ].v <> v1 do
                 begin
                   c1 := CN[ c1 ].r;
                   length := length + 1
                 end;
               (* The length of the path from v1 to v2 in the clockwise
                  direction is the circuit size minus the length of the path
                  from v2 to v1 in the clockwise direction.
               *)
               length := C.size - length;
             end;
     
     (* Now split C into C1 and C2.  Both C1 and C2 will have their own copies
        of v1 and v2. *)
     C1.window := NewCircuitNode;
     C2.window := NewCircuitNode;
     CN[ C1.window ].v := v1;
     CN[ C2.window ].v := v2;
     
     CN[ C1.window ].r := CN[ c1 ].r;
     CN[ CN[ c1 ].r ].l := C1.window;
     
     CN[ C2.window ].r := CN[ c2 ].r;
     CN[ CN[ c2 ].r ].l := C2.window;
     
     CN[ c2 ].r := C1.window;
     CN[ C1.window ].l := c2;
     
     CN[ c1 ].r := C2.window;
     CN[ C2.window ].l := c1;
     
     C1.size := length + 1;
     C2.size := C.size - length + 1;
     
     C.window := nilC;
     C.size := 0;
     freesp( C.addr );
     
     getsp( C1.addr, C1.size );
     if C1.addr = niladdress
        then begin
               writeln('** ERROR : getsp failed in NodeSplit');
               exitst(1)
             end;
     c1 := C1.window;
     i := 0;
     repeat
       memory[ C1.addr + i ] := c1;
       CN[ c1 ].randcell := i;
       i := i + 1;
       c1 := CN[ c1 ].r
     until c1 = C1.window;
     
     getsp( C2.addr, C2.size );
     if C2.addr = niladdress
        then begin
               writeln('** ERROR : getsp failed in NodeSplit');
               exitst(1)
             end;
     c2 := C2.window;
     i := 0;
     repeat
       memory[ C2.addr + i ] := c2;
       CN[ c2 ].randcell := i;
       i := i + 1;
       c2 := CN[ c2 ].r
     until c2 = C2.window
   end; (* NodeSplit *)
     
(* --------------------------------------------------------------------------
   ------------------------  NodeJoin  --------------------------------------
   --------------------------------------------------------------------------
   The inverse operation of NodeSplit.  It is assumed that both C1 and C2 have
   circuit edges labeled ( v1,v2 ) and that v1 <> v2.  C's window is set v1.
*)
     
procedure NodeJoin{( var C1, C2, C : circuit;
                         v1, v2    : vertexname )};
     
   var c : circuitname;
       i : integer;
     
   begin
     C.size := C1.size + C2.size - 2;
     while CN[ C1.window ].v <> v1 do C1.window := CN[ C1.window ].r;
     while CN[ C2.window ].v <> v2 do C2.window := CN[ C2.window ].r;
     
     (* It is assumed that v1 and v2 are mirror images of each other in C1 and
        C2; i.e., v2 is clockwise of v1 in one circuit and is counterclockwise
        from v1 in the other circuit.  This is consistent with the view that
        NodeJoin is the inverse operation of NodeSplit.  *)
     if CN[ CN[ C1.window ].r ].v = v2
        then C.window := CN[ C2.window ].r
        else C.window := CN[ C2.window ].l;
     CN[ CN[ C1.window ].l ].r := CN[ C2.window ].r;
     CN[ CN[ C2.window ].r ].l := CN[ C1.window ].l;
     CN[ CN[ C2.window ].l ].r := CN[ C1.window ].r;
     CN[ CN[ C1.window ].r ].l := CN[ C2.window ].l;
     DisposeCircuitNode( C1.window );
     DisposeCircuitNode( C2.window );
     
     C1.window := nilC;
     C1.size := 0;
     freesp( C1.addr );
     
     C2.window := nilC;
     C2.size := 0;
     freesp( C2.addr );
     
     getsp( C.addr, C.size );
     if C.addr = niladdress
        then begin
               writeln('** ERROR : getsp failed in NodeJoin');
               exitst(1)
             end;
     c := C.window;
     i := 0;
     repeat
       memory[ C.addr + i ] := c;
       CN[ c ].randcell := i;
       i := i + 1;
       c := CN[ c ].r
     until c = C.window
   end; (* NodeJoin *)
     
(* --------------------------------------------------------------------------
   --------------------------  RandomCircuitNode  ---------------------------
   --------------------------------------------------------------------------
   Randomly choose a node on C.  Set C's window to this node, and return the
   vertex label contained in the node. *)
     
%include 'urand.h';
     
function RandomCircuitNode{( var C     : circuit;
                             var rseed : integer ) : vetexname };
     
   begin
     C.window := memory[ C.addr + trunc( urand( rseed ) * C.size ) ];
     RandomCircuitNode := CN[ C.window ].v
   end;
