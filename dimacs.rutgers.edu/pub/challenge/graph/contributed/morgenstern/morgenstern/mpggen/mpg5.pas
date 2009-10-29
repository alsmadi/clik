{$nomain}
{$case}
{$double}

(*  FILE : mpg5.p  *)                     (* written by Craig Morgenstern *)
     
%include 'graph_globals.h';
%include 'mpg5.h';
     
(* ---------------------------------------------------------------------------
   -------------------------------  MPG5  ------------------------------------
   ---------------------------------------------------------------------------
   Call this procedure to reduce any maximally planar graph to a maximally
   planar graph with all vertices of degree 5 or greater.
     
   Non-maximally planar input graphs return garbage.  User is responsible for
   indirectly maintaining the data structure -- interface to package is
   provided by the following six functions and procedures.  On return, vertices
   that have been eliminated from the graph will have degree set to zero -- user
   can relabel the graph in his own data structure, as desired, after return.
   The label of a vertex cannot be changed by the user during the running of
   this procedure, i.e., relabeling cannot be done as a side effect in the six
   procedures and functions, unless, somehow the original labeling is maintained
   as well.
     
   All the procedures and functions should work correctly if passed a vertex of
   degree 0, i.e., an isolated vertex.
*)
     
     
procedure MPG5{    numV : integer;
                   /* Number of vertices of maximally planar graph on input. */
     
                   procedure makeadjacent( v1, v2 : vertexname );
                   /* Make v1 and v2 adjacent, symmetrically, incrementing
                      degrees of both vertices.  This procedure will not be
                      called if v1 and v2 are already adjacent. */
     
                   procedure removevertex( v : vertexname );
                   /* Remove the edges between v and all its neighbors,
                      symmetrically, decreasing the degrees of all the
                      neighbors by 1.  v will be reduced to degree 0. */
     
                   function isadjacent( v1, v2 : vertexname ) : boolean;
                   /* Return whether two vertices are adjacent. */
     
                   function degreeof( v : vertexname ) : integer;
                   /* Return the current degree of a vertex. */
     
                   function firstadj( v : vertexname ) : vertexname;
                   /* This and the next function are used to scan down
                      logical adjacency lists.  These functions will be used
                      uninterruptedly, i.e., the adjacency list of a vertex
                      will be retrieved, and no calls to any of the above four
                      procedures and functions will be made until the entire
                      list is retrieved. */
     
                   function nextadj( v : vertexname ) : vertexname;
                   /* When the adjacency list is exhausted this function
                      should return the constant  NILvertex  listed in the
                      file GRAPH_OPS_globals.h as a flag.  For vertices of
                      degree 0  firstadj  should return NILvertex. */
     
               var error : boolean
                   /* Error flag set if storage requirements are exceeded. */
              };
     
label     99;
     
const   MAXQ = 15000;  (* Size of internal queues maintained by MPG5 package.
                          If this size is exceeded an error indication is
                          returned.  Change this constant and recompile if this
                          occurs. *)
type    queue = record  (* Circular queue with pointers to head and tail *)
                  H, T : integer; (* H always points to one space before
                                     where the actual first element is.  T
                                     always points to the last element. *)
                  Q : array[ 0..MAXQ ] of vertexname
                end;
     
var     Q3,  (* queue containing vertices of degree 3 *)
        Q4   (* queue containing vertices of degree 3 or 4 *)
             : queue;
     
        v : vertexname;
     
 (* ------------------------  QUEUE ROUTINES  ------------------------------- *)
     
    procedure initializeQ( var QUEUE : queue );
           begin
             with QUEUE do
               begin
                 H := 0;
                 T := 0
               end
           end;
     
    function emptyQ( var QUEUE : queue ) : boolean;
       begin
         with QUEUE do
           emptyQ := (H = T)
       end;
     
    procedure enQ( var QUEUE : queue;
                           v : vertexname );
       begin
         with QUEUE do
           begin
             T := (T + 1) mod (MAXQ + 1);
             if T = H
                then begin
                       error := true;
                       goto 99 (* return *)
                     end
                else Q[T] := v
           end
       end;
     
    function deQ( var QUEUE : queue ) : vertexname;
       (* Will not be called when queue is empty. *)
       begin
         with QUEUE do
           begin
             H := (H + 1) mod (MAXQ + 1);
             deQ := Q[H]
           end
       end;
     
    procedure loadQs(     numV : integer;
                      var Q3, Q4 : queue  );
       (* Scan through all the vertices and find all that are initially of
          degree 3 and 4.
       *)
       var v : vertexname;
       begin
         initializeQ( Q3 );
         initializeQ( Q4 );
         (* The vertices are assumed numbered 1, 2, ... numV *)
         for v := 1 to numV do
           if degreeof( v ) = 3
              then enQ( Q3, v )
               else if degreeof( v ) = 4
                      then enQ( Q4, v )
                   (* else ignore the vertex at this time *)
       end;
     
