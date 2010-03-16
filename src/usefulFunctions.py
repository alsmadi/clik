# -*- coding: utf-8 -*-

"""
determina una lista di tuple (nodo, upperbound) con upperbound cacolato 
dal grafo indotto dal closed neighborhood di nodo, mediante la funzione 
passata come secondo parametro   
"""
def get_upper_bounds(graph, upper_bound_function):
    return [(node, upper_bound_function(graph.subgraph(graph.closed_neighborhood(node)))) 
            for node in graph.nodes()]

def unique(seq):
    keys = {}
    for e in seq:
        if not e is None: 
            keys[e] = 1
    return keys.keys()