CC = gcc
FLAGS = -O
LIBS = -lm
INCLS = alloc.h gen_graph.h graph.h graph_io.h random.h
OBJS = alloc.o gen_graph.o graph.o graph_io.o ggen.o
EXEC = ggen

$(EXEC) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o ggen $(LIBS)

$(OBJS) : $(INCLS)
	$(CC) $(FLAGS) -c $*.c 
