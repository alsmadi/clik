/* FILE       : res.c
   WRITTEN BY : Craig Morgenstern
*/
     
#    include <stdio.h>
#    include <sys/types.h>
#    include <sys/times.h>
#    include <sys/time.h>
#    include <sys/resource.h> 
     
#    include "graph_globals.h"
#    include "graph_ops.h"
#    include "class_ops.h"
#    include "shuffle.h"
#    include "urand.h"
#    include "backtrack.h"
#    include "sat_ops.h"
#    include "smallest_last.h"
#    include "static_ops.h"
#    include "single_wander.h"
#    include "kempe.h"
     
#    define    random            0
#    define    smallest_last     1
#    define    saturation        2
     
#    define    none              0
#    define    localized_deep_BT 1
#    define    Wander            2
#    define    Wander_BT         3
#    define    Kempe_chain       4
#    define    Wander_Kempe      5
#    define    W2_LDBT_KEM	 6
     
typedef void	(*PFV)();
typedef int    	(*PFI)();
typedef double  (*PFD)();
static  PFV    	SETCOLOR, UNCOLOR;
static	PFI    	COLOROF, INCONFLICT, NUMADJCOLORS;
     
static	FILE	*mapfp, *methodfp, *datafp;

static	struct  tms	buffer;
static  int     start_time, finish_time;
static  struct  rusage  flts;  
static	int	start_minor, start_major, finish_minor, finish_major;
static  int     SIGMA[ numV ];
     
#	define	MG	10
#	define	MI	75000

static	double	BTfact;
static  int    	target, Otype, Oseed, Rtype, maxBT, maxdepth, T, maxIchanges, 
		maxrestarts, n, g, numgraphs, Fcolorings;

static  int	Wcalls[MG], BTcalls[MG], Kcalls[MG],
		FIcnt[MG], Icnt[MG], MajorSwaps[MG], MinorSwaps[MG], 
		ExeTime[MG], NumRestarts[MG], Gsize[MG];

static	int	ICHANGES[MI], CHAINSIZE[MI], IPCENT[MI], SSWAPS[MI], 
		USWAPS[MI], BTS[MI], BTDEPTH[MI],
		ICHANGEScnt, CHAINSIZEcnt, IPCENTcnt, SSWAPScnt, USWAPScnt, 
		BTScnt, BTDEPTHcnt;


/*-----------------------------------------------------------------------------
  ---------------------  max, mean and median routines  -----------------------
  -----------------------------------------------------------------------------
*/
     
static	int	max(A, Asz)
	int	*A, Asz;
{
	int 	i, m;
	if ( Asz == 0 ) return(-1);
	for ( m = A[0], i = 1;  i < Asz;  i++ ) if ( A[i] > m ) m = A[i];
	return( m );
}
     
static	int	mean(A, Asz)
	int	*A, Asz;
{
	int 	i, m;
	if ( Asz == 0 ) return(-1);
	for ( m = 0, i = 0;  i < Asz;  i++ ) m += A[i];
	return( m/Asz );
}
     
static	int	intcompare( a, b )
	int	*a, *b;
{
	return( *a - *b );
}

static	int	median(A, Asz)
	int	*A, Asz;
{
	if ( Asz < 2 ) return(-1);
	qsort( (char *) A, Asz, sizeof(int), intcompare );
        return( (Asz % 2 == 1) ? (A[Asz/2]) : ((A[Asz/2]+A[Asz/2-1])/2) );
}

/*-----------------------------------------------------------------------------
  ----------------------------  ResolutionColor  ------------------------------
  -----------------------------------------------------------------------------
*/
     
