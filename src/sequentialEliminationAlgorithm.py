# -*- coding: utf-8 -*-

"""
questo file contiene le definizioni dei due algoritmi; entrambi fanno uso 
di funzioni contenute nei file: upperBoundAlgorithms.py e in usefulFunctions.py
"""

from graphStructure import Graph
import upperBoundAlgorithms as UBA
from networkx.operators import subgraph
import time
import operator

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
    k = 0
    while True:
        k += 1
        upper_bounds = [(node, ub_function(get_subgraph(get_neighborhood(node)))) 
                        for node in get_nodes()]
        upper_bounds.sort(key=operator.itemgetter(1))
        node, induced_upper_bound = upper_bounds[0]
        upper_bound_opt = max(induced_upper_bound, upper_bound_opt)
        ignore, max_upper_bound = upper_bounds[len(upper_bounds) - 1]
        if upper_bound_opt < max_upper_bound:
            remove_node(node)
        else:
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
    graph_cur = graph
    get_subgraph = graph_cur.subgraph
    get_neighborhood = graph_cur.closed_neighborhood
    get_nodes = graph_cur.nodes
    remove_node = graph_cur.remove_node
    get_ub_from_linear_coloring = UBA.upper_bound_from_linear_coloring
    ub_function_1 = upper_bound_function
    ub_function_2 = UBA.upper_bound_from_sequential_elimination_algorithm
    data= []
    append_data = data.append
    min_graph = graph_cur
    while min_graph.is_complete() is False: 
        def get_upper_bounds(node):
            subgraph = get_subgraph(get_neighborhood(node))
            return node, ub_function_1(subgraph), subgraph
        upper_bounds = [get_upper_bounds(node) for node in get_nodes()]
        min_item = min(upper_bounds, key=operator.itemgetter(1))
        append_data(min_item)
        min_graph = min_item[2]
        remove_node(node)
    print "end: elapsed time (first part) - ", time.time() - start
    start = time.time()
    upper_bound_opt = len(min_graph)
    data = [item for item in data if item[1] > upper_bound_opt]
    data.sort(key=operator.itemgetter(1), reverse=True)
    print "# iterations: ", len(data)
    for i in range(len(data)):
        if data[i][1] > upper_bound_opt:
            upper_bound_tmp = ub_function_2(data[i][2], 
                                    get_ub_from_linear_coloring, 
                                    upper_bound_opt)
            upper_bound_opt = max(upper_bound_opt, upper_bound_tmp)
        else:
            return upper_bound_opt
    print "end: elapsed time (second part) - ", time.time() - start
    return upper_bound_opt
