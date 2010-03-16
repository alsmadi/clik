program Clique(input, output);

const
        nmax=200;
        mmax=20000;
        maxQ=50;
        maxL=50;

type
        Vet = array[0..nmax] of integer;
        Mat = array[1..nmax,1..nmax] of integer;

var
        n:integer;
        m,c,k,mc,edges:integer;
        b,r, check1,check2: integer;
        Q: Mat;
        g: Vet;
        F, P, somma:double;

        fname, fname1 :string;
        outFile: text;

        D: real;
        t,vert1,vert2, part1,part2, prove: integer;

begin (* main*)
	write('number of vertices = ');
	readln(n);
	write('density of the graph = ');
	readln(D);
	write('maximum clique cardinality = ');
	readln(k);
    write('file output name: ');
    readln(fname1);

    m:=trunc(n*(n-1)/2*(1-D)+0.5);
    mc:=n*(n-1) div 2-m;
    c:=n-k;

    (* check the input *)
    b:=n div k;
    r:=n mod k;
    check1:= r*b*(b+1) div 2 + (k-r)*b*(b-1) div 2;
    check2:=c*(c-1) div 2 + k*c;
    if (m<check1) or (m>check2) then begin
        writeln('the data are not consistent');
        readln;
        Halt(1)
    end;


    (* generate the instances *)
    prove := 10;
    randomize;
	for t:=0 to prove-1 do begin
        (* initialize vertex degree *)
        for vert1:=1 to n do
            g[vert1]:=0;


        for vert1:=1 to n do
            for vert2:=1 to n do
                Q[vert1,vert2]:=0;
        edges:=0;

        (* generate the cliques *)
        for vert1:=1 to n-1 do begin
            part1:=vert1 mod k;
            for vert2:=vert1+1 to n do begin
                part2:=vert2 mod k;
                if (part1=part2) then begin
                    g[vert1]:=g[vert1]+1;
                    g[vert2]:=g[vert2]+1;
                    Q[vert1,vert2]:=1;
                    Q[vert2,vert1]:=1;
                    edges:=edges+1;
                end;
            end; (* for vert2.. *)
       end; (* for vert1 *)

       {generate the remaining edges in such a way that at least one extreme is
       one of the vertices k+1,...,n that are the vertex cover}

       while (edges<m) do begin
           somma:=0;
           for vert1:=1 to n do
                somma:=somma+1/g[vert1];

           repeat
               P:=random*somma;
               F:=1/g[1];vert1:=1;
               while F < P do begin
                    vert1:=vert1+1;
                    F:=F+1/g[vert1]
               end;
               P:=random*somma;
               F:=1/g[1];vert2:=1;
               while F < P do begin
                    vert2:=vert2+1;
                    F:=F+1/g[vert2]
               end;
           until (vert1<>vert2) and ((vert1>k) or (vert2>k));
           if Q[vert1,vert2]=0 then begin
               g[vert1]:=g[vert1]+1;
               g[vert2]:=g[vert2]+1;
               Q[vert1,vert2]:=1;
               Q[vert2,vert1]:=1;
               edges:=edges+1
           end;
       end; (* while edges<m.... *)
       (* print *)
       fname := concat('In',fname1,'_',chr(t+ord('0')), '.txt');
       writeln('FNAME = ', fname);
       assign(outFile, fname);
	   rewrite(outFile);
       writeln(outFile, 'p edge ', n:4, mc :8, k:4);
       for vert1 := 1 to n-1 do
              for vert2 := vert1+1 to n do
                      if Q[vert1,vert2] = 0 then
                              writeln(outFile, 'e ',vert1:4, vert2:4);
       (* for vert1:=1 to n do writeln(outFile,vert1:4, g[vert1]:4); *)
       close(outFile);
    end;(*for prove*)
    writeln('terminato');
    readln;
end.
