# -*- coding: utf-8 -*-

"""
questo file contiene le definizioni delle funzioni che calcolano gli upperbound.
TODO: aggiungere un algoritmo di coloring o entrambi, tra linear coloring 
o dsatur
"""
import sys
import numpy as NU
import networkx as NX
from usefulFunctions import *

"""
upperbound = numeri di nodi del grafo passato come parametro
"""
def upper_bound_from_number_of_nodes(graph):
    if not isinstance(graph, NX.Graph):
        raise Exception, "Not a Graph!"
    return len(graph)

"""
upperbound = massimo numero dei nodi che compongono il closed 
neighborhood di ogni nodo del grafo passato come parametro
"""
def upper_bound_from_largest_closed_neighborhood(graph):
    if not isinstance(graph, NX.Graph):
        raise Exception, "Not a Graph!"
    size_neighborhood_list = []
    for node in graph:
        size_neighborhood_list.append(len(graph[node]) + 1)
    return max(size_neighborhood_list)

"""
upperbound = valore ricavato dall'algoritmo di eliminazione sequanziale
NB: la funzione di default per calcolare l'upper bound è
upper_bound_from_number_of_nodes
"""
def upper_bound_from_sequential_elimination_algorithm(graph, upper_bound_function, 
                                                      init=0):
    if not isinstance(graph, NX.Graph):
        raise Exception, "Not a Graph!"
    upper_bound_opt = init
    while True:
        upper_bounds = get_upper_bounds(graph, upper_bound_function)
        node, induced_upper_bound = get_min_upper_bound(upper_bounds)
        if upper_bound_opt < induced_upper_bound:
            upper_bound_opt = induced_upper_bound 
        ignore, max_upper_bound = get_max_upper_bound(upper_bounds)
        if upper_bound_opt < max_upper_bound:
            graph.remove_node(node)
        else:
            return upper_bound_opt

"""
upperbound = massimo autovalore della matrice di adiacenza 
del grafo passato come parametro + 1
"""
def upper_bound_from_adjacency_matrix(graph):
    if not isinstance(graph, NX.Graph):
        raise Exception, "Not a Graph!"
    matrix = NX.adj_matrix(graph)
    eigenvalues = NU.linalg.eigvals(matrix)
    max_eigenvalue = int(max(eigenvalues))
    return max_eigenvalue + 1
