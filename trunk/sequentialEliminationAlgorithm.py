# -*- coding: utf-8 -*-

"""
questo file contiene le definizioni dei due algoritmi; entrambi fanno uso 
di funzioni contenute nei file: upperBoundAlgorithms.py e in usefulFunctions.py
"""

import sys
import numpy as NU
import networkx as NX
from upperBoundAlgorithms import *
from usefulFunctions import *

"""
algoritmo di eliminazione sequenziale  
"""
def sequential_elimination_algorithm(graph, upper_bound_function):
    graph_cur = NX.Graph(data=graph)
    upper_bound_opt = 0
    k = 0
    while True:
        k += 1
        upper_bounds = get_upper_bounds(graph_cur, upper_bound_function)
        node, induced_upper_bound = get_min_upper_bound(upper_bounds)
        if upper_bound_opt < induced_upper_bound:
            upper_bound_opt = induced_upper_bound 
        ignore, max_upper_bound = get_max_upper_bound(upper_bounds)
        if upper_bound_opt < max_upper_bound:
            graph_cur.remove_node(node)
        else:
            print "# iterations: ", k
            return upper_bound_opt
          
"""
algoritmo di eliminazione sequenziale addendum
"""
def sequential_elimination_algorithm_addendum(graph, upper_bound_function):
    graph_cur = NX.Graph(data=graph)
    k = 0
    data= []
    while is_complete(graph_cur) is False: 
        upper_bounds = get_upper_bounds(graph_cur, upper_bound_function)
        node, induced_upper_bound = get_min_upper_bound(upper_bounds)
        node_closed_neighborhood = get_node_closed_neighborhood(graph_cur, node)
        data.append((node, induced_upper_bound, 
                     graph_cur.subgraph(node_closed_neighborhood)))
        graph_cur.remove_node(node)
        k += 1
    upper_bound_opt = len(graph_cur)
    for i in range(k):
        if data[i][1] > upper_bound_opt:
            upper_bound_tmp = upper_bound_from_sequential_elimination_algorithm(
                                            data[i][2], 
                                            upper_bound_from_adjacency_matrix, 
                                            upper_bound_opt)
            upper_bound_opt = max(upper_bound_opt, upper_bound_tmp)
    return upper_bound_opt
        
    
