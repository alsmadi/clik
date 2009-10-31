#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import numpy as NU
import networkx as NX
import matplotlib.pyplot as Plot
from sequentialEliminationAlgorithm import *
from upperBoundAlgorithms import *

"""main"""
def main(args):
    graph = NX.Graph()
    graph.add_nodes_from(['d', 'c', 'b', 'e', 'a', 'f', 'g'])
    graph.add_edges_from([('d', 'c'), ('c', 'b'), ('b', 'e'), ('c', 'a'), 
                          ('b', 'a'), ('f', 'a'), ('g', 'a')])
#    NX.draw(graph)
#    Plot.show()
    upper_bound_opt, graph_opt = sequential_elimination_algorithm(graph, 
                                upper_bound_from_adjacency_matrix)
    print "upper bound optimum: ", upper_bound_opt
    NX.draw(graph_opt)
    Plot.show()

if __name__ == "__main__":
    main(sys.argv)
