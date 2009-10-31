# -*- coding: utf-8 -*-

import sys
import numpy as NU
import networkx as NX

"""
upperbound = numeri di nodi del grafo passato come parametro
"""
def upper_bound_from_number_of_nodes(graph):
    if isinstance(graph, NX.Graph):
        return len(graph)
    else:
        raise Error, "Not a Graph!"

"""
upperbound = massimo autovalore della matrice di adiacenza 
del grafo passato come parametro + 1
"""
def upper_bound_from_adjacency_matrix(graph):
    if isinstance(graph, NX.Graph):
        matrix = NX.adj_matrix(graph)
        print matrix
        eigenvalues = NU.linalg.eigvals(matrix)
        max_eigenvalue = eigenvalues.argmax()
        print max_eigenvalue
        return max_eigenvalue + 1
    else:
        raise Error, "Not a Graph!"
