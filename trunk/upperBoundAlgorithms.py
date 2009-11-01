# -*- coding: utf-8 -*-
"""
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
        raise Error, "Not a Graph!"
    return len(graph)

"""
upperbound = massimo numero dei nodi che compongono il closed 
neighborhood di ogni nodo del grafo passato come parametro
"""
def upper_bound_from_largest_closed_neighborhood(graph):
    if not isinstance(graph, NX.Graph):
        raise Error, "Not a Graph!"
    len_neighborhood_list = []
    for node in graph:
        len_neighborhood_list.append(len(graph[node]) + 1)
    return max(len_neighborhood_list)

"""
upperbound = valore ricavato dall'algoritmo di eliminazione sequanziale
NB: la funzione di default per calcolare l'upper bound è
upper_bound_from_number_of_nodes
"""
def upper_bound_from_sequential_elimination_algorithm(graph):
    if not isinstance(graph, NX.Graph):
        raise Error, "Not a Graph!"
    upper_bound_function = upper_bound_from_number_of_nodes
    graph_cur = NX.Graph(data=graph)
    upper_bound_opt = 0
    upper_bounds = get_upper_bounds(graph_cur, upper_bound_function)
    ignore, max_upper_bound = get_max_upper_bound(upper_bounds)
    while upper_bound_opt < max_upper_bound:
        node, ignore = get_min_upper_bound(upper_bounds)
        node_closed_neighborhood = get_node_closed_neighborhood(graph_cur, node)
        induced_upper_bound = upper_bound_function(graph.subgraph(
                                                node_closed_neighborhood))
        if upper_bound_opt < induced_upper_bound:
            upper_bound_opt = induced_upper_bound
        remaing_nodes = graph_cur.remove_node(node)
        graph_cur = graph_cur.subgraph(remaing_nodes)
        upper_bounds = get_upper_bounds(graph_cur, upper_bound_function)
        ignore, max_upper_bound = get_max_upper_bound(upper_bounds)
    return upper_bound_opt

"""
FIXME: non funge :-(
upperbound = massimo autovalore della matrice di adiacenza 
del grafo passato come parametro + 1
"""
def upper_bound_from_adjacency_matrix(graph):
    if isinstance(graph, NX.Graph):
        matrix = NX.adj_matrix(graph)
        eigenvalues = NU.linalg.eigvals(matrix)
        max_eigenvalue = max(eigenvalues)
        return max_eigenvalue + 1
    else:
        raise Error, "Not a Graph!"
