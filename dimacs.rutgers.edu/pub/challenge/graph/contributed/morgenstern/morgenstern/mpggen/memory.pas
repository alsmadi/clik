{$nomain}
{$case}
{$double}

(* FILE : memory.p *)                 (* WRITTEN BY : Craig Morgenstern *)
     
(* BINARY BUDDY MEMORY MANAGEMENT PACKAGE *)
     
(* Tag and block ptr scheme :
     
      word  0       For allocated blocks, this word contains an encoding of
            the status, allocated size and requested size as
     
                  ( 2 * power + 1 ) + 100 * requested_size.
     
            Note that the allocated size ( block size ) is 2 ** power
            and that this tag word is odd.
     
      word  0       For free blocks, this word contains an encoding of the
            status and block size as
     
                          2 * power.
     
            Note that the block size is 2 ** power and that this tag
            word is even.
     
      words 1 & 2   For free blocks only, these words are used as pointers to
            link lists of free blocks.
*)
     
%include 'memory_globals.h';
%include 'memory.h';
     
(* --------------------------------------------------------------------------
   ----------------------  INTERNAL DECLARATIONS  ---------------------------
   --------------------------------------------------------------------------
   These declarations and routines are not known to the user. *)
     
{$own}
const tagwd = 0;
      next  = 1;
      prev  = 2;
     
      maxpower   = 18;      (* lg(  maxaddress + 1 ) *)
      largepower = 19;      (* maxpower + 1 *)
      minpower   = 2;       (* smallest block is of size 4 *)
      littlepower= 1;       (* minpower - 1 *)
     
type  powers  = littlepower..largepower;
     
var   shelf  : array [ powers ] of address;
         (* Shelf[ i ] is the address of the 1st on a (doubly) linked list
        of available blocks of size 2**i. *)
     
      power  : array [ powers ] of integer;
     (* Hold powers of 2 so that they are only computed once. *)
     
     
      (* --------------------  removeblock  ------------------------ *)
     
procedure removeblock( addr  : address;
               index : powers  );
     
   (* Remove a free block from its shelf. *)
     
   var nextblk, prevblk : address;
     
   begin
     nextblk := memory[ addr + next ];
     prevblk := memory[ addr + prev ];
     if nextblk <> niladdress
    then memory[ nextblk + prev ] := prevblk;
     if prevblk <> niladdress
        then memory[ prevblk + next ] := nextblk
        else shelf[ index ] := nextblk
   end;
     
      (* --------------------  insertblock  ------------------------ *)
     
procedure insertblock( addr  : address;
               index : powers  );
     
   (* Insert a free block onto the proper shelf *)
     
   var firstblk : address;
     
   begin
     firstblk := shelf[ index ];
     memory[ addr + prev ] := niladdress;
     memory[ addr + next ] := firstblk;
     if firstblk <> niladdress
    then memory[ firstblk + prev ] := addr;
     shelf[ index ] := addr
   end;
     
     
(* --------------------------------------------------------------------------
   --------------------------  PACKAGE ROUTINES  ----------------------------
   -------------------------------------------------------------------------- *)
     
(* --------------------------------------------------------------------------
   --------------------  InitializeMEMORY  ----------------------------------
   --------------------------------------------------------------------------
   This routine should be called exactly once and prior to using any of the
   other routines in this package.
*)
     
procedure InitializeMEMORY;
     
   var i : integer;
     
   begin
     (* sentinals *)
     power[ largepower ] := minint;
     power[ littlepower ] := minint;
     
     shelf[ maxpower ] := 0;
     power[ maxpower ] := round( exp( maxpower * ln(2) ) );
     for i := maxpower - 1  downto  minpower do
    begin
      shelf[ i ] := niladdress;
      power[ i ] := power[ i + 1 ] div 2
    end;
     
     memory[ tagwd ] := 2 * maxpower;
     memory[ next ] := niladdress;
     memory[ prev ] := niladdress
   end;
     
(* --------------------------------------------------------------------------
   ----------------------------  getsp  -------------------------------------
   -------------------------------------------------------------------------- *)
     
procedure getsp{( var addr : address;
              size : integer )};
     
   label 99;  (* return *)
     
   var index, fitindex  : powers;
       currentsize      : integer;
       block, halfblock : address;
     
   begin
     addr := niladdress;
     size := size + 1;
     index := minpower;
     currentsize := power[ index ];
     
     while size > currentsize do
    begin
      index := index + 1;
          if index > maxpower
         then goto 99;
      currentsize := power[ index ]
    end;
     
     fitindex := index;
     while shelf[ index ] = niladdress do
    begin
      index := index + 1;
          if index > maxpower
         then goto 99
        end;
     
     block := shelf[ index ];
     removeblock( block, index );
     
     while index <> fitindex do
    begin
          index := index - 1;
      halfblock := block + power[ index ];
      memory[ halfblock + tagwd ] := 2 * index;
      insertblock( halfblock, index )
    end;
     
     memory[ block + tagwd ] :=  ( 2 * index + 1 ) + ( 100 * (size - 1) );
     addr := block + 1;
     
99:
   end;
     
(* --------------------------------------------------------------------------
   -----------------------------  freesp  -----------------------------------
   -------------------------------------------------------------------------- *)
     
procedure freesp{( var addr : address )};
     
   var merge : boolean;
       index : powers;
       size, newaddr, buddy : address;
     
   begin
     addr := addr - 1;
     index := ( ( memory[ addr + tagwd ] - 1 ) mod 100 ) div 2;
     memory[ addr + tagwd ] := 2 * index;
     size := power[ index ];
     merge := index < maxpower;
     
     while merge do
    begin
          if ( ( addr div size ) mod 2 ) = 0
         then begin
            buddy := addr + size;
            newaddr := addr
          end
         else begin
            buddy := addr - size;
            newaddr := buddy
          end;
     
      if not odd( memory[ buddy + tagwd ] )
         then if index = ( memory[ buddy + tagwd ] div 2 )
             then begin
                removeblock( buddy, index );
                index := index + 1;
                size := 2 * size;
                addr := newaddr;
                memory[ addr + tagwd ] := 2 * index;
                merge := index < maxpower
              end
             else merge := false
         else merge := false
    end;
     
     insertblock( addr, index );
     addr := niladdress
   end;
     
(* --------------------------------------------------------------------------
   ---------------------------  extendsp  -----------------------------------
   -------------------------------------------------------------------------- *)
     
procedure extendsp{( var addr    : address;
                 amt     : integer;
             var success : boolean )};
     
   label 99; (* return *)
     
   var   size, newsize, i : integer;
     index : powers;
     buddy, newblk : address;
     
   begin
     success := true;
     addr := addr - 1;
     size := memory[ addr + tagwd ] div 100;
     newsize := size + amt;
     index := ( memory[ addr + tagwd ] mod 100 ) div 2;
     
     (* Can the current block contain the expansion? *)
     if newsize < power[ index ]
    then begin
           memory[ addr + tagwd ] := memory[ addr + tagwd ] + amt * 100;
           addr := addr + 1;
           goto 99
         end;
     
     (* Is the buddy free, and if so, can the current block + its buddy
    contain the expansion? *)
     if newsize < power[ index + 1 ]
    then begin
               buddy := addr + power[ index ];
               if not odd( memory[ buddy + tagwd ] )
              then if memory[ buddy + tagwd ] div 2 = index
                  then begin
                         removeblock( buddy, index );
                         memory[ addr + tagwd ] := 2 * (index + 1) + 1
                                   + 100 * newsize;
                 addr := addr + 1;
                 goto 99
                   end
         end;
     
     (* Need to copy to a new larger block *)
     getsp( newblk, newsize );
     if newblk = niladdress
    then begin
           (* Can't expand *)
           success := false;
           goto 99
         end;
     addr := addr + 1;
     for i := 0 to size - 1 do
    memory[ newblk + i ] := memory[ addr + i ];
     freesp( addr );
     addr := newblk;
     
99:
   end;
     
(* --------------------------------------------------------------------------
   ----------------------------  shrinksp  ----------------------------------
   -------------------------------------------------------------------------- *)
     
procedure shrinksp{( addr : address;
             amt  : integer )};
     
   var index  : powers;
       size   : integer;
       newblk : address;
     
   begin
     addr := addr - 1;
     index := ( memory[ addr + tagwd ] mod 100 ) div 2;
     size := memory[ addr + tagwd ] div 100 - amt;
     
     (* Only return half the block when < 1/4 of the block is being used *)
     if index > minpower
    then while size < power[ index - 2 ] do
               begin
             index := index - 1;
             newblk := addr + power[ index ];
             memory[ newblk + tagwd ] := 2 * index;
             (* newblk's buddy is addr ==> no merging possible *)
             insertblock( newblk, index )
               end;
     
     memory[ addr + tagwd ] := ( 2 * index + 1 ) + ( 100 * size )
   end;
