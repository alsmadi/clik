#include <stdio.h>
#define SIZE 5
#define MAXWORDS (1 << (2*SIZE) )
#define MASK ( (1 << (2*SIZE) ) - 1 )
#define ODDMASK (MASK & 357913941)
#define EVENMASK (MASK & 715827882)

main() {
unsigned long int ci, ctemp, nodelist[MAXWORDS];
int i, j, edgecount = 0, nodenum = 0; 
for(ci = 0; ci < MAXWORDS; ci++)
{   
    if ( ( ( (~ci)&ODDMASK )<<1 & (ci&EVENMASK) ) == 0) goto not_a_node;
    ctemp = (ci | (ci << 1) ) & EVENMASK;
    if ( (ctemp & (ctemp-1) ) == 0) goto not_a_node;
    nodelist[nodenum] = ci; 
    nodenum ++;
    not_a_node: ;
}

for (i=0; i<nodenum-1; i++)
    for (j=i; j<nodenum; j++)
        if (getbit(nodelist[i],nodelist[j]) == 0) edgecount++;

printf("p edge %d %d\n", nodenum, edgecount); 

printf("c Graph Keller-%d\n", SIZE);

for (i=0; i<nodenum-1; i++)
    for (j=i; j<nodenum; j++)
        if (getbit(nodelist[i],nodelist[j]) == 0) printf("e %d %d\n", i, j);

}

int getbit(ci,cj)
unsigned long int ci, cj;
{
    unsigned long int ctemp;
    ctemp = ( ~(ci^cj) ) & ODDMASK;
    if ( ( (ci^cj) & (ctemp << 1) ) == 0) return(1);
    ctemp = (ci^cj);
    ctemp = ( ctemp | (ctemp << 1) ) & EVENMASK;
    if ( (ctemp & (ctemp-1) ) == 0) return(1);
    return(0) ;
}

