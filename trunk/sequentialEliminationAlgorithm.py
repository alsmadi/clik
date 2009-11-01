# -*- coding: utf-8 -*-

import sys
import numpy as NU
import networkx as NX
from upperBoundAlgorithms import *
from usefulFunctions import *

"""
algoritmo di eliminazione sequenziale  
"""
def sequential_elimination_algorithm(graph, upper_bound_function):
    #varie inizializzazioni
    graph_cur = NX.Graph(data=graph)
    graph_opt = NX.Graph(data=graph)
    upper_bound_opt = 0
    #determiniamo la lista degli upperbounds
    upper_bounds = get_upper_bounds(graph_cur, upper_bound_function)
    ignore, max_upper_bound = get_max_upper_bound(upper_bounds)
    while upper_bound_opt < max_upper_bound:
        #selezioniamo il nodo associato al minor upperbound
        node, ignore = get_min_upper_bound(upper_bounds)
        #determiniamo il closed neighborhood del nodo selezionato
        node_closed_neighborhood = get_node_closed_neighborhood(graph_cur, node)
        induced_upper_bound = upper_bound_function(graph.subgraph(
                                                node_closed_neighborhood))
        if upper_bound_opt < induced_upper_bound:
            upper_bound_opt = induced_upper_bound
            graph_opt = NX.Graph(data=graph_cur)
        #bisogna aggiornare i dati utili nelle iterazioni
        remaing_nodes = graph_cur.remove_node(node)
        graph_cur = graph_cur.subgraph(remaing_nodes)
        upper_bounds = get_upper_bounds(graph_cur, upper_bound_function)
        ignore, max_upper_bound = get_max_upper_bound(upper_bounds)
    return upper_bound_opt,  graph_opt

"""
algoritmo di eliminazione sequenziale addendum
"""
def sequential_elimination_algorithm_addendum(graph, upper_bound_function):
    graph_cur = NX.Graph(data=graph)
    k = 0
    data= []
    while is_complete(graph_cur) is False: 
        upper_bounds = get_upper_bounds(graph_cur, 
                    upper_bound_from_sequential_elimination_algorithm)
        node, upper_bound = get_min_upper_bound(upper_bounds)
        node_closed_neighborhood = get_node_closed_neighborhood(graph_cur, node)
        data.append((node, upper_bound, graph_cur.subgraph(node_closed_neighborhood)))
        remaing_nodes = graph_cur.remove_node(node)
        graph_cur = graph_cur.subgraph(remaing_nodes)
        k += 1
        print "iterazione: ", k
    upper_bound_opt = len(graph_cur)
    for i in range(k):
        if data[i][1] > upper_bound_opt:
            upper_bound_opt = max(upper_bound_opt, upper_bound_function(data[i][2]))
    return upper_bound_opt
        
    
