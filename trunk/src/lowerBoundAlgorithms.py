# -*- coding: utf-8 -*-

import operator
#---------------------------------------------- from graphStructure import Graph

def lower_bound_from_MIN(graph):
    graph_cur = graph
    node_max_list = []
    neighborhood_list = [(node, len(graph_cur[node])) for node in graph_cur.nodes()]
    while len(neighborhood_list) != 0:
        node_max = max(neighborhood_list, key=operator.itemgetter(1))
        node_max_list.append(node_max[0])
        graph_cur = graph_cur.subgraph(graph_cur.closed_neighborhood(node_max[0]))
        neighborhood_list = [(node, len(graph_cur[node])) for node in graph_cur.nodes() 
                             if not node in node_max_list]
    return len(node_max_list)