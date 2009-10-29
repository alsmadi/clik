(* FILE : circuit_globals.h *)             (* written by Craig Morgenstern *)
     
(* see memory_globals.h *)
     
const   maxC = 75000;  (* Largest valid circuit label. *)
        nilC = 0;      (* Flag to indicate the "nil" circuit. *)
     
type    circuitname = nilC .. maxC;
    	circuit = record
            	    window : circuitname;
                    size   : integer;
                    addr   : address
                  end;