int	ResolutionColor()
{
        int    c, i, v, numBT, mindepth, depth, impasse_resolved, 
               swaps, interchanges, StartOseed;

        times(&buffer);
        start_time = buffer.tms_utime;
	getrusage(RUSAGE_SELF, &flts);
	start_minor = flts.ru_minflt;
	start_major = flts.ru_majflt; 
     
	Gsize[g] = n;
	StartOseed = Oseed;

restart:
	FIcnt[g] = 0;
        createclasses();
        switch ( Otype )
        {
              case random :
                   for ( i = 0;  i < n;  ++i )  SIGMA[i] = i;
                   shuffle( SIGMA, n, &Oseed, urand );
                   create_static(n);
                   break;
              case smallest_last :
                   for ( i = 0;  i < n;  ++i )  SIGMA[i] = i;
                   shuffle( SIGMA, n, &Oseed, urand );
                   create_static(n);
                   SmallestLast( SIGMA );
              case saturation :
                   create_sat( n, &Oseed, urand );
                   break;
        }
     
        for ( i = 0;  i < n;  ++i )
        {
             if ( Otype == saturation )
                  v = max_saturated_vertex();
             else
                  v = SIGMA[i];
     
             for ( c = 0;  c < target && (*INCONFLICT)(v,c);  ++c );
     
             if ( c != target )
                  (*SETCOLOR)(v,c);
             else
             {
		  FIcnt[g]++;
		  Icnt[g]++;
                  switch ( Rtype )
                  {
                       case none :
                            if ( NumRestarts[g] < maxrestarts )
                            {
                                 ++NumRestarts[g];
                                 goto restart;
                            }
			    else
                                 goto done;
                            break;
     
                       case localized_deep_BT :
			    BTcalls[g]++;
                            mindepth = (i > maxdepth) ? (i-maxdepth) : (1);
     
                            impasse_resolved =
                              localized_deepBT( i, mindepth, &depth,
				   (i*BTfact > maxBT) ? (int)(i*BTfact) : maxBT,
				   &numBT, target, *COLOROF,v, *INCONFLICT, 
				   firstadj, nextadj, *SETCOLOR, *UNCOLOR );
     
			    BTS[BTScnt++] = numBT;
			    BTDEPTH[BTDEPTHcnt++] += i - depth;
                            if ( ! impasse_resolved )
                                 if ( NumRestarts[g] < maxrestarts )
                                 {
                                      ++NumRestarts[g];
                                      goto restart;
                                 }
                                 else
                                      goto done;
                            break;
     
                       case Wander :
			    Wcalls[g]++;
                            impasse_resolved =
                                  singleWander(v, target, T, &swaps,
                                               firstadj, nextadj, *SETCOLOR,
                                               *UNCOLOR,*COLOROF,*NUMADJCOLORS);
     
                            if ( ! impasse_resolved )
                            {
				 USWAPS[USWAPScnt++] = swaps;
                                 if ( NumRestarts[g] < maxrestarts )
                                 {
                                      ++NumRestarts[g];
                                      goto restart;
                                 }
                                 else
                                      goto done;
                            }
                            else
				 SSWAPS[SSWAPScnt++] = swaps;
                            break;
     
                       case Wander_BT :
			    Wcalls[g]++;
                            impasse_resolved =
                              singleWander(v, target, T, &swaps,
                                           firstadj, nextadj,
                                           *SETCOLOR, *UNCOLOR, *COLOROF,
                                           *NUMADJCOLORS );
     
                            if ( ! impasse_resolved )
                            {
				 USWAPS[USWAPScnt++] = swaps;
				 BTcalls[g]++;
                                 mindepth = (i>maxdepth)?(i-maxdepth):(1);
                                 impasse_resolved =
                                   localized_deepBT( i, mindepth, &depth,
				   (i*BTfact > maxBT) ? (int)(i*BTfact) : maxBT,
						     &numBT, target, *COLOROF,v,
                                                     *INCONFLICT, firstadj,
                                                     nextadj, *SETCOLOR,
                                                     *UNCOLOR );
				 BTS[BTScnt++] = numBT;
				 BTDEPTH[BTDEPTHcnt++] = i - depth;
                                 if ( ! impasse_resolved )
                                      if ( NumRestarts[g] < maxrestarts )
                                      {
                                           ++NumRestarts[g];
                                           goto restart;
                                      }
                                      else
                                           goto done;
                            }
			    else
				 SSWAPS[SSWAPScnt++] = swaps;
                            break;

     
                       case Kempe_chain :
			    Kcalls[g]++;
                            impasse_resolved =
                                  Kempe(v, i, target, maxIchanges, 
					&interchanges, 
					CHAINSIZE + CHAINSIZEcnt,
					IPCENT + IPCENTcnt,
					firstadj, nextadj, 
					*SETCOLOR, *UNCOLOR, *COLOROF,
					*INCONFLICT);

			    CHAINSIZEcnt += interchanges;
			    IPCENTcnt += interchanges;
			    ICHANGES[ICHANGEScnt++] = interchanges;
                            if ( ! impasse_resolved )
                            {
                                 if ( NumRestarts[g] < maxrestarts )
                                 {
                                      ++NumRestarts[g];
                                      goto restart;
                                 }
                                 else
                                      goto done;
                            }
                            break;

                       case Wander_Kempe :
			    Wcalls[g]++;
                            impasse_resolved =
                              singleWander(v, target, T, &swaps,
                                           firstadj, nextadj,
                                           *SETCOLOR, *UNCOLOR, *COLOROF,
                                           *NUMADJCOLORS );
     
                            if ( ! impasse_resolved )
                            {
				 USWAPS[USWAPScnt++] = swaps;
				 Kcalls[g]++;
                                 impasse_resolved =
                                  Kempe(v, i, target, maxIchanges, 
				        &interchanges, 
					CHAINSIZE + CHAINSIZEcnt,
					IPCENT + IPCENTcnt,
					firstadj, nextadj, 
					*SETCOLOR, *UNCOLOR, *COLOROF,
					*INCONFLICT);

			         CHAINSIZEcnt += interchanges;
			         IPCENTcnt += interchanges;
			         ICHANGES[ICHANGEScnt++] = interchanges;
                                 if ( ! impasse_resolved )
                                      if ( NumRestarts[g] < maxrestarts )
                                      {
                                           ++NumRestarts[g];
                                           goto restart;
                                      }
                                      else
                                           goto done;
			    }
			    else
				 SSWAPS[SSWAPScnt++] = swaps;
                            break;
     
                       case W2_LDBT_KEM :
			    Wcalls[g]++;
                            impasse_resolved =
                              singleWander(v, target, T, &swaps,
                                           firstadj, nextadj,
                                           *SETCOLOR, *UNCOLOR, *COLOROF,
                                           *NUMADJCOLORS );
     
                            if ( ! impasse_resolved )
                            {
				 USWAPS[USWAPScnt++] = swaps;
				 BTcalls[g]++;
                                 mindepth = (i>maxdepth)?(i-maxdepth):(1);
                                 impasse_resolved =
                                   localized_deepBT( i, mindepth, &depth,
				   (i*BTfact > maxBT) ? (int)(i*BTfact) : maxBT,
						     &numBT, target, *COLOROF,v,
                                                     *INCONFLICT, firstadj,
                                                     nextadj, *SETCOLOR,
                                                     *UNCOLOR );
				 BTS[BTScnt++] = numBT;
				 BTDEPTH[BTDEPTHcnt++] = i - depth;
                                 if ( ! impasse_resolved )
                                 {
				      Kcalls[g]++;
                                      impasse_resolved =
                                       Kempe(v, i, target, maxIchanges, 
				             &interchanges, 
					     CHAINSIZE + CHAINSIZEcnt,
					     IPCENT + IPCENTcnt,
					     firstadj, nextadj, 
					     *SETCOLOR, *UNCOLOR, *COLOROF,
					     *INCONFLICT);

			              CHAINSIZEcnt += interchanges;
			              IPCENTcnt += interchanges;
			              ICHANGES[ICHANGEScnt++] = interchanges;
                                      if ( ! impasse_resolved )
                                           if ( NumRestarts[g] < maxrestarts )
                                           {
                                                ++NumRestarts[g];
                                                goto restart;
                                           }
                                           else
                                                goto done;
			         }
                            }
			    else
				 SSWAPS[SSWAPScnt++] = swaps;
                            break;
                  }
             }
        }
     
done :
        times(&buffer);
        finish_time = buffer.tms_utime;
	ExeTime[g] = finish_time - start_time;
	getrusage(RUSAGE_SELF, &flts);
	finish_minor = flts.ru_minflt;
	finish_major = flts.ru_majflt; 
	MajorSwaps[g] = finish_major;
	MinorSwaps[g] = finish_minor;
	if ( allcolored(n) )  Fcolorings++;
	if ( !proper() )
	{
	     fprintf(datafp, "*** IMPROPER COLORATION:graph %d:seed %d\n", 
		     g, StartOseed);
	     exit(1);
	}
}


