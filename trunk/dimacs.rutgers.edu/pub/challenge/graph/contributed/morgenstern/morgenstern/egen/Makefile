CC = gcc
FLAGS = -O
OBJS = egen.o circuit.o graph.o triangulate.o
INCLS = circuit.h graph.h triangulate.h urand.h
EXEC = egen

$(EXEC): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(EXEC)

$(OBJS): $(INCLS)
	$(CC) $(FLAGS) -c $*.c
