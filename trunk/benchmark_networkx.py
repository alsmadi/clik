#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import time
import random
from src.graphStructure import Graph, get_edges_from_file
import psyco      
import src.upperBoundAlgorithms as UBA

def main(args):
    psyco.full()
    list_nodes = [str(n) for n in xrange(1,201)]
#    test_list = [random.sample(list_nodes, 200) for i in xrange(100)]
    edges = get_edges_from_file("./benchmarks/brock200_3.clq")
#    edges = get_edges_from_file("./benchmarks/In200_40_13_0.txt")
#    edges = get_edges_from_file("./benchmarks/MANN_a9.clq")
    start = time.time()
    graph = Graph(edges)
    print "graph: elapsed time - ", time.time() - start
#    psyco.cannotcompile(graph.induced_subgraph_2)
#    print "graph: elapsed time - ", time.time() - start
    start = time.time()
    copy = graph.dcopy()
    print "copy: elapsed time - ", time.time() - start
    start = time.time()
    for i in xrange(1, 201):
        subgraph = graph.induced_subgraph(str(i)) 
    print "subgraph: elapsed time - ", time.time() - start
#    start = time.time()
#    for i in xrange(1, 201):
#        subgraph = graph.induced_subgraph_2(str(i)) 
#    print "subgraph_c: elapsed time - ", time.time() - start
    start = time.time()
    graph.remove_nodes(list_nodes)
    print "remove: elapsed time - ", time.time() - start

if __name__ == "__main__":
    main(sys.argv)