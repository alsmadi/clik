# -*- coding: utf-8 -*-

"""
questo file contiene le definizioni dei due algoritmi; entrambi fanno uso 
di funzioni contenute nei file: upperBoundAlgorithms.py e in usefulFunctions.py
"""

from graphStructure import Graph
import upperBoundAlgorithms as UBA
import time
import operator
from usefulFunctions import get_upper_bounds

"""
algoritmo di eliminazione sequenziale.
i parametri sono:
* il grafo da studiare
* la funzione da usare per ricavare l'upperbound
"""
def sequential_elimination_algorithm(graph, upper_bound_function):
    graph_cur = graph
    upper_bound_opt = 0
    k = 0
    while True:
        k += 1
        upper_bounds = [(node, upper_bound_function(graph_cur.subgraph(graph_cur.closed_neighborhood(node)))) 
                        for node in graph_cur.nodes()]
        node, induced_upper_bound = min(upper_bounds, key=operator.itemgetter(1))
        print "selected node: ", node
        upper_bound_opt = max(induced_upper_bound, upper_bound_opt)
        ignore, max_upper_bound = max(upper_bounds, key=operator.itemgetter(1))
        if upper_bound_opt < max_upper_bound:
            graph_cur.remove_node(node)
        else:
            print "# iterations: ", k
            return upper_bound_opt
          
"""
algoritmo di eliminazione sequenziale addendum.
i parametri sono:
* il grafo da studiare
* la funzione da usare per ricavare l'upperbound
"""
def sequential_elimination_algorithm_addendum(graph, upper_bound_function):
    start = time.time()
    graph_cur = graph
    k = 0
    data= []
    append_data = data.append
    while graph_cur.is_complete() is False: 
        upper_bounds = [(node, upper_bound_function(graph_cur.subgraph(graph_cur.closed_neighborhood(node)))) 
                        for node in graph_cur.nodes()]
        node, induced_upper_bound = min(upper_bounds, key=operator.itemgetter(1))
        print "selected node: ", node
        node_closed_neighborhood = graph_cur.closed_neighborhood(node)
        append_data((node, induced_upper_bound, 
                     graph_cur.subgraph(node_closed_neighborhood)))
        graph_cur.remove_node(node)
        k += 1  
    print "end: elapsed time (first part) - ", time.time() - start
    print "# iterations: ", k
    start = time.time()
    upper_bound_opt = len(graph_cur)
    upper_bound_function = UBA.upper_bound_from_sequential_elimination_algorithm
    for i in range(k):
        print "iteration: ", k - i
        if data[i][1] > upper_bound_opt:
            upper_bound_tmp = upper_bound_function(data[i][2], 
                                            UBA.upper_bound_from_linear_coloring, 
                                            upper_bound_opt)
            upper_bound_opt = max(upper_bound_opt, upper_bound_tmp)
    print "end: elapsed time (second part) - ", time.time() - start
    return upper_bound_opt
        
  
