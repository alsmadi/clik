/* FILE       : ggen.c
 * WRITTEN BY : Craig Morgenstern
 */

# include <stdio.h>
# include "alloc.h"
# include "graph.h"
# include "gen_graph.h"
# include "graph_io.h"
void exit();

void main(argc, argv)

   int argc;
   char *argv[];
{
   graph G;
   FILE *fp_log, *fp_in, *fp_out;
   
   if (argc < 2  ||  argc > 3)
   {
       printf("usage: %s [parameter_input_file] graph_output_file\n", argv[0]);
       exit(1);
   }

   if (argc == 2)
   {
       if ((fp_log = fopen(argv[1], "w")) == NULL)
       {
           printf("Could not open %s to write\n", argv[1]);
           exit(1);
       }
       G = new_graph();
       gen_graph(stdin, stdout, fp_log, G);
   }
   else

   {
       if ((fp_log = fopen(argv[2], "w")) == NULL)
       {
           printf("Could not open %s to write\n", argv[2]);
           exit(1);
       }
       if ((fp_in = fopen(argv[1], "r")) == NULL)
       {
           printf("Could not open %s to read\n", argv[1]);
           exit(1);
       }
       fp_out = fopen("/dev/null", "w");
       G = new_graph();
       gen_graph(fp_in, fp_out, fp_log, G);
   }
   fprintf(fp_log, "p col %d %d\n", high_vertex(G) + 1, num_edges(G));
   write_edges(fp_log, G);
}
