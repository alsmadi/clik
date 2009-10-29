#include <stdio.h>
#include "dim_def.c"

/* Transforms an instance of SET COVERING */
/* into an instance of MAXIMUM CLIQUE */

/* The input file contains a clutter C written in the following format: */
/* Line 1 : Number of elements of the ground set E(C) */
/* Line 2 : Number of members of the clutter C */
/* Following lines:  all the members of C  */
/* Each member is terminated by a star */

/* Example: the clutter C={t1,t2,t3}, where    */ 
/* t1={1,2,3} , t2 = {4,5,6} , t3 = {1,6,7,8}  */ 
/* will appear on the file as :                */
/*
    
8
3
1  2  3 *
4  5  6 *
1  6  7  8 *
 
*/

/* Let G=(V,E) be the graph produced by the transformer.           */
/* The minimum cover P of C and the maximum clique K of G satisfy: */
/*  |P| + |K| =  |E(C)| + |C|.                                     */


main( int argc, char *argv[] )
{
   char   name[60] ;
   NODE   GR_NNODES , CL_NNODES , ELEM1 , ELEM2 , I , CLIQUE_CARD;
   GRAPH  GRAPH1;
   int    CARD_CLI , C , NCONST , INVERT; 
   FILE   *CLUT_POINT ;
   SET    MEMBER;

   if ( argc > 3 || argc < 2)
   {
      printf("Usage: %s inp-file [out-file]\n\n",argv[0]);  
      exit(10); 
   }
   if (argc == 2)
   {
      strcpy(name , argv[1]);
      strcat(name , ".clq\0");
   }

   else  sprintf(name, "%s", argv[2]);

   printf("\n       Output file: %s \n\n" , name);   
   
   CLUT_POINT = fopen(argv[1] ,  "r");
   fscanf(CLUT_POINT , "%u" , &CL_NNODES);
   fscanf(CLUT_POINT , "%u" , &NCONST);

   GR_NNODES = CL_NNODES;

   for (I = 1; I <= GR_NNODES; I++)  emptyset(GRAPH1[I]);

   for (I = 1; I <= NCONST; I++)  
   {
     fread_set(CLUT_POINT , MEMBER) ;
     CLIQUE_CARD = card(MEMBER);
     if (CLIQUE_CARD > 2)
     {
        while (!is_empty(MEMBER))
        {
            ELEM1 = ext_elem(MEMBER);
            GR_NNODES++;
            emptyset(GRAPH1[GR_NNODES]);
            ins_elem(GRAPH1[GR_NNODES],ELEM1);
            ins_elem(GRAPH1[ELEM1],GR_NNODES);
         } /*** end while  ***/

         /** generation of a clique **/
         for (ELEM1 = (GR_NNODES-CLIQUE_CARD+1); ELEM1 <= GR_NNODES-1; ELEM1++)
             for (ELEM2 = ELEM1+1; ELEM2 <= GR_NNODES ; ELEM2++)
             {
     	        ins_elem(GRAPH1[ELEM2],ELEM1);
         	ins_elem(GRAPH1[ELEM1],ELEM2);
             }
     }
     else   
     {
          ELEM1 = ext_elem(MEMBER);
          ELEM2 = ext_elem(MEMBER);
          ins_elem(GRAPH1[ELEM2],ELEM1);
          ins_elem(GRAPH1[ELEM1],ELEM2);
     }
   }
   /* end for */
   fclose(CLUT_POINT);
   INVERT = true;
   fprint_graph_dim(name , GR_NNODES , GRAPH1 , INVERT);
   return;
}
