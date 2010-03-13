# -*- coding: utf-8 -*-

"""
questo file contiene le definizioni dei due algoritmi; entrambi fanno uso 
di funzioni contenute nei file: upperBoundAlgorithms.py e in usefulFunctions.py
"""

import upperBoundAlgorithms as UBA
import time
import operator
from src.lowerBoundAlgorithms import lower_bound_from_MIN

"""
algoritmo di eliminazione sequenziale.
i parametri sono:
* il grafo da studiare
* la funzione da usare per ricavare l'upperbound
"""
def sequential_elimination_algorithm_1(graph, upper_bound_function):
    start = time.time()
    graph_cur = graph
    get_subgraph = graph_cur.subgraph
    get_neighborhood = graph_cur.closed_neighborhood
    get_nodes = graph_cur.nodes
    remove_node = graph_cur.remove_node
    ub_function = upper_bound_function
    upper_bound_opt = 0
    max_upper_bound = 1
    k = 0
    while upper_bound_opt < max_upper_bound:
        k += 1
        upper_bounds = [(node, ub_function(get_subgraph(get_neighborhood(node)))) 
                        for node in get_nodes()]
        upper_bounds.sort(key=operator.itemgetter(1))
        node, induced_upper_bound = upper_bounds[0]
        upper_bound_opt = max(induced_upper_bound, upper_bound_opt)
        ignore, max_upper_bound = upper_bounds[len(upper_bounds) - 1]
        remove_node(node)
    print "end: elapsed time - ", time.time() - start
    print "# iterations: ", k
    print "upper bound optimum: ", upper_bound_opt
          
"""
algoritmo di eliminazione sequenziale addendum.
i parametri sono:
* il grafo da studiare
* la funzione da usare per ricavare l'upperbound
"""
def sequential_elimination_algorithm_2(graph, upper_bound_function, init=0):
    start = time.time()
    lower_bound = lower_bound_from_MIN(graph)
    graph_cur = graph
    get_subgraph = graph_cur.subgraph
    get_neighborhood = graph_cur.closed_neighborhood
    get_nodes = graph_cur.nodes
    remove_node = graph_cur.remove_node
    get_ub_from = UBA.upper_bound_from_linear_coloring
    ub_function_1 = upper_bound_function
#    ub_function_2 = UBA.upper_bound_from_sequential_elimination_algorithm
    ub_function_2 = UBA.upper_bound_from_linear_coloring
    data= []
    append_data = data.append
    min_graph = graph_cur
    def get_upper_bounds(node):
        subgraph = get_subgraph(get_neighborhood(node))
        return node, ub_function_1(subgraph), subgraph
    while len(min_graph) >= lower_bound: 
        upper_bounds = [get_upper_bounds(node) for node in get_nodes()]
        min_item = min(upper_bounds, key=operator.itemgetter(1))
        append_data(min_item)
        min_graph = min_item[2]
        remove_node(node)
#    print "# iterations (first part): ", len(data)
#    print "elapsed time (first part) - ", time.time() - start
#    start = time.time()
    upper_bound_opt = len(min_graph)
    data = [item for item in data if item[1] > upper_bound_opt]
    if len(data) != 0:
        data.sort(key=operator.itemgetter(1), reverse=True)
        for d in data:
            if d[1] > upper_bound_opt:
#                upper_bound_opt = max(ub_function_2(d[2], get_ub_from, upper_bound_opt), 
#                                      upper_bound_opt)
                upper_bound_opt = max(ub_function_2(d[2]), upper_bound_opt)
            else:
                break
    print "elapsed time - ", time.time() - start
    print "# iterations (second part): ", len(data)
    print "upper bound optimum (addendum): ", upper_bound_opt
#    return upper_bound_opt
