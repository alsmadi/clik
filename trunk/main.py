#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import numpy as NU
import networkx as NX
import matplotlib.pyplot as Plot
from sequentialEliminationAlgorithm import *
from upperBoundAlgorithms import *

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

"""main"""
def main(args):
#    filename = "/home/luigi/progetti/tesi/hamming6-2.clq"
#    filename = "/home/luigi/progetti/tesi/johnson8-2-4.clq"
#    filename = "/home/luigi/progetti/tesi/brock200_1.clq"
#    filename = "/home/luigi/progetti/tesi/hamming6-4.clq"
    filename = "/home/luigi/progetti/tesi/johnson16-2-4.clq"
#    filename = "/home/luigi/progetti/tesi/johnson8-4-4.clq"
    graph = get_graph(filename)
    print "start"
    upper_bound_opt, ignore = sequential_elimination_algorithm(graph, 
                            upper_bound_from_largest_closed_neighborhood)
    print "upper bound optimum: ", upper_bound_opt
    print "start addendum"
    upper_bound_opt = sequential_elimination_algorithm_addendum(graph, 
                            upper_bound_from_largest_closed_neighborhood)
    print "upper bound optimum (addendum): ", upper_bound_opt

if __name__ == "__main__":
    main(sys.argv)
