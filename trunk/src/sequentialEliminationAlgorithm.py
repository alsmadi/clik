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

"""
algoritmo di eliminazione sequenziale.
i parametri sono:
* il grafo da studiare
* la funzione da usare per ricavare l'upperbound
"""
def sequential_elimination_algorithm_1(nfile, graph, ub_function_1, init=0):
    l = str(len(graph))
    e = str(graph.number_of_edges())
    start = time.time()
    remove_node = graph.remove_node
    getter = operator.itemgetter
    upper_bound_opt = init
    k = 0
    while 1:
        k += 1
        upper_bounds = ub_function_1(graph)
        upper_bounds.sort(key=getter(1))
        ignore, max_upper_bound = upper_bounds[len(upper_bounds) - 1]
        if upper_bound_opt >= max_upper_bound:
            break
        node, min_upper_bound = upper_bounds[0]
        upper_bound_opt = max(min_upper_bound, upper_bound_opt)
        remove_node(node)
    t_tot = round(time.time() - start, 2)
    text = ''.join([str(nfile), "; ", l, "; ", e, "; ", str(upper_bound_opt), 
                    "; ", str(t_tot), "; ", str(k)])
    logging.info(text)
    print text

def sequential_elimination_algorithm_1_p(nfile, graph, ub_function_1, init=0):
    l = str(len(graph))
    e = str(graph.number_of_edges())
    start = time.time()
    remove_node = graph.remove_node
    upper_bound_opt = init
    k = 0
    while 1:
        k += 1
        node, ub_min, ub_max = ub_function_1(graph)
#        print upper_bound_opt, node, ub_min, ub_max
        if ub_min > upper_bound_opt:
            upper_bound_opt = ub_min
        if upper_bound_opt >= ub_max:
            break
        remove_node(node)
    t_tot = round(time.time() - start, 2)
    text = ''.join([str(nfile), "; ", l, "; ", e, "; ", str(upper_bound_opt), 
                    "; ", str(t_tot), "; ", str(k)])
    logging.info(text)
    print text
"""
algoritmo di eliminazione sequenziale addendum.
i parametri sono:
* il grafo da studiare
* le funzioni da usare per ricavare l'upperbound
"""
def sequential_elimination_algorithm_2(nfile, graph, ub_function_1, ub_function_2, init=0):
    l = str(len(graph))
    e = str(graph.number_of_edges())
    start = time.time()
    lower_bound = lower_bound_from_MIN(graph)
    remove_node = graph.remove_node
    ub_function_3 = UBA.upper_bound_from_sequential_elimination_algorithm
    data = []
    append_data = data.append
    getter = operator.itemgetter
    while len(graph) >= lower_bound:
        upper_bounds = ub_function_1(graph)
        min_item = min(upper_bounds, key=getter(1))
        append_data(min_item)
        remove_node(min_item[0])
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
    t_tot = round(t_1 + t_2, 2)
    t_1 = round(t_1, 2)
    t_2 = round(t_2, 2)
    text = ''.join([str(nfile), "; ", l, "; ", e, "; ", str(upper_bound_opt), 
                    "; ", str(t_tot), "; ", str(t_1), "; ", str(t_2), "; ", 
                    str(iter_1), "; ", str(iter_2)])
    logging.info(text)
    print text

def sequential_elimination_algorithm_2_p(nfile, graph, ub_function_1, ub_function_2, init=0):
    l = str(len(graph))
    e = str(graph.number_of_edges())
    start = time.time()
    lower_bound = lower_bound_from_MIN(graph)
    remove_node = graph.remove_node
    ub_function_3 = UBA.upper_bound_from_sequential_elimination_algorithm_2_p
    data = []
    append_data = data.append
    getter = operator.itemgetter
    while len(graph) >= lower_bound:
        min_item = ub_function_1(graph)
        append_data(min_item)
        remove_node(min_item[0])
    iter_1 = len(data)
    t_1 =  time.time() - start
    start = time.time()
    iter_2 = 0
    data = [item for item in data if len(item[2]) >= lower_bound]
    data.sort(key=getter(1), reverse=True)
    upper_bound_opt = 0
    for d in data:
        if d[1] > upper_bound_opt:
            ub = ub_function_3(d[2], ub_function_2, upper_bound_opt)
            if ub > upper_bound_opt:
                upper_bound_opt = ub
#            upper_bound_opt = max(ub_function_3(d[2], ub_function_2, upper_bound_opt),
#                                    upper_bound_opt)
            iter_2 += 1
        else:
            break
    t_2 =  time.time() - start
    t_tot = round(t_1 + t_2, 2)
    t_1 = round(t_1, 2)
    t_2 = round(t_2, 2)
    text = ''.join([str(nfile), "; ", l, "; ", e, "; ", str(upper_bound_opt), 
                    "; ", str(t_tot), "; ", str(t_1), "; ", str(t_2), "; ", 
                    str(iter_1), "; ", str(iter_2)])
    logging.info(text)
    print text