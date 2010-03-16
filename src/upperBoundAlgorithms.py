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
    nodes = graph.nodes()
    size_neighborhood_list = [(len(graph[node]) + 1) for node in nodes]
    size_neighborhood_list.sort(reverse=True)
    return size_neighborhood_list[1]

"""
upperbound =
"""
def upper_bound_from_cardinality(graph):
    nodes = graph.nodes()
    size_neighborhood_list = [(len(graph[node]) + 1) for node in nodes]
    size_neighborhood_list.sort(reverse=True)
    length = len(size_neighborhood_list) + 1
    for i in xrange(3, length):
        if size_neighborhood_list[i-1] <= i:
            return min(i, size_neighborhood_list[i-2])

"""
upperbound = valore ricavato dall'algoritmo di eliminazione sequanziale
"""
def upper_bound_from_sequential_elimination_algorithm(graph, ub_function, init=0):
    get_subgraph = graph.subgraph
    get_neighborhood = graph.closed_neighborhood
    get_nodes = graph.nodes
    remove_node = graph.remove_node
    upper_bound_opt = init
    nodes = get_nodes()
    upper_bounds = [(node, ub_function(get_subgraph(get_neighborhood(node))))
                    for node in nodes]
    upper_bounds.sort(key=operator.itemgetter(1), reverse=True)
    ignore, max_upper_bound = upper_bounds[0]
    while 1:
        nodes = get_nodes()
        upper_bounds = [(node, ub_function(get_subgraph(get_neighborhood(node))))
                        for node in nodes]
        upper_bounds.sort(key=operator.itemgetter(1))
        node, min_upper_bound = upper_bounds[0]
        upper_bound_opt = max(upper_bound_opt, min_upper_bound)
        ignore, max_upper_bound = upper_bounds[len(upper_bounds) - 1]
        if upper_bound_opt >= max_upper_bound:
            break
        remove_node(node)
    return upper_bound_opt

"""
upperbound = numero di colori ricavato dall'algoritmo DSATUR
"""
def upper_bound_from_dsatur(graph):
    return dsatur_algorithm(graph)

"""
upperbound = numero di colori ricavato dall'algoritmo di Linear Coloring
"""
def upper_bound_from_linear_coloring(graph):
    return linear_coloring_algorithm(graph)

"""
upperbound = massimo autovalore della matrice di adiacenza
del grafo passato come parametro + 1
"""
def upper_bound_from_max_eigenvalue(graph):
    matrix = graph.adjacency_matrix()
    eigenvalues = linalg.eigvals(matrix)
    max_eigenvalue = max(eigenvalues)
    return round(max_eigenvalue + 1)
