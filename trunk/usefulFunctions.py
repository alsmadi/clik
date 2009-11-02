# -*- coding: utf-8 -*-

import sys
import numpy as NU
import networkx as NX

"""
determina se un grafo è completo
"""
def is_complete(graph):
    n = len(graph.nodes())
    if len(graph.edges()) != (n * (n - 1) / 2) :
        return False
    return True

"""
funzione usata per calcolare il min/max della lista di upperbounds
"""
def upper_bounds_compare(x):
    return x[1]

"""
determina una lista di tuple (nodo, upperbound) con upperbound cacolato 
dal grafo indotto dal closed neighborhood di nodo, mediante la funzione 
passata come secondo parametro   
"""
def get_upper_bounds(graph, upper_bound_function):
    upper_bounds = []
    for node in graph:
        node_closed_neighborhood = get_node_closed_neighborhood(graph, node)
        subgraph = graph.subgraph(node_closed_neighborhood)
        upper_bound = upper_bound_function(subgraph)
        upper_bounds.append((node, upper_bound))
    return upper_bounds

"""
seleziona il nodo con l'upperbound di valore minore da una lista 
di tuple (nodo, upperbound)
"""
def get_min_upper_bound(upper_bounds):
    return min(upper_bounds, key=upper_bounds_compare)
    
"""
seleziona il nodo con l'upperbound di valore maggiore da una lista 
di tuple (nodo, upperbound) e ritorna il valore dell'upperbound
"""
def get_max_upper_bound(upper_bounds):
    return max(upper_bounds, key=upper_bounds_compare)

def get_node_closed_neighborhood(graph, node):
    node_neighborhood = graph[node].keys()
    node_neighborhood.append(node)
    return node_neighborhood
