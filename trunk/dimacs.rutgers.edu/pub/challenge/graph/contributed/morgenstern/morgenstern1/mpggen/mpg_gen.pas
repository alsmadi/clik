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
  creategraph( maxV );   (* create maxV isolated vetices *)
  init_urand;

  writeln('c MPG GENERATION');
  readln(circuitsz);
  writeln('c initial circuit size     : ', circuitsz:1, ' vertices');
  readln(s);
  writeln('c random number stream seed: ', s:1);
  writeln('c');
 
  readln(method);
  case method of
       0 : begin
         writeln('c  GENERAL PLANAR GRAPH');
         writeln('c  min degree >= 3');
         inside := 1;
         outside := 1;
         reduce := false;
       end;
       1 : begin
         writeln('c  REDUCED GENERAL PLANAR GRAPH');
         writeln('c  min degree >= 5');
         inside := 2;
         outside := 2;
         reduce := true;
       end;
       2 : begin
         writeln('c  EXPANDED PLANAR GRAPH');
         writeln('c  min degree >= 5');
         writeln('c  connectivity >= 3');
         inside := 2;
         outside := 3;
         reduce := false;
       end;
       3 : begin
         writeln('c  EXPANDED PLANAR GRAPH');
         writeln('c  min degree >= 5');
         writeln('c  connectivity >= 4');
         inside := 2;
         outside := 4;
         reduce := false;
       end;
     end;
  writeln('c');
 
  GenerateMap( circuitsz, maxV, numV, s, success, inside, outside,
             reduce, makeadjacent, removevertex, isadjacent,
             degreeof, firstadj, nextadj, swaplabels );
  if not success then 
     begin
       writeln('c triangulation success is ', success:1 );
       goto 999;
     end;
 
  writeln('c number of vertices : ', numV:1);
  for d := 0 to maxV do  degree[ d ] := 0;
  numedges := 0;
  for i := 1 to numV do
    begin
      d := degreeof( i );
      numedges := numedges + d;
      degree[ d ] := degree[ d ] + 1
    end;
  writeln('c number of edges : ', numedges div 2:1);
  writeln('c');
  writeln('c degree distribution :   degree   num of vertices ');
  writeln('c                         ------   --------------- ');
  for d := 1 to numV do
     if degree[ d ] <> 0
        then writeln('c ',d:29,'  ',degree[ d ]:15 );
 
  writeln('p col ', numV:1, ' ', numedges div 2:1);
  for v := 1 to numV do
      begin
        w := firstadj(v);
        while w <> NILvertex do
           begin
             if (w > v) then writeln('e ', v:1, ' ', w:1);
             w := nextadj(v)
           end
      end;

999:
end.
