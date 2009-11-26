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
    append = upper_bounds.append
    for node in graph.nodes():
        node_closed_neighborhood = graph.closed_neighborhood(node)
        subgraph = graph.subgraph(node_closed_neighborhood)
        upper_bound = upper_bound_function(subgraph)
        append((node, upper_bound))
    return upper_bounds
