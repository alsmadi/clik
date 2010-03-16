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
    t_tot = time.time() - start
    print "opt_1:", upper_bound_opt, "|", t_tot, "|", k
          
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
    remove_node = graph_cur.remove_node
    ub_function_1 = upper_bound_function
    ub_function_2 = UBA.upper_bound_from_sequential_elimination_algorithm
    ub_function_3 = UBA.upper_bound_from_linear_coloring
    data = []
    append_data = data.append
    min_graph = graph_cur
    while len(min_graph) >= lower_bound: 
        nodes = graph_cur.nodes()
        upper_bounds = []
        for node in nodes:
            subgraph = get_subgraph(get_neighborhood(node))
            upper_bounds.append((node, ub_function_1(subgraph), subgraph))
        min_item = min(upper_bounds, key=operator.itemgetter(1))
        append_data(min_item)
        min_graph = min_item[2]
        remove_node(node)
    iter_1 = len(data)
    t_1 =  time.time() - start
    start = time.time()
    upper_bound_opt = len(min_graph)
    data = [item for item in data if item[1] > upper_bound_opt]
    iter_2 = 0
    if len(data) != 0:
        data.sort(key=operator.itemgetter(1), reverse=True)
        for d in data:
            if d[1] > upper_bound_opt:
                upper_bound_opt = max(ub_function_2(d[2], ub_function_3, upper_bound_opt), 
                                      upper_bound_opt)
                iter_2 += 1
            else:
                break
    t_2 =  time.time() - start
    t_tot = t_1 + t_2
    print "opt_2:", upper_bound_opt, "|", t_tot, "|", t_1, "|", t_2, "|", iter_1, "|", iter_2