# -*- coding: utf-8 -*-

"""
questo file contiene le definizioni delle funzioni che calcolano gli upperbound.
"""

import operator
from dsatur import dsatur_algorithm
from linearColoring import linear_coloring_algorithm
from numpy import linalg

"""
upperbound = numeri di nodi del grafo passato come parametro
"""
def upper_bound_from_number_of_nodes(graph):
    return len(graph)

"""
upperbound = massimo numero dei nodi che compongono il closed 
neighborhood di ogni nodo del grafo (= parametro)
"""
def upper_bound_from_largest_closed_neighborhood(graph):
    size_neighborhood_list = [(len(graph[node]) + 1) for node in graph.nodes()]
    size_neighborhood_list.sort(reverse=True)
    return size_neighborhood_list[1]

"""
upperbound = 
"""
def upper_bound_from_cardinality(graph):
    size_neighborhood_list = [(len(graph[node]) + 1) for node in graph.nodes()]
    size_neighborhood_list.sort(reverse=True)
    for i in xrange(len(size_neighborhood_list)):
        if size_neighborhood_list[i] <= i:
            return min(i - 1, size_neighborhood_list[i-1])

"""
upperbound = valore ricavato dall'algoritmo di eliminazione sequanziale
NB: la funzione di default per calcolare l'upper bound è
upper_bound_from_number_of_nodes
"""
def upper_bound_from_sequential_elimination_algorithm(graph, upper_bound_function, init=0):
    get_subgraph = graph.subgraph
    get_neighborhood = graph.closed_neighborhood
    get_nodes = graph.nodes
    remove_node = graph.remove_node
    ub_function = upper_bound_function
    upper_bound_opt = init
    max_upper_bound = init + 1
    while upper_bound_opt < max_upper_bound:
        upper_bounds = [(node, ub_function(get_subgraph(get_neighborhood(node)))) 
                        for node in get_nodes()]
        upper_bounds.sort(key=operator.itemgetter(1))
        node, induced_upper_bound = upper_bounds[0]
        upper_bound_opt = max(upper_bound_opt, induced_upper_bound)
        ignore, max_upper_bound = upper_bounds[len(upper_bounds) - 1]
        remove_node(node)
    return upper_bound_opt
"""
upperbound = numero di colori ricavato dall'algoritmo DSATUR
"""
def upper_bound_from_dsatur(graph):
    upper_bound = dsatur_algorithm(graph)
    return upper_bound

"""
upperbound = numero di colori ricavato dall'algoritmo di Linear Coloring
"""
def upper_bound_from_linear_coloring(graph):
    upper_bound = linear_coloring_algorithm(graph)
    return upper_bound

"""
upperbound = massimo autovalore della matrice di adiacenza 
del grafo passato come parametro + 1
"""
def upper_bound_from_max_eigenvalue(graph):
    matrix = graph.adjacency_matrix()
    eigenvalues = linalg.eigvals(matrix)
    max_eigenvalue = max(eigenvalues)
    return round(max_eigenvalue + 1)
