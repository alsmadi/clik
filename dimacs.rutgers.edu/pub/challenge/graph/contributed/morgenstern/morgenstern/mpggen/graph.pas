{$nomain}
{$double}
{$case}

(*  FILE : graph.pas  *)             (* written by CRAIG MORGENSTERN *)

%include 'halt.h';
%include 'graph_globals.h';
%include 'graph.h';

(*	This package contains routines to support operations on 
	sparse graphs.  A newly created graph is a collection isolated 
	vertices to which edges can be added and then, if desired, removed.
        Vertex labels are limited to a range between 1 and  maxV  inclusive. 
*)

{$own}
type	adjptr = ^adjrec;
	adjrec = record (* on adjacency list of vertex w *)
		   v : vertexname;  (* label of vertex adjacent to w *)
		   next : adjptr (* linked list of other vertices adj to w *)
		 end;
                                                
var	ADJ : array[ 1..maxV ] of adjptr; 
              (* ADJ contains the graphs (through its adjcency lists).  *)
        
	freeptr : adjptr;
              (* Save disposed adjrec's for later use *)
	
	scanp : adjptr;  (* scan an adjacency list *)
	scanv : vertexname;

           (* ------------------- GetAdjNode ---------------------------- *)

procedure GetAdjNode( var p : adjptr );
   begin
     if freeptr = nil
	then new( p )
	else begin
	       p := freeptr;
	       freeptr := freeptr^.next
	     end
   end;

           (* ------------------- DisposeAdjNode  ------------------------ *)

procedure DisposeAdjNode( p : adjptr );
   begin
     p^.next := freeptr;
     freeptr := p
   end;



(* --------------------------------------------------------------------------
   ----------------------  creategraph  -------------------------------------
   -------------------------------------------------------------------------- 
   Initialize the adjacency list data structure for vertices labeled
   1, ..., maxV  to be empty, i.e., create a collection of contiguously
   labeled isolated vertices.  Should only be called once, and prior to any 
   calls to other subroutines in this package.  *)

procedure creategraph{( numV : integer )};

   var v : vertexname;

   begin
     freeptr := nil;
     scanv := NILvertex;
     for v:= 1 to numV do ADJ[v] := nil
   end;


(* --------------------------------------------------------------------------
   ---------------------------  makeadjacent  -------------------------------
   --------------------------------------------------------------------------
   Add edge (v1,v2) to the graph *)

procedure makeadjacent{( v1, v2 : vertexname )};

   var p, q : adjptr;

   begin
     GetAdjNode( p );
     p^.next := ADJ[v1];
     p^.v := v2;
     ADJ[v1] := p;

     GetAdjNode( q );
     q^.next := ADJ[v2];
     q^.v := v1;
     ADJ[v2] := q;
   end;

(* --------------------------------------------------------------------------
   ---------------------------  isadjacent  ---------------------------------
   --------------------------------------------------------------------------
   Search v1's adjacency list for v2; return true if found. *)

function isadjacent{( v1, v2 : vertexname ) : boolean};

   label 99;
   var p : adjptr;

   begin
     isadjacent := false;
     p := ADJ[v1];
     while p <> nil do
       begin
	 if p^.v = v2
	    then begin
		   isadjacent := true;
		   goto 99
		 end;
	 p := p^.next
       end;
   99:
   end;

(* --------------------------------------------------------------------------
   ---------------------------  degreeof  -----------------------------------
   --------------------------------------------------------------------------
   Return the degree of vertex v. *)

function degreeof{( v : vertexname ) : integer};

   var d : integer;
       p : adjptr;

   begin
     d := 0;
     p := ADJ[v];
     while p <> nil do
       begin
	 d := d+1;
	 p := p^.next
       end;
     degreeof := d
   end;

(* --------------------------------------------------------------------------
   ---------------------------  removeedge  ---------------------------------
   --------------------------------------------------------------------------
   Remove v1 from the adjacency list of v2, and vice-versa. 
   The edge is assumed to exist. *)

procedure removeedge{( v1, v2 : vertexname )};

   var p, q : adjptr;

   begin
     p := ADJ[v1];
     if p^.v = v2
	then begin
	       ADJ[v1] := p^.next;
     	       DisposeAdjNode(p)
	     end
	else begin
	       while p^.next^.v <> v2 do p := p^.next;
               q := p^.next;
	       p^.next := q^.next;
     	       DisposeAdjNode(q)
	     end;

     p := ADJ[v2];
     if p^.v = v1
	then begin
	       ADJ[v2] := p^.next;
     	       DisposeAdjNode(p)
	     end
	else begin
	       while p^.next^.v <> v1 do p := p^.next;
               q := p^.next;
	       p^.next := q^.next;
     	       DisposeAdjNode(q)
	     end;
   end;

(* --------------------------------------------------------------------------
   ---------------------------  removevertex  -------------------------------
   --------------------------------------------------------------------------
   Remove v from the adjacency list of each of it's neighbors and set v's adj 
   list nil. *)

procedure removevertex{( v : vertexname )};

   begin
     while ADJ[v] <> nil do removeedge( v, ADJ[v]^.v )
   end;

(* ****************************************************************************
   *********************  firstadj  and  nextadj  *****************************
   ****************************************************************************
   NOTE: THIS AND THE NEXT PROCEDURE MUST BE CALLED WITHOUT ANY CHANGE TO THE
   GRAPH TO SCAN AN ADJACENCY LIST.  ONLY ONE SCAN CAN BE ACTIVE AT ANY TIME.

   Any change to the graph that involves v followed by a call to nextadj is an 
   error.  In this case, the scan must be restarted with a call to firstadj.  
*)

function firstadj{( v : vertexname ) : vertexname};

   begin
     scanv := v;
     scanp := ADJ[ v ];
     if scanp = nil
	then firstadj := NILvertex 
	else begin
	       firstadj := scanp^.v;
	       scanp := scanp^.next (* Move on in anticipation *)
	     end
   end;

function nextadj{( v : vertexname ) : vertexname};
   begin
     if v <> scanv
	then begin
	       writeln('Nested adjacency list scans');  exitst(1)
	     end;
     if scanp = nil
	then nextadj := NILvertex  
	else begin
	       nextadj := scanp^.v;
	       scanp := scanp^.next (* Move on in anticipation *)
	     end
   end;

(* --------------------------------------------------------------------------
   ---------------------------  swaplabels  ---------------------------------
   --------------------------------------------------------------------------
*)

procedure swaplabels{( v1, v2 : vertexname )};

   var p, q : adjptr;

   begin
     p := ADJ[ v1 ];
     while p <> nil do
	begin
	  q := ADJ[p^.v];
	  while q^.v <> v1 do q := q^.next;
	  q^.v := v2;
	  p := p^.next
	end;
     p := ADJ[ v2 ];
     while p <> nil do
	begin
	  q := ADJ[p^.v];
	  while q^.v <> v2 do q := q^.next;
	  q^.v := v1;
	  p := p^.next
	end;
     p := ADJ[ v2 ];
     ADJ[ v2 ] := ADJ[ v1 ];
     ADJ[ v1 ] := p;
   end;