(* -----------------------  DEG3REDUCTION  ---------------------------------- *)
     
    procedure deg3reduction( v : vertexname );
      (* Delete v from the graph and propagate the degree change of
         v's neighbors.
      *)
     
       var w : vertexname;
           newdeg : integer;
     
       begin
         w := firstadj( v );
         if w <> NILvertex
            then begin
                   repeat
                     newdeg := degreeof( w ) - 1;
                     if newdeg = 3 (* < 3 ==> already on Q3, and in fact
                                         graph will completely disappear *)
                        then enQ( Q3, w ) (* = 3 ==> now on Q3 and Q4. *)
                        else if newdeg = 4
                                then enQ( Q4, w );
                     w := nextadj( v )
                   until w = NILvertex;
                 removevertex( v )
               end
       end;
     
(* ---------------------------  DEG4REDUCTION  ----------------------------- *)
     
    procedure deg4reduction( v : vertexname );
      (* Delete v and connect two of v's nonadjacent neighbors;  propagate
         the degree change of the other two neighbors.
     
         In a maximally planar graph a vertex of degree 4 must be the
         center of a fully connected diamond.  One and possibly two pairs
         of vertices on the outside of the diamond are NOT adjacent.
         Delete the central vertex ( parameter v ), choose a pair with
         minimal degree sum and connect the pair vertices.
      *)
     
       label 10;
     
       type   neighborhood = array [ 1..4 ] of vertexname;
              degrees = array [ 1..4 ] of integer;
     
       var    neighbor : neighborhood;  (* vertices in v's neighborhood *)
              d : degrees; (* To avoid recomputation/function calls *)
              ii, i, j, jj, k, l, t : integer;
     
       begin
         neighbor[1] := firstadj( v );
         for i := 2 to 4 do                  (* load v's 1st neighborhood *)
               neighbor[i] := nextadj( v );
     
         (* find the first nonadjacent neighborhood vertex pair *)
         for ii := 1 to 3 do
           for jj := i+1 to 4 do
             if not isadjacent( neighbor[i], neighbor[j] )
               then begin
		      i := ii;  j := jj;
		      goto 10 (* if must become true *)
		    end;
     
     10: (* Find other pair no matter what -- we will need it one way or
            the other. *)
         case i of
           1: begin
                case j of
                  2: begin
                       k := 3;
                       l := 4
                     end;
                  3: begin
                       k := 2;
                       l := 4
                     end;
                  4: begin
                       k := 2;
                       l := 3
                     end
                end;
                (* both pairs can be non-adjacent *)
                if not isadjacent( neighbor[k], neighbor[l] )
                   then begin
                          for t := 1 to 4 do d[t] := degreeof( neighbor[t] );
                          if d[k] + d[l] < d[i] + d[j]
                             then begin (* swap the pairs *)
                                    t := i; i := k; k := t;
                                    t := j; j := l; l := t
                                  end
                        end
                   else begin
                          d[k] := degreeof( neighbor[k] );
                          d[l] := degreeof( neighbor[l] )
                        end
              end;
           2: begin
                case j of
                  3: begin
                       k := 1;
                       l := 4
                     end;
                  4: begin
                       k := 1;
                       l := 3
                     end
                end;
                d[k] := degreeof( neighbor[k] );
                d[l] := degreeof( neighbor[l] )
              end;
           3: begin
                k := 1;
                l := 2;
                d[k] := degreeof( neighbor[k] );
                d[l] := degreeof( neighbor[l] )
              end
         end; (* outer case *)
     
         (* Vertices neighbor[i] and neighbor[j] are to be made
            adjacent after the elimination of vertex v.
         *)
         (* The new degree of neighbor[k] *)
         if d[k] - 1 = 3 (* in this proc ==> can't be < 3 *)
            then enQ( Q3, neighbor[k] )  (* and it is also on Q4 *)
            else if d[k] - 1 = 4
                    then enQ( Q4, neighbor[k] );
         (* The new degree of neighbor[l] *)
         if d[l] - 1 = 3 (* in this proc ==> can't be < 3 *)
            then enQ( Q3, neighbor[l] )  (* and it is also on Q4 *)
            else if d[l] - 1 = 4
                    then enQ( Q4, neighbor[l] );
         removevertex( v );
         makeadjacent( neighbor[i], neighbor[j] )
       end;
     
     
begin
  (* invariant : the graph is maximally planar after every reduction *)
  loadQs( numV, Q3, Q4 );
  while true do
    if not emptyQ( Q3 )
       then begin
              v := deQ( Q3 );
              deg3reduction( v ) (* vertex can have degree less than 3 (graph
                                    will completely disappear shortly) and
                                    can even have degree 0 (if final vertex
                                    of totally reduced graph). *)
            end
       else if not emptyQ( Q4 )
               then begin
                      v := deQ( Q4 );
                      if degreeof(v) = 4 (* only other case is it has degree 0,
                                            since it moved to Q3 at one time and
                                            Q3 is empty at this time *)
                         then deg4reduction(v)
                    end
               else goto 99;  (* Reduction complete *)
99:
end;
