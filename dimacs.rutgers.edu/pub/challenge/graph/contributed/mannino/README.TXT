
clut2graph.c    is a C-program which generates instances of MAX CLIQUE 
                from instances of MINIMUM SET COVERING. It includes the file    
dim_def.c       where subroutines and constants are defined.

The usage of the program is the following: 
      
     Clut2graph inp-file [out-file]  

where inp_file is a name of an input file, containing a clutter written 
in the following format:

   Line 1 : Number of elements of the ground set E(C) 
   Line 2 : Number of members of the clutter C 
   Following lines:  all the members of C    
   Each member is terminated by a star   

   Example: the clutter C={t1,t2,t3}, where       
   t1={1,2,3} , t2 = {4,5,6} , t3 = {1,6,7,8}     
   will appear on the file as :                  
  
    
8
3
1  2  3 *
4  5  6 *
1  6  7  8 *
 
  
The output file is a graph in DIMACS TEXT FORMAT. 
Let G=(V,E) be the graph produced by the transformer.           
The minimum cover P of C and the maximum clique K of G satisfy:   
         
                |P| + |K| =  |E(C)| + |C|.              
 

In the example, |P| = 2, |E(C)| = 8, |C| = 3 and thus |K| = 9. 

In this way, from Steiner Triple Systems A9 , A27 , A45 , A81, I  
generated the instances MANN_A9.clq , MANN_A27.clq , MANN_A45.clq , MANN_A81.clq

I will put a note on the FTP area with additional information about how the 
transformation operates. Essentially, it is an extension of the well 
known transformation from 3-SAT to VERTEX COVER (see M.Garey and D.Johnson, 
COMPUTERS AND INTRACTABILITY, pp. 53-56). 
 
Carlo Mannino

E-MAIL address    mannino@iasi.rm.cnr.it 
