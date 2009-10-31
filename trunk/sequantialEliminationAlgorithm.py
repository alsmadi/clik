# -*- coding: utf-8 -*-

import sys
import numpy as NU
import networkx as NX

"""
funzione usata per calcolare il min/max della lista di upperbounds
"""
def upper_bounds_compare(x, y):
    if x[1] > y[1]:
        return 1
    elif x[1] == y[1]:
        return 0
    else:
        return -1

"""
determina una lista di tuple (nodo, upperbound) con upperbound cacolato 
dal grafo indotto dal closed neighborhood di nodo, mediante la funzione 
passata come secondo parametro   
"""
def get_upper_bounds(graph, upper_bound_function):
    upper_bounds = []
    for node in graph:
        node_neighborhood = graph_cur.neighbors(node)
        closed_node_neighborhood = node_neighborhood.append(node)
        subgraph = graph.subgraph(closed_node_neighborhood)
        upper_bound = ub_function(subgraph)
        upper_bounds.append((node, upper_bound))
    return upper_bounds

"""
seleziona il nodo con l'upperbound di valore minore da una lista 
di tuple (nodo, upperbound)
"""
def select_node(upper_bounds):
    min = upper_bounds.min(upper_bounds, key=upper_bounds_compare)
    return min[0]
    
"""
seleziona il nodo con l'upperbound di valore maggiore da una lista 
di tuple (nodo, upperbound) e ritorna il valore dell'upperbound
"""
def get_max_upper_bound(upper_bounds):
    max = upper_bounds.max(upper_bounds, key=upper_bounds_compare)
    return max[1]

def get_closed_node_neighborhood(graph, node):
    node_neighborhood = graph_cur.neighbors(node)
    return node_neighborhood.append(node)

def get_induced_upper_bound(graph, nodes):
    induced_subgraph = graph_cur.subgraph(nodes)
    return upper_bound_function(induced_subgraph)

"""
algoritmo di eliminazione sequenziale  
"""
def sequential_elimination_algorithm(graph, upper_bound_function):
    #varie inizializzazioni
    graph_cur = NX.Graph(data=graph)
    graph_opt = graph_cur
    upper_bound_opt = 0
    #determiniamo la lista degli upperbounds
    upper_bounds = get_upper_bounds(graph_cur, upper_bound_function)
    max_upper_bound = get_max_upper_bound(upper_bounds)
    while upper_bound_opt < max_upper_bound:
        #selezioniamo il nodo associato al minor upperbound
        node_ = select_node(upper_bounds)
        #determiniamo il closed neighborhood del nodo selezionato
        closed_node_neighborhood = get_closed_node_neighborhood(graph_cur, node)
        induced_upper_bound = get_induced_upper_bound(graph, closed_node_neighborhood)
        if upper_bound_opt < induced_upper_bound:
            upper_bound_opt = induced_upper_bound
            graph_opt = graph_cur
        #bisogna aggiornare i dati utili nelle iterazioni
        remaing_nodes = graph_cur.remove_node(node)
        graph_cur = graph_cur.subgraph(remaing_nodes)
        upper_bounds = get_upper_bounds(graph_cur, upper_bound_function)
        max_upper_bound = get_max_upper_bound(upper_bounds)
    return upper_bound_opt, graph_opt
