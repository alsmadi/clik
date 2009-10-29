program p_hat_generator (input, output);

(*  Random graph generator that generates graph instances according to	   *)
(*  the P_HAT generation scheme as described in:			   *)
(*    Gendreau, M., P. Soriano and L. Salvail, "Solving the maximum clique *)
(*    problem using a tabu search approach", Annals of Operations Research *)
(*    41, 385-403							   *)
(*									   *)
(*  Author: Patrick Soriano						   *)
(*  Date: 29-07-1993							   *)
(*									   *)
(*  Input parameters:						           *)
(*      n -> the size (number of vertices) of the graphs generated;        *)
(*      a, b -> the two density parameters characterizing the type of      *)
(*		graph being generated, where a and b stisfy		   *)
(*		   0  <=  a  <=  b  <= 1  ;				   *)
(*	nb_prob -> the number of problem instances being generated (note   *)
(*		   that all instances generated in the same run will have  *)
(*		   identical characteristics and will be stored in a       *)
(*		   single file that will have to be split afterwards ---   *)
(*		    BEWARE of the resulting file SIZE when nb_prob > 1 );  *)
(*      f_prob -> name of the output file containing the graphs.	   *)
(*									   *)
(*  Output:								   *)
(*	the generated graph instances are copied on the output file	   *)
(*	"f_prob" in the DIMACS Challenge format, i.e. approx. 1 line per   *)
(*	edge, so BEWARE of output file sizes !!!			   *)
(*									   *)


  const
    epsilon = 0.01;
    maxnoeud = 1500;

  type
   intervalle    = 1..maxnoeud;

  var
    n, nb_prob, germe	: integer;
    a, b, h, s   	: real;
    i, j, k, edge_num	: integer;
    pchap 		: array [ intervalle ] of real;
    t_edge		: array [intervalle,intervalle] of integer;
    bool  		: boolean;
    f_prob		: packed array [1..30] of char;
    f			: text;
    p, dens		: real;
    exseed		: integer;


  function unif ( a,b : real ) : real;
  (* this procedure draws a random number *)
  (* in a Uniform[a,b] distribution       *)

  begin
    unif := a + ( random(1.0) * ( b - a ) );
  end;


(***&&  Main Program  &&***)
  begin
    readln( n, a, b, nb_prob);
    readln( f_prob);
    rewrite (f, f_prob);
    s := a;
    p := (a + b) / 2;

    for k := 1 to nb_prob do
    begin

(*** Computation of the generation seed ***)
      germe := 3011*k + 19*n + (k+1);
      exseed := seed(germe);

(***  Mise a zero ***)
      for i:= 1 to (n-1) do
      begin
	pchap[i] := 0;
	for j := (i+1) to n do  t_edge[i,j] := 0;
      end;
      pchap[n] := 0;
      edge_num := 0;

(***  Generation  ***)
      if (b-a) > epsilon then
        for i := 1 to n do pchap[i] := unif (a,b);

      for i := 1 to (n-1) do
        for j := (i+1) to n do
        begin
        h := random(1.0);
        if (b-a) > epsilon then
          s := (pchap[i] + pchap[j]) / 2;
        bool := h <= s;
        if bool then
        begin
	  t_edge[i,j] := 1;
	  edge_num := edge_num + 1;
        end;
      end;
      dens := edge_num * 2 / (n * (n-1));

(***  Output  ***)
      writeln(f, 'c File  ',f_prob);
      writeln(f,'c');
      writeln(f,'c Source: P. Soriano and M. Gendreau at  patrick@crt.umontreal.ca');
      writeln(f,'c');
      writeln(f,'c Reference: p-hat graphs are generated with the p-hat generator');
      writeln(f,'c 	      which is a generalization of the classical uniform');
      writeln(f,'c 	      random graph generator.');
      writeln(f,'c 	      Graphs generated with p-hat have wider node degree spread');
      writeln(f,'c 	      and larger cliques than uniform graphs.');
      writeln(f,'c');
      writeln(f,'c Generation parameters: ');
      writeln(f,'c 	n = ', n:6, ' a = ',a:5:2,'  b = ',b:5:2,'  Expected density = ',p:5:2);
      writeln(f,'c	Seed = ', germe:10,'       Real density = ', dens:6:3);
      writeln(f,'c');
      writeln(f,'p edge', n:5, edge_num:10);
      for i := 1 to (n-1) do
	for j := (i+1) to n do
	  if t_edge[i,j] = 1 then writeln(f,'e ', (i-1):5, (j-1):5);

(*  Graph instance separator  *)
      if nb_prob > 1 then writeln(f, 'c GRAPH_NUMBER ',nb_prob:2);
    end;

  end.
