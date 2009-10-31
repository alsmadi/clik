# -*- coding: utf-8 -*-

import sys
import numpy as NU
import networkx as NX

def number_nodes_algorithm_ub(graph):
    if isinstance(graph, NX.Graph):
        return len(graph)
    else:
        raise Error, "Not a Graph!"
        
def adjacency_matrix_algorithm_ub(graph):
    if isinstance(graph, NX.Graph):
        matrix = NX.adj_matrix(graph)
        eigenvalues = NU.eigvals(matrix)
        max_eigenvalue = eigenvalues.argmax()
        return max_eigenvalue + 1
    else:
        raise Error, "Not a Graph!"
