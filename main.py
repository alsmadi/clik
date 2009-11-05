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
import numpy as NU
import networkx as NX
import matplotlib.pyplot as Plot
from sequentialEliminationAlgorithm import *
from upperBoundAlgorithms import *
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
    graph = NX.Graph()
    graph.add_edges_from(edges)
    return graph

"""
main
"""
def main(args):
	cwd = os.getcwd()
	ext = ".clq"
#    testname = "graph_test"
#    testname = "hamming6-2"
#    testname = "hamming6-4"
#    testname = "johnson16-2-4"
	testname = "johnson8-4-4"
#	 testname = "johnson8-2-4"
#    testname = "brock200_1"
#    testname = "brock400_4"
#    testname = "brock800_2"
#    testname = "c-fat500-2"
	filename = cwd + "/" + testname + ext
	graph = get_graph(filename)
	print "*****************************"
	print "*****************************"
	print "file: ", filename
	print "graph: ", testname
	print "*****************************"
	print "start first"
	start = time.time()
	upper_bound_opt = sequential_elimination_algorithm(graph, 
							upper_bound_from_adjacency_matrix)
	print "end: elapsed time - ", time.time() - start
	print "upper bound optimum: ", upper_bound_opt
	print "*****************************"
	print "*****************************"
	print "start second"
	start = time.time()
	upper_bound_opt = sequential_elimination_algorithm_addendum(graph, 
							upper_bound_from_adjacency_matrix)
	print "end: elapsed time - ", time.time() - start
	print "upper bound optimum (addendum): ", upper_bound_opt
	print "*****************************"
	print "*****************************"

if __name__ == "__main__":
    main(sys.argv)
