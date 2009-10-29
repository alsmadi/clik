(* FILE : memory.h *)                 (* WRITTEN BY : Craig Morgenstern *)
     
(*
  include "memory_globals.h";
     
    const maxaddress = ???;
          niladdress = -1;
     
    type  address = niladdress..maxaddress;  /* 0..maxaddress are legal */
     
    var   memory : array[ address ] of integer;
*)
     
     
(* --------------------------------------------------------------------------
   --------------------  InitializeMEMORY  ----------------------------------
   --------------------------------------------------------------------------
   This routine should be called exactly once and prior to using any of the
   other routines in this package.
*)
     
procedure InitializeMEMORY;
   external;
     
     
(* --------------------------------------------------------------------------
   ----------------------------  getsp  -------------------------------------
   -------------------------------------------------------------------------- *)
     
procedure  getsp( var addr : address;
                      size : integer );
   external;
     
     
(* --------------------------------------------------------------------------
   -----------------------------  freesp  -----------------------------------
   -------------------------------------------------------------------------- *)
     
procedure freesp( var addr : address );
   external;
     
     
(* --------------------------------------------------------------------------
   ---------------------------  extendsp  -----------------------------------
   -------------------------------------------------------------------------- *)
     
procedure extendsp( var addr    : address;
                        amt     : integer;
                    var success : boolean );
   external;
     
     
(* --------------------------------------------------------------------------
   ----------------------------  shrinksp  ----------------------------------
   -------------------------------------------------------------------------- *)
     
procedure shrinksp( addr : address;
                    amt  : integer );
   external;
