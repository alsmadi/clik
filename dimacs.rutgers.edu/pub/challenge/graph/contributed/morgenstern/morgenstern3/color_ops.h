/* FILE: color_ops.h	WRITTEN BY: Craig Morgenstern */

# define nocolor 255

extern int *CCcnt, numcc;
extern unsigned char *color; /* color labels are 0..254; 255 = no color */

# define ccsz(c) (*(CCcnt + (c)))
# define colorof(v) (*(color+(v)))
# define setcolor(v,c) colorof(v) = (c); ++ccsz(c)
# define uncolor(v) --ccsz(colorof(v)); colorof(v) = nocolor;

int create_colors();
int chivalue();
int inconflict(/* v, c */);
int proper();
int allcolored();
