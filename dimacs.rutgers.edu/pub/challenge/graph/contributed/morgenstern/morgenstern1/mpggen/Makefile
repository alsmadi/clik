INCLS = circuit.h circuit_globals.h graph.h halt.h graph_globals.h \
	generate_map.h memory.h memory_globals.h mpg5.h triangulate2.h \
	triangulate1.h urand.h

OBJS  = generate_map.o graph.o circuit.o memory.o mpg5.o \
	pas_urand.o triangulate1.o triangulate2.o urand.o

FLAGS = -double -case -nowalkback -noframe

EXEC  = mpg

$(EXEC) : $(INCLS) $(OBJS) mpg_gen.pas
	pc $(FLAGS) $(OBJS) mpg_gen.pas -output $(EXEC)

generate_map.o : $(INCLS)
	pc $(FLAGS) -nomain -object generate_map.pas
graph.o  : $(INCLS)
	pc $(FLAGS) -nomain -object graph.pas
circuit.o : $(INCLS)
	pc $(FLAGS) -nomain -object circuit.pas
memory.o : $(INCLS)
	pc $(FLAGS) -nomain -object memory.pas
mpg5.o : $(INCLS)
	pc $(FLAGS) -nomain -object mpg5.pas
pas_urand.o : $(INCLS)
	pc $(FLAGS) -nomain -object pas_urand.pas
triangulate1.o : $(INCLS)
	pc $(FLAGS) -nomain -object triangulate1.pas
triangulate2.o : $(INCLS)
	pc $(FLAGS) -nomain -object triangulate2.pas
urand.o :
	cc -c urand.c
