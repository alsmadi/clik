/*  FILE       : graph_globals.h
    WRITTEN BY : Craig Morgenstern
     
    Valid vertex labels are in the range 0 .. numV-1.
    Valid color class labels are in the range 0 .. maxC-1.
    A vertex can have at most maxD neighbors ( max degree ).
    NIL indicates "no vertex", "no edge" or "no color class".
*/
     
#	define	NIL	-1
     
#    	define  numV    140000
#    	define  maxC    5         /* < 8 */
#    	define  maxD    75
