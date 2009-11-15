# -*- coding: utf-8 -*-

"""
in questo file ho raccolto alcune funzioni utili per l'esecuzione.
"""

import operator

"""
determina una lista di tuple (nodo, upperbound) con upperbound cacolato 
dal grafo indotto dal closed neighborhood di nodo, mediante la funzione 
passata come secondo parametro   
"""
def get_upper_bounds(graph, upper_bound_function):
    upper_bounds = []
    for node in graph.nodes():
        node_closed_neighborhood = graph.closed_neighborhood(node)
        subgraph = graph.subgraph(node_closed_neighborhood)
        upper_bound = upper_bound_function(subgraph)
        upper_bounds.append((node, upper_bound))
    return upper_bounds

"""
seleziona il nodo con l'upperbound di valore minore da una lista 
di tuple (nodo, upperbound)
"""
def get_min_upper_bound(upper_bounds):
    return min(upper_bounds, key=operator.itemgetter(1))
    
"""
seleziona il nodo con l'upperbound di valore maggiore da una lista 
di tuple (nodo, upperbound) e ritorna il valore dell'upperbound
"""
def get_max_upper_bound(upper_bounds):
    return max(upper_bounds, key=operator.itemgetter(1))

