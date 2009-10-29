
/* FILE       : alloc.c
 * WRITTEN BY : Craig Morgenstern
 */

# include <stdio.h>
# include <malloc.h>

void exit();

char *alloc(s)

   unsigned s;
{
   char *p;

   if ((p = malloc(s)) == NULL)
   {
      fprintf(stderr, "\n** malloc failed -- out of memory\n");
      exit(1);
   }
   return(p);
}
