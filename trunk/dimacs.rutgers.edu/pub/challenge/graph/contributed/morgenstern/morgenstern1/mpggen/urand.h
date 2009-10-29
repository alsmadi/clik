(* FILE : urand.h
*  
*                       uniform random number generator
*
*       Reference: "Computer Methods for Mathematical Computations"
*                   by Forsythe, Malcolm and Moler
*
*)

procedure _init_urand; nonpascal;
procedure _urand( var y : integer ); nonpascal;

procedure init_urand; external;
function  urand( var y : integer ) : real; external;
