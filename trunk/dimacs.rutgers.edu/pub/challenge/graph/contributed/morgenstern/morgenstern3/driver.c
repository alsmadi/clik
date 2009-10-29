/* FILE: driver.c		WRITTEN BY: Craig Morgenstern */

# include <stdio.h>
# include <sys/types.h>
# include <sys/times.h>
# include <sys/time.h>
# include <sys/resource.h>
void exit();

# include "static_graph.h"
# include "color_ops.h"
# include "color_loop.h"

static  void printrusage()
{
        struct rusage rstats;
        getrusage(RUSAGE_SELF, &rstats);
        printf("      current running time = %d secs\n",
                        rstats.ru_utime.tv_sec);
        printf("      maximum resident set size = %d bytes\n",
                        rstats.ru_maxrss * getpagesize());
        printf("      number of major page faults = %d\n",
                        rstats.ru_majflt);
}


void	main()
{
	int K, verbose, maxBT, SGincr;
	char graph[128];

	struct tms buffer;
	int start_time, finish_time, zero_time, TotalTime, ColorTime;
	struct rusage flts;
	int start_major, finish_major, ColorFaults;

        times(&buffer);
        zero_time = buffer.tms_utime;

	scanf("%s %d %d %d %d %d",
	   graph, &K, &verbose, &maxBT, &SGincr);

	printf("Impasse Resolution by Localized Backtracking with Dynamic Ordering\n");
	printf("Graph file     : %s\n", graph);
	printf("Target chi     : %d\n", K);
	printf("Verbose        : %d\n", verbose);
	printf("BT ceiling     : %d\n", maxBT);
	printf("Subgraph incr  : %d\n", SGincr);

	printf("\n\n");

	if (! read_static_graph(graph)) exit(0);
	printf("Number vertices : %d\n", numvertices);
	printf("Number edges    : %d\n", numedges);
	printf("Max vertex deg  : %d\n", maxdeg);
	printf("\n");
	if (! create_colors(K)) exit(0);

        times(&buffer);
        start_time = buffer.tms_utime;
        getrusage(RUSAGE_SELF, &flts);
        start_major = flts.ru_majflt;

        if (color_loop(verbose, maxBT, SGincr))
	    printf("\n Successful Coloring\n");
	else
	    printf("\n Unsuccessful Coloring\n");
	printrusage();

        times(&buffer);
        finish_time = buffer.tms_utime;
        ColorTime = (finish_time - start_time) / 60;
        TotalTime = (finish_time - zero_time) / 60;
        getrusage(RUSAGE_SELF, &flts);
        finish_major = flts.ru_majflt;
	ColorFaults = finish_major - start_major;

	printf("\n");
	printf("Final K = %d   Proper = %d   All colored = %d\n",
		chivalue(), proper(), allcolored());
	printf("Total time = %d secs\n", TotalTime);
	printf("Color time = %d secs\n", ColorTime);
	printf("Major faults when coloring = %d\n", ColorFaults);
}
