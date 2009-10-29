(* FILE : memory_globals.h *)         (* WRITTEN BY : Craig Morgenstern *)
     
     
const maxaddress = 262143;
      maxaddressplus1 = 262144;
      niladdress = -1;
     
type  address = niladdress..maxaddressplus1;
     
var   memory : array[ address ] of integer;
