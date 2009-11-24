# -*- coding: utf-8 -*-

"""
questo file contiene le definizioni dei due algoritmi; entrambi fanno uso 
di funzioni contenute nei file: upperBoundAlgorithms.py e in usefulFunctions.py
"""

from graphStructure import Graph
import upperBoundAlgorithms as UBA
from usefulFunctions import get_upper_bounds,get_min_upper_bound, get_max_upper_bound 
import time

"""
algoritmo di eliminazione sequenziale  
"""
def sequential_elimination_algorithm(graph, upper_bound_function):
    graph_cur = Graph(graph.edges())
    upper_bound_opt = 0
    k = 0
    while True:
        k += 1
        upper_bounds = get_upper_bounds(graph_cur, upper_bound_function)
        node, induced_upper_bound = get_min_upper_bound(upper_bounds)
        print "selected node: ", node
        if upper_bound_opt < induced_upper_bound:
            upper_bound_opt = induced_upper_bound 
        ignore, max_upper_bound = get_max_upper_bound(upper_bounds)
        if upper_bound_opt < max_upper_bound:
            graph_cur.remove_node(node)
        else:
            print "# iterations: ", k
            return round(upper_bound_opt)
          
"""
algoritmo di eliminazione sequenziale addendum
"""
def sequential_elimination_algorithm_addendum(graph, upper_bound_function):
    start = time.time()
    graph_cur = Graph(graph.edges())
    k = 0
    data= []
    while graph_cur.is_complete() is False: 
        upper_bounds = get_upper_bounds(graph_cur, upper_bound_function)
        node, induced_upper_bound = get_min_upper_bound(upper_bounds)
        print "selected node: ", node
        node_closed_neighborhood = graph_cur.closed_neighborhood(node)
        data.append((node, induced_upper_bound, 
                     graph_cur.subgraph(node_closed_neighborhood)))
        graph_cur.remove_node(node)
        k += 1  
    print "end: elapsed time (first part) - ", time.time() - start
    print "# iterations: ", k
    start = time.time()
    upper_bound_opt = len(graph_cur)
    for i in reversed(range(k)):
        print "iteration: ", k - i
        if data[i][1] > upper_bound_opt:
            upper_bound_tmp = UBA.upper_bound_from_sequential_elimination_algorithm(
                                            data[i][2], 
                                            UBA.upper_bound_from_dsatur, 
                                            upper_bound_opt)
            upper_bound_opt = max(upper_bound_opt, upper_bound_tmp)
    print "end: elapsed time (second part) - ", time.time() - start
    return round(upper_bound_opt)
        
    
