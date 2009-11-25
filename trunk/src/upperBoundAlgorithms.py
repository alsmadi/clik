# -*- coding: utf-8 -*-

"""
questo file contiene le definizioni delle funzioni che calcolano gli upperbound.
TODO: aggiungere un algoritmo di coloring o entrambi, tra linear coloring 
o dsatur
"""

from dsatur import dsatur_algorithm
from linearColoring import linear_coloring_algorithm
from numpy import linalg
from graphStructure import Graph
from usefulFunctions import get_upper_bounds, get_max_upper_bound, get_min_upper_bound

"""
upperbound = numeri di nodi del grafo passato come parametro
"""
def upper_bound_from_number_of_nodes(graph):
    return len(graph)

"""
upperbound = massimo numero dei nodi che compongono il closed 
neighborhood di ogni nodo del grafo passato come parametro
"""
def upper_bound_from_largest_closed_neighborhood(graph):
    size_neighborhood_list = []
    for node in graph.nodes():
        size_neighborhood_list.append(len(graph[node]) + 1)
    return max(size_neighborhood_list)

"""
upperbound = valore ricavato dall'algoritmo di eliminazione sequanziale
NB: la funzione di default per calcolare l'upper bound è
upper_bound_from_number_of_nodes
"""
def upper_bound_from_sequential_elimination_algorithm(graph, upper_bound_function, 
                                                      init=0):
    graph_cur = Graph(graph.edges())
    upper_bound_opt = init
    while True:
        upper_bounds = get_upper_bounds(graph_cur, upper_bound_function)
        node, induced_upper_bound = get_min_upper_bound(upper_bounds)
        if upper_bound_opt < induced_upper_bound:
            upper_bound_opt = induced_upper_bound 
        ignore, max_upper_bound = get_max_upper_bound(upper_bounds)
        if upper_bound_opt < max_upper_bound:
            graph_cur.remove_node(node)
        else:
            return round(upper_bound_opt)
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
    return max_eigenvalue + 1
