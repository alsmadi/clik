CC     = gcc
CFLAGS = -O
INCLS  = bt.h sat_ops.h color_ops.h static_graph.h subgraph.h color_loop.h
OBJS   = driver.o bt.o sat_ops.o color_ops.o static_graph.o \
	 subgraph.o color_loop.o
EXEC   = bt.x

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

$(OBJS): $(INCLS)
	$(CC) $(CFLAGS) -c $*.c
