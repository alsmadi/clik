# -*- coding: utf-8 -*-

"""
questo file contiene le definizioni dei due algoritmi; entrambi fanno uso
di funzioni contenute nei file: upperBoundAlgorithms.py e in usefulFunctions.py
"""

import upperBoundAlgorithms as UBA
import time
import operator
from src.lowerBoundAlgorithms import lower_bound_from_MIN
import logging
import graphStructure

"""
algoritmo di eliminazione sequenziale.
i parametri sono:
* il grafo da studiare
* la funzione da usare per ricavare l'upperbound
"""
def sequential_elimination_algorithm_1(graph, ub_function, init=0):
    start = time.time()
    get_subgraph = graph.subgraph
    get_neighborhood = graph.closed_neighborhood
    get_nodes = graph.nodes
    remove_node = graph.remove_node
    getter = operator.itemgetter
    upper_bound_opt = init
    k = 0
    while 1:
        k += 1
        nodes = get_nodes()
        upper_bounds = [(node, ub_function(get_subgraph(get_neighborhood(node))))
                        for node in nodes]
        upper_bounds.sort(key=getter(1))
        ignore, max_upper_bound = upper_bounds[len(upper_bounds) - 1]
        if upper_bound_opt >= max_upper_bound:
            break
        node, min_upper_bound = upper_bounds[0]
        upper_bound_opt = max(min_upper_bound, upper_bound_opt)
        remove_node(node)
    t_tot = time.time() - start
    print "opt_1:", upper_bound_opt, "|", t_tot, "|", k

"""
algoritmo di eliminazione sequenziale addendum.
i parametri sono:
* il grafo da studiare
* le funzioni da usare per ricavare l'upperbound
"""
def sequential_elimination_algorithm_2(graph, ub_function_1, ub_function_2, init=0):
    start = time.time()
    lower_bound = lower_bound_from_MIN(graph)
    get_induced_subgraph = graph.induced_subgraph
    get_nodes = graph.nodes
    remove_node = graph.remove_node
    remove_nodes = graph.remove_nodes
    ub_function_3 = UBA.upper_bound_from_sequential_elimination_algorithm
    data = []
    append_data = data.append
    getter = operator.itemgetter
    while 1:
        nodes = get_nodes()
        del_nodes = []
        del_nodes_append = del_nodes.append
        upper_bounds = []
        upper_bounds_append = upper_bounds.append
        for node in nodes:
            subgraph = get_induced_subgraph(node)
            if len(subgraph) >= lower_bound:
                upper_bounds_append((node, ub_function_1(subgraph), subgraph))
            else:
                del_nodes_append(node)
        if len(upper_bounds) == 0:
            break
        min_item = min(upper_bounds, key=getter(1))
        append_data(min_item)
        del_nodes_append(min_item[0])
        remove_nodes(del_nodes)
    iter_1 = len(data)
    t_1 =  time.time() - start
    start = time.time()
    iter_2 = 0
    data = [item for item in data if len(item[2]) >= lower_bound]
    data.sort(key=getter(1), reverse=True)
    upper_bound_opt = 0
    for d in data:
        if d[1] > upper_bound_opt:
            upper_bound_opt = max(ub_function_3(d[2], ub_function_2, upper_bound_opt),
                                    upper_bound_opt)
            iter_2 += 1
        else:
            break
    t_2 =  time.time() - start
    t_tot = t_1 + t_2
    text = ''.join(["opt_2: ", str(upper_bound_opt), " | ", str(t_tot), " | ",
                    str(t_1), " | ", str(t_2), " | ", str(iter_1), " | ", str(iter_2)])
    logging.info(text)
    print text
