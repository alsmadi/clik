


#define WORD 32                       /* length of an integer variable */
#define MAX_WORDS  100      
#define MAX_ELEM   MAX_WORDS*WORD     /* max. number of nodes */
#define DIM_SET MAX_WORDS-1           


typedef int SET[MAX_WORDS];
typedef SET *PSET;
typedef int NODE;
typedef SET GRAPH[MAX_ELEM + 1];


enum boolean {false , true};


/*****    CARD     *****/
int card(SET set1);
/* Returns the cardinality of the set set1 */

/******   EMPTYSET    *****/
void emptyset(SET SET1);
/*  SET1 = 0 */

/*****    EXT_ELEM    *****/
int ext_elem(SET set1);
/* Extracts the first element in set1 */

/******  FPRINT_GRAPH_DIM   ******/
void  fprint_graph_dim(char *name , NODE NNODES , GRAPH GRAPH1 , int INVERT);
/* Prints a GRAPH on a file in DIMACS TEXT FORMAT (not binary) */  

/*****    FREAD_SET     *****/
int fread_set(FILE  *fp , SET set1);
/* Reads a set from the file pointed by fp */

/******   INS_ELEM    *****/
int ins_elem(SET SET1 , int elem);
/* INSERT elem in set1       */

/******   IS_EMPTY    *****/
int is_empty(SET set1);
/* returns true if set1 is empty */


                 /****    CODES  *****/

/*****    CARD     *****/
int card(SET set1)
/* Returns the cardinality of the set set1 */
{
   int   x , y , conta ;

   conta = 0;
   for (y = 0 ; y <= DIM_SET ; y++)
     if (set1[y] != 0)
       for (x = 0 ; x <= (WORD-1) ; x++)
           if ((set1[y] >> x) & 1)   conta++ ;
       /* end for x */
   /* end for y */
   return conta ;
}


/******   EMPTYSET    *****/
void emptyset(SET SET1)
/* empties SET1 */
{
   int k ;
   for(k = 0; k <= DIM_SET; k++)
       SET1[k] = 0;
   return;
}



/******  FPRINT_GRAPH_DIM   ******/
void  fprint_graph_dim(char *name , NODE NNODES , GRAPH GRAPH1 , int INVERT)
/* Prints a GRAPH on a file in DIMACS TEXT FORMAT (not binary) */  
{
   NODE     J , K , ELEM , CLIQUE_CARD;
   FILE     *FILE_P;
   int      NEDGES  ;
   SET      SET1;
   double   DENSITY;

   NEDGES = 0; 
   for (J=1;J<=NNODES;J++)     NEDGES = NEDGES + card(GRAPH1[J]);           
   NEDGES = NEDGES/2;
   if (INVERT) NEDGES = ((NNODES*(NNODES-1))/2)-NEDGES;
   FILE_P = fopen(name , "w");
   DENSITY = 2*NEDGES;
   DENSITY = DENSITY/(NNODES*(NNODES-1));
   fprintf(FILE_P , "c  EDGE DENSITY: %1.4f \n" , DENSITY);    

   fprintf(FILE_P , "p edge %3d %8d\n" , NNODES , NEDGES); 
   for (J=1;J<=NNODES;J++)
       for (K=1;K<=J-1;K++)
       {
           if  (!INVERT && (in(GRAPH1[J] , K)))    
                fprintf(FILE_P , "e %3d %3d\n" , J , K);
           if  (INVERT && (!in(GRAPH1[J] , K)))    
                fprintf(FILE_P , "e %3d %3d\n" , J , K);
       }      
  
   fclose(FILE_P);
       
   printf("#Nodes %4d   Density: %1.4f  \n\n" , NNODES , DENSITY); 
   return;
}


/******   IN    *****/
int in(SET set1 , int elem)
/* Returns true if elem belongs to set1 */

{
   int com_set ;
   int  x , y ; 

   y = (elem-1) / WORD ;
   x = (elem-1) % WORD ;

   com_set = set1[y] >> x;

   if (com_set & 1)  return true ;
   else              return  false ;

}


/******   INS_ELEM    *****/
int ins_elem(SET SET1 , int elem)
/* INSERT elem in set SET1      */
/* returns 1 if elem > MAX_ELEM */

{
   int  z;                /* z is the mask                    */
   int  x , y ;           /* y is the entry in the array SET1 */
		          /* x  is the bit to be set to one   */
		          /* i.e. the number of shits         */

   z = 1 ;
   if (elem > MAX_ELEM) return 1;   /* error code = 1 */

   y = (elem-1) / WORD ;
   x = (elem-1) % WORD ;
   z = z << x;
   SET1[y] =  (SET1[y] |  z);
   return  NULL ;
}


/******   IS_EMPTY    *****/
int is_empty(SET set1)
/* returns true if set1 is empty */
{
   int k ;
   for(k = 0; k <= DIM_SET; k++)
       if (set1[k] != 0) return false;
  return true;
}


/*****    EXT_ELEM    *****/
int ext_elem(SET set1)
/* Extracts the first element in set1 */
{
   int  com_set ;
   int   k , elem ;

   for(k = 0; k <= DIM_SET; k++)
       if (set1[k] != 0)
       {
          com_set = set1[k];
          elem = 1;
          while ((com_set & 1) == 0)
          {
            elem++;
	    com_set = com_set >> 1 ;
	  }
	  com_set = 1 ;
	  com_set = com_set << elem-1;
	  com_set = ~com_set;
	  set1[k] =  (set1[k] &  com_set);
	  return (WORD * k + elem) ;
        }
   /* endfor */
   return 0 ;
}



/*****    FREAD_SET     *****/
int fread_set(FILE  *fp , SET set1)
/* Reads a set from the file pointed by fp */
{
   int  x , y , conta , elem  , k ;
   int  com_set , C;


   for(k = 0; k <= DIM_SET; k++)
       set1[k] = 0;
   C = getc(fp);
   while ((C != '*') && (C != EOF) )
   {
	if ( isdigit(C) )
	{
	   ungetc(C , fp);
	   com_set = 1 ;
	   fscanf(fp , "%u" , &elem);
	   if (elem > MAX_ELEM) return 1;
	   y = (elem-1) / WORD ;
	   x = (elem-1) % WORD ;
	   com_set = com_set << x;
	   set1[y] =  (set1[y] |  com_set);
	} /* end if */
	C = getc(fp);
   } /* end while */

   return;
}

