(* FILE : mpg5.h *)                        (* WRITTEN BY : Craig Morgenstern *)
     
     
(* Call this procedure to reduce any maximally planar graph to a maximally
   planar graph with all vertices of degree 5 or greater.
*)
     
(* Non-maximally planar input graphs return garbage.  User is responsible for
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
     
procedure MPG5(    numV : integer;
                   (* Number of vertices of maximally planar graph on input. *)
     
                   procedure makeadjacent( v1, v2 : vertexname );
                   (* Make v1 and v2 adjacent, symmetrically, incrementing
                      degrees of both vertices.  This procedure will not be
                      called if v1 and v2 are already adjacent. *)
     
                   procedure removevertex( v : vertexname );
                   (* Remove the edges between v and all its neighbors,
                      symmetrically, decreasing the degrees of all the
                      neighbors by 1.  v will be reduced to degree 0. *)
     
                   function isadjacent( v1, v2 : vertexname ) : boolean;
                   (* Return whether two vertices are adjacent. *)
     
                   function degreeof( v : vertexname ) : integer;
                   (* Return the current degree of a vertex. *)
     
                   function firstadj( v : vertexname ) : vertexname;
                   (* This and the next function are used to scan down
                      logical adjacency lists.  These functions will be used
                      uninterruptedly, i.e., the adjacency list of a vertex
                      will be retrieved, and no calls to any of the above four
                      procedures and functions will be made until the entire
                      list is retrieved. *)
     
                   function nextadj( v : vertexname ) : vertexname;
                   (* When the adjacency list is exhausted this function
                      should return the constant  NILvertex  listed in the
                      file GRAPH_OPS_globals.h as a flag.  For vertices of
                      degree 0  firstadj  should return NILvertex. *)
     
                   var error : boolean
                   (* Error flag set if storage requirements are exceeded. *)
              );
   external;
