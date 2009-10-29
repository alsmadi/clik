(*  FILE : graph_globals.pas *)             (* WRITTEN BY : Craig Morgenstern *)

const   maxV = 150000;  (* largest valid vertex label *)
        NILvertex = 0; (* Flag to indicate end of adjacency list in calls to
                           firstadj and nextadj. *)
        
type    vertexname = NILvertex .. maxV;
           (* Only 1..maxV are valid vertex labels.  NILvertex indicates the
              absence of a vertex.  *)
     
    	vertexarray = array[ 0 .. maxV ] of vertexname;
       	   (* Indexed by integer or vertexname -- should really be
              two distinct types.  *)
