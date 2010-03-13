#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
questo file contiene il main; ho aggiunto qualche funzione per misurare 
il tempo di esecuzione.
il primo algoritmo è quello originale.
le definizioni degli algoritmi si trovano nel file sequentialEliminationAlgorithm.py
NB: probabilmente all'inizio dell'esecuzione vedrà alcuni warning; riguardano 
una libreria usata non l'applicazione.
"""

import sys
import os
from src.graphStructure import Graph
from src.sequentialEliminationAlgorithm import \
                                        sequential_elimination_algorithm_1, \
                                        sequential_elimination_algorithm_2
import src.upperBoundAlgorithms as UBA

"""
funzione per leggere il grafo dal file su disco
"""
def get_graph(filename):
    file = open(filename, 'r')
    def get_edge(list):
        return list[1], list[2]
    edges = [get_edge(line.split()) for line in file 
             if line.find('e', 0, 1) >= 0]
    graph = Graph(edges)
    return graph

"""
main
"""
def main(args):
    cwd = os.getcwd()
    ext = ".clq"
#    testname = "graph_test" # 7 nodes
#    testname = "myciel6(col)"
#    testname = "myciel4(col)"
#    testname = "myciel7(col)"
#    testname = "queen9(col)"
#    testname = "keller4" # 171 nodes
#    testname = "hamming6-2" # 64 nodes
#    testname = "MANN_a9" # 45 nodes
#    testname = "sanr200_0_7" # 200 nodes
#    testname = "hamming6-4" # 64 nodes
#    testname = "johnson16-2-4" # 120 nodes
    testname = "johnson8-4-4" # 70 nodes
#    testname = "johnson8-2-4" # 28 nodes
#    testname = "brock200_1" # 200 nodes
#    testname = "brock400_4" # 400 nodes
#    testname = "brock800_2" # 800 nodes
#    testname = "c-fat500-2" # 500 nodes
#    filename1 = cwd + "/benchmarks/" + "sanr200_0.7" + ext
#    filename2 = cwd + "/benchmarks/" + "brock200_1" + ext
#    filenames = [filename1, filename2]
#    for filename in filenames:
    filename = cwd + "/benchmarks/" + testname + ext
    print "*****************************"
    print "*****************************"
    print "file: ", filename
    print "graph: ", testname
#    print "*****************************"
#    print "start first"
#    graph = get_graph(filename)
#    sequential_elimination_algorithm_1(graph, UBA.upper_bound_from_linear_coloring)
    print "*****************************"
    print "start second"
    graph = get_graph(filename)
    sequential_elimination_algorithm_2(graph, UBA.upper_bound_from_number_of_nodes)

if __name__ == "__main__":
    main(sys.argv)