/*-----------------------------------------------------------------------------
  ------------------------------------  main  ---------------------------------
  -----------------------------------------------------------------------------
*/

void 	main( argc, argv )
	
	int	argc;
	char	*argv[];
{
	int	dummy, v, w;

	if ( argc != 4 ) exit(1);
	if ( (mapfp = fopen(argv[1], "r")) == NULL ) exit(1);
	if ( (methodfp = fopen(argv[2], "r")) == NULL ) exit(1);
	if ( (datafp = fopen(argv[3], "a")) == NULL ) exit(1);

	if ( fscanf(mapfp, "%d", &numgraphs) == 0 ) exit(1);
        if ( fscanf(methodfp, "%d%d%d%d%d%d%d%d%d%lf",
		&target, &Otype, &Oseed, &Rtype, &maxBT, &maxdepth, &T, 
		&maxIchanges, &maxrestarts, &BTfact) != 10 ) exit(1);

	fprintf(datafp, "------------------------------------------------------------------------\n");
	fprintf(datafp, "Coloring %d graphs in file %s\n", numgraphs, argv[1]);

	init_urand();
	createvertices(numV, 3*numV);
        COLOROF = colorof;
        fprintf(datafp, "target chi          : %d\n", target);

        fprintf(datafp, "vertex ordering     : ");
        switch( Otype )
        {
             case random : fprintf(datafp, "random\n");
                  INCONFLICT = static_inconflict; SETCOLOR = static_setcolor;
                  UNCOLOR = static_uncolor; NUMADJCOLORS = static_numAdjColors;
                  break;
             case smallest_last : fprintf(datafp, "smallest last\n");
                  INCONFLICT = static_inconflict; SETCOLOR = static_setcolor;
                  UNCOLOR = static_uncolor; NUMADJCOLORS = static_numAdjColors;
                  break;
             case saturation : fprintf(datafp, "saturation\n");
                  INCONFLICT = sat_inconflict; SETCOLOR = sat_setcolor;
                  UNCOLOR = sat_uncolor; NUMADJCOLORS = sat_numAdjColors;
                  break;
             default :
                  fprintf(datafp, "invalid vertex ordering method specified\n");
                  exit(1);
        }
        fprintf(datafp, "impasse resolution  : ");
        switch ( Rtype )
        {
             case none : 
		  fprintf(datafp, "none\n"); 
		  break;
             case localized_deep_BT :
                  fprintf(datafp, "localized deep backtrack\n");
                  fprintf(datafp, "max backtracks      : %d\n", maxBT);
                  fprintf(datafp, "backtrack factor    : %1.2lf\n", BTfact);
                  fprintf(datafp, "max levels          : %d\n", maxdepth);
                  initializeBT(n);
                  break;
             case Wander :
                  fprintf(datafp, "single wander\n");
                  fprintf(datafp, "max crossings       : %d\n", T);
                  break;
             case Wander_BT :
                  fprintf(datafp, "single wander / localized deep backtrack\n");
                  fprintf(datafp, "max crossings       : %d\n", T);
                  fprintf(datafp, "max backtracks      : %d\n", maxBT);
                  fprintf(datafp, "backtrack factor    : %1.2lf\n", BTfact);
                  fprintf(datafp, "max levels          : %d\n", maxdepth);
                  initializeBT(n);
                  break;
             case Kempe_chain :
                  fprintf(datafp, "Kempe chaining\n");
                  fprintf(datafp, "max interchanges    : %d\n", maxIchanges);
                  break;
	     case Wander_Kempe :
		  fprintf(datafp, "single wander / Kempe chaining\n");
                  fprintf(datafp, "max crossings       : %d\n", T);
                  fprintf(datafp, "max interchanges    : %d\n", maxIchanges);
                  break;
	     case W2_LDBT_KEM :
		  fprintf(datafp, "single wander / LDBT / Kempe chaining\n");
                  fprintf(datafp, "max crossings       : %d\n", T);
                  fprintf(datafp, "max backtracks      : %d\n", maxBT);
                  fprintf(datafp, "backtrack factor    : %1.2lf\n", BTfact);
                  fprintf(datafp, "max levels          : %d\n", maxdepth);
                  fprintf(datafp, "max interchanges    : %d\n", maxIchanges);
                  initializeBT(n);
                  break;
             default :
                  fprintf(datafp, "invalid resolution method specified\n");
                  exit(1);
        }
	fprintf(datafp, "\n");
	fflush(datafp);

	for ( g = 0;  g < numgraphs;  g++ )
	{
	     while ( (dummy = getc(mapfp)) != '@' );
             fscanf(mapfp, "%d", &n);
	     for ( v = 0;  v < n;  ++v )
	          for (fscanf(mapfp,"%d",&w);  w != NIL;  fscanf(mapfp,"%d",&w))
		       addedge(v,w);
	     ResolutionColor();
             for ( v = 0;  v < n;  ++v )  removevertex(v);
	     fprintf(datafp, "Graph %2d : %7.2lf secs \n", 
			     g, ExeTime[g]/60.0);
	     fflush(datafp);
	}
	fprintf(datafp, "\n");

	fprintf(datafp, 
		"Num of vertices (max, median, mean)  : (%d, %d, %d)\n",
		max(Gsize, numgraphs), median(Gsize, numgraphs), 
		mean(Gsize, numgraphs));
	fprintf(datafp, 
		"Time secs       (max, median, mean)  : (%d, %d, %d)\n",
		max(ExeTime, numgraphs)/60, median(ExeTime, numgraphs)/60, 
		mean(ExeTime, numgraphs)/60);
	fprintf(datafp, 
		"Major faults    (max, median, mean)  : (%d, %d, %d)\n",
		max(MajorSwaps, numgraphs), median(MajorSwaps, numgraphs), 
		mean(MajorSwaps, numgraphs));
	fprintf(datafp, 
		"Minor faults    (max, median, mean)  : (%d, %d, %d)\n",
		max(MinorSwaps, numgraphs), median(MinorSwaps, numgraphs), 
		mean(MinorSwaps, numgraphs));
	fprintf(datafp, 
		"Num of restarts (max, median, mean)  : (%d, %d, %d)\n",
		max(NumRestarts, numgraphs), median(NumRestarts, numgraphs), 
		mean(NumRestarts, numgraphs));
	fprintf(datafp, 
		"Total Icnt      (max, median, mean)  : (%d, %d, %d)\n",
		max(Icnt, numgraphs), median(Icnt, numgraphs), 
		mean(Icnt, numgraphs));
	fprintf(datafp, 
		"Final Icnt      (max, median, mean)  : (%d, %d, %d)\n",
		max(FIcnt, numgraphs), median(FIcnt, numgraphs), 
		mean(FIcnt, numgraphs));
	fprintf(datafp, 
		"Wander Calls    (max, median, mean)  : (%d, %d, %d)\n",
		max(Wcalls, numgraphs), median(Wcalls, numgraphs), 
		mean(Wcalls, numgraphs));
	fprintf(datafp, 
		"BT Calls        (max, median, mean)  : (%d, %d, %d)\n",
		max(BTcalls, numgraphs), median(BTcalls, numgraphs), 
		mean(BTcalls, numgraphs));
	fprintf(datafp, 
		"Kempe Calls     (max, median, mean)  : (%d, %d, %d)\n",
		max(Kcalls, numgraphs), median(Kcalls, numgraphs), 
		mean(Kcalls, numgraphs));
	fprintf(datafp, "Num of successful 4-colorings: %d\n", Fcolorings);
	fprintf(datafp, "\n");

	fprintf(datafp, "Sswaps (max, median, mean)    : %5d  %5d  %5d\n",
			max(SSWAPS, SSWAPScnt),
			median(SSWAPS, SSWAPScnt),
			mean(SSWAPS, SSWAPScnt) );
	fprintf(datafp, "Uswaps (max, median, mean)    : %5d  %5d  %5d\n",
			max(USWAPS, USWAPScnt),
			median(USWAPS, USWAPScnt),
			mean(USWAPS, USWAPScnt) );
	fprintf(datafp, "Ichanges (max, median, mean)  : %5d  %5d  %5d\n",
			max(ICHANGES, ICHANGEScnt),
			median(ICHANGES, ICHANGEScnt),
			mean(ICHANGES, ICHANGEScnt) );
	fprintf(datafp, "chainsize (max, median, mean) : %5d  %5d  %5d\n",
			max(CHAINSIZE, CHAINSIZEcnt),
			median(CHAINSIZE, CHAINSIZEcnt),
			mean(CHAINSIZE, CHAINSIZEcnt) );
	fprintf(datafp, "ICpercent (max, median, mean) : %5d  %5d  %5d\n",
			max(IPCENT, IPCENTcnt),
			median(IPCENT, IPCENTcnt),
			mean(IPCENT, IPCENTcnt) );
	fprintf(datafp, "BTs (max, median, mean)       : %5d  %5d  %5d\n",
			max(BTS, BTScnt),
			median(BTS, BTScnt),
			mean(BTS, BTScnt) );
	fprintf(datafp, "BT depth (max, median, mean)  : %5d  %5d  %5d\n",
			max(BTDEPTH, BTDEPTHcnt),
			median(BTDEPTH, BTDEPTHcnt),
			mean(BTDEPTH, BTDEPTHcnt) );

	fprintf(datafp, "------------------------------------------------------------------------\n");
}
