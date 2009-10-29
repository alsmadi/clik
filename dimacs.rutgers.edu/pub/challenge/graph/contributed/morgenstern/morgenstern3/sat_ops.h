/* FILE: sat_ops.h		WRITTEN BY: Craig Morgenstern */

extern char *which_heap;
# define sat_dirty(v) (*(which_heap + v) == 1)  /* has v been colored before? */

extern unsigned char *AdjColorCnt;
# define sat_inconflict0(v,c) ((*(AdjColorCnt + (v)*numcc + (c))) > 0)
# define sat_numAdjColors(v,c) (*(AdjColorCnt + (v)*numcc + (c)))

int create_sat();
int max_saturated_vertex();
void sat_makedirty(/* v */);
void sat_setcolor(/* v, c */);
void sat_uncolor(/* v */);
int sat_inconflict1(/* v, c */);
