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
def sequential_elimination_algorithm(graph, upper_bound_function):
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
        #--------------------------------- if upper_bound_opt < max_upper_bound:
        remove_node(node)
        #----------------------------------------------------------------- else:
    print "# iterations: ", k
    return upper_bound_opt
          
"""
algoritmo di eliminazione sequenziale addendum.
i parametri sono:
* il grafo da studiare
* la funzione da usare per ricavare l'upperbound
"""
def sequential_elimination_algorithm_addendum(graph, upper_bound_function, init=0):
    start = time.time()
    lower_bound = lower_bound_from_MIN(graph)
    graph_cur = graph
    get_subgraph = graph_cur.subgraph
    get_neighborhood = graph_cur.closed_neighborhood
    get_nodes = graph_cur.nodes
    remove_node = graph_cur.remove_node
    get_ub_from = UBA.upper_bound_from_linear_coloring
    ub_function_1 = upper_bound_function
    ub_function_2 = UBA.upper_bound_from_sequential_elimination_algorithm
    data= []
    append_data = data.append
    min_graph = graph_cur
    # min_graph.is_complete() is False and 
    while len(min_graph) > lower_bound: 
        def get_upper_bounds(node):
            subgraph = get_subgraph(get_neighborhood(node))
            return node, ub_function_1(subgraph), subgraph
        upper_bounds = [get_upper_bounds(node) for node in get_nodes()]
        min_item = min(upper_bounds, key=operator.itemgetter(1))
        append_data(min_item)
        min_graph = min_item[2]
        remove_node(node)
    print "# iterations (first part): ", len(data)
    print "elapsed time (first part) - ", time.time() - start
    start = time.time()
    upper_bound_opt = len(min_graph)
    data = [item for item in data if item[1] > upper_bound_opt]
    i = 0
    if len(data) != 0:
        data.sort(key=operator.itemgetter(1), reverse=True)
        for i in xrange(0, len(data)):
            if data[i][1] > upper_bound_opt:
                upper_bound_tmp = ub_function_2(data[i][2], 
                                        get_ub_from, 
                                        upper_bound_opt)
                upper_bound_opt = max(upper_bound_opt, upper_bound_tmp)
            else:
                break
    print "# iterations (second part): ", i, "/", len(data)
    print "elapsed time (second part) - ", time.time() - start
    return upper_bound_opt
