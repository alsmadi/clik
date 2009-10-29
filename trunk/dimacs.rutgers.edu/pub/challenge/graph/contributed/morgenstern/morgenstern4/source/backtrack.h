/* FILE       : backtrack.h
   WRITTEN BY : Craig Morgenstern
*/
     
void    initializeBT(/* n */);
     /* Need only be called once in main */
     
int    localized_deepBT(/* impasse, maxdepth, depth, maxBT, numBT, chi,
                           color, sigma, inconflict, firstadj, nextadj,
                           setcolor, uncolor */);
