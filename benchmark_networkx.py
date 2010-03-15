#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import networkx
import time
import random
from src.graphStructure import Graph
 
def get_edges_from_file(filename):
    try:
        file = open(filename, 'r')
        def get_edge(list):
            return list[1], list[2]
        edges = [get_edge(line.split()) for line in file 
                 if line.find('e', 0, 1) >= 0]
        return edges
    except:
        print "Errorre nel aprire il file!"
        
def main(args):
    list_nodes = [str(n) for n in xrange(1,401)]
    test_list = [random.sample(list_nodes, 200) for i in xrange(100)]
    edges = get_edges_from_file("./benchmarks/brock400_4.clq")
    start = time.time()
    graph = Graph(edges)
    print "graph: elapsed time - ", time.time() - start
    start = time.time()
    for i in xrange(10):
        subgraph = graph.subgraph(test_list[i]) 
    print "subgraph: elapsed time - ", time.time() - start
    start = time.time()
    graph.remove_nodes(list_nodes)
    print "remove: elapsed time - ", time.time() - start

if __name__ == "__main__":
    main(sys.argv)