{$case}
{$double}

(* FILE : MPG_gen.p *)             (* WRITTEN BY : Craig Morgenstern *)
 
program MPGgen( input, output );
 
(* COMMUNCIATE BASIC VOCABULARY BETWEEN PACKAGES *)
%include 'graph_globals.h';
%include 'memory_globals.h';
%include 'circuit_globals.h';
%include 'graph.h';
%include 'circuit.h';
%include 'urand.h';
%include 'generate_map.h';
 
label 999;

var method, circuitsz, s : integer;   (* generation parameters *)
    t : real;
    len, d, i, numedges, v, w : integer;
    numV : vertexname;
    degree : array[ 0..maxV ] of integer;
    reduce, success : boolean;
    inside, outside : 1..4;
 
begin
  InitializeCircuitPkg;  (* initialize circuit storage *)
  creategraph( maxV );   (* create maxV (75000) isolated vetices *)
  init_urand;

  writeln('MPG GENERATION');
  readln(circuitsz);
  writeln('initial circuit size     : ', circuitsz:1, ' vertices');
  readln(s);
  writeln('random number stream seed: ', s:1);
  writeln;
 
  readln(method);
  case method of
       0 : begin
         writeln(' GENERAL PLANAR GRAPH');
         writeln(' min degree >= 3');
         inside := 1;
         outside := 1;
         reduce := false;
       end;
       1 : begin
         writeln(' REDUCED GENERAL PLANAR GRAPH');
         writeln(' min degree >= 5');
         inside := 2;
         outside := 2;
         reduce := true;
       end;
       2 : begin
         writeln(' EXPANDED PLANAR GRAPH');
         writeln(' min degree >= 5');
         writeln(' connectivity >= 3');
         inside := 2;
         outside := 3;
         reduce := false;
       end;
       3 : begin
         writeln(' EXPANDED PLANAR GRAPH');
         writeln(' min degree >= 5');
         writeln(' connectivity >= 4');
         inside := 2;
         outside := 4;
         reduce := false;
       end;
     end;
 
  writeln;
  writeln('... generation begins');
  t := time;
  GenerateMap( circuitsz, maxV, numV, s, success, inside, outside,
             reduce, makeadjacent, removevertex, isadjacent,
             degreeof, firstadj, nextadj, swaplabels );
  t := time - t;
  writeln('... generation completed; generation time : ',
            t:1:2, ' cpu seconds');
  writeln('... triangulation success is ', success:1 );
  if not success then goto 999;
  writeln;
 
            (* PRINT GENERATION DATA *)
  writeln('... map analysis begins');
  writeln;
 
  writeln(' number of vertices : ', numV:1);
  for d := 0 to maxV do  degree[ d ] := 0;
  numedges := 0;
  for i := 1 to numV do
    begin
      d := degreeof( i );
      numedges := numedges + d;
      degree[ d ] := degree[ d ] + 1
    end;
  writeln(' number of edges : ', numedges div 2:1);
  writeln;
  writeln(' degree distribution :   degree   num of vertices ');
  writeln('                         ------   --------------- ');
  for d := 1 to numV do
     if degree[ d ] <> 0
        then writeln(d:29,'  ',degree[ d ]:15 );
  writeln('@');
 
  writeln(numV:1);
  for v := 1 to numV do
      begin
        w := firstadj(v);
        while w <> NILvertex do
           begin
             if ( w > v )
                then write(w:1, ' ');
                w := nextadj(v)
           end;
        writeln('-1');
      end;

999:
end.
