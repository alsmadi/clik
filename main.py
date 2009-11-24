#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
questo file contiene il main; ho aggiunto qualche funzione per misurare 
il tempo di esecuzione.
il primo algoritmo è quello originale.
le definizioni degli algoritmi si trovano nel file sequentialEliminationAlgorithm.py
NB: probabilmente all'inizio dell'esecuzione vedrà alcuni warning; rigyardano 
una libreria usata non l'applicazione.
"""
import sys
from src.graphStructure import Graph
from src.sequentialEliminationAlgorithm import sequential_elimination_algorithm, sequential_elimination_algorithm_addendum
import src.upperBoundAlgorithms as UBA
import time
import os

"""
funzione per leggere il grafo dal file su disco
"""
def get_graph(filename):
    file = open(filename, 'r')
    edges = []
    for line in file:
        if line.find('e', 0, 1) >= 0:
            list = line.split()
            edges.append((list[1], list[2]))
    graph = Graph()
    graph.add_edges(edges)
    return graph

"""
main
"""
def main(args):
    cwd = os.getcwd()
    ext = ".clq"
#    testname = "graph_test" # 6 nodes
#    testname = "graph_testColor2" # 6 nodes
#    testname = "keller4" # 171 nodes
#    testname = "hamming6-2" # 64 nodes
#    testname = "hamming6-4" # 64 nodes
#    testname = "johnson16-2-4" # 120 nodes
#    testname = "johnson8-4-4" # 70 nodes
    testname = "johnson8-2-4" # 28 nodes
#    testname = "brock200_1" # 200 nodes
#    testname = "brock400_4" # 400 nodes
#    testname = "brock800_2" # 800 nodes
#    testname = "c-fat500-2" # 500 nodes
    filename = cwd + "/benchmarks/" + testname + ext
    graph = get_graph(filename)
    print "*****************************"
    print "*****************************"
    print "file: ", filename
    print "graph: ", testname
    ncolors = UBA.upper_bound_from_dsatur(graph)
    print ncolors
    #print "*****************************"
    #print "start first"
    #start = time.time()
    #upper_bound_opt = sequential_elimination_algorithm(graph, 
                                #UBA.upper_bound_from_max_eigenvalue)
    #print "end: elapsed time - ", time.time() - start
    #print "upper bound optimum: ", upper_bound_opt
    #print "*****************************"
    #print "*****************************"
    #print "start second"
    #start = time.time()
    #upper_bound_opt = sequential_elimination_algorithm_addendum(graph, 
    #                            UBA.upper_bound_from_number_of_nodes)
    #print "end: elapsed time (total) - ", time.time() - start
    #print "upper bound optimum (addendum): ", upper_bound_opt
    #print "*****************************"
    #print "*****************************"
    
if __name__ == "__main__":
    main(sys.argv)
