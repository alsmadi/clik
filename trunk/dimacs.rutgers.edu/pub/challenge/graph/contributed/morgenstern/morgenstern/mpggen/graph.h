(*  FILE : GRAPH.h  *)
 
(*    This package contains routines to support operations on
    sparse graphs.  A newly created graph is a collection isolated
    vertices to which edges can be added and then, if desired, removed.
*)
 
(*
Globals in "GRAPH_globals.h"
 
const    maxV = 75000;  /* largest valid vertex label */
        NILvertex = 0; /* Flag to indicate end of adjacency list in calls to
                          firstadj and nextadj. */
 
type    vertexname = NILvertex .. maxV;  /* Only 1..maxV are valid vertex
                        labels.  NILvertex indicates the
                        absence of a vertex.  */
*)
 
procedure creategraph( numV : vertexname );  external;
           (* Create isolated vertices contiguously labeled  1..numV.
              Should only be called once, and prior to any calls to other
              subroutines in this package.  Initializes memory management. *)
 
procedure makeadjacent( v1, v2 : vertexname ); external;
           (* v1 and v2 are assumed to be nonadjacent.  Add an edge connecting
              v1 and v2.  *)
 
function isadjacent( v1, v2 : vertexname ) : boolean;  external;
           (* Return true if v1 and v2 are adjacent. *)
 
function degreeof( v : vertexname ) : integer;  external;
           (* Return the degree of v. *)
 
procedure removeedge( v1, v2 : vertexname );  external;
           (* v1 and v2 are assumed to be adjacent.  Remove the edge connecting
              v1 and v2.  *)
 
procedure removevertex( v : vertexname );  external;
           (* Remove all edges containing v as an endpoint. v becomes an
              isolated vertex.  *)
 
function firstadj( v : vertexname ) : vertexname;  external;
function nextadj ( v : vertexname ) : vertexname;  external;
           (* Scan the adjacency list belonging to v.
 
          Initiate the scan by a call to firstadj, and continue
          the scan by calls to nextadj.  The scan is complete when
          NILvertex is returned.  Any change to the graph that involves
          v, followed by a call to nextadj, is an error.  In this case,
          the scan must be restarted with a call to firstadj.  *)
 
procedure swaplabels( v1, v2 : vertexname );  external;
       (* Structurally  swap v1 and v2; i.e., make v1 adjacent to
          only those vertices adjacent to v2 and vice-versa.  *)
