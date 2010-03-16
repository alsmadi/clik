# -*- coding: utf-8 -*-

import operator
"""
algoritmo greedy per determinare il lower bound del problema della massima
clique
"""
def lower_bound_from_MIN(graph):
    graph_cur = graph
    nodes = graph_cur.nodes()
    node_max_list = []
    node_max_list_append = node_max_list.append
    neighborhood_list = [(node, len(graph_cur[node])) for node in nodes]
    while len(neighborhood_list) != 0:
        node_max = max(neighborhood_list, key=operator.itemgetter(1))
        node_max_list_append(node_max[0])
        node_max_set = set(node_max_list)
        graph_cur = graph_cur.subgraph(graph_cur.closed_neighborhood(node_max[0]))
        nodes = graph_cur.nodes()
        neighborhood_list = [(node, len(graph_cur[node])) for node in nodes 
                             if not node in node_max_set]
    return len(node_max_list)