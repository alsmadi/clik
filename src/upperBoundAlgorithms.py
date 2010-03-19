# -*- coding: utf-8 -*-

"""
questo file contiene le definizioni delle funzioni che calcolano gli upperbound.
"""

import operator
import dsatur
import usefulFunctions
#from numpy import linalg

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
def upper_bound_from_cardinality(graph, lower_bound):
    nodes = graph.nodes()
    get_induced_subgraph = graph.induced_subgraph
    upper_bounds = []
    upper_bounds_append = upper_bounds.append
    for node in nodes:
        subgraph = get_induced_subgraph(node)
        sub_nodes = subgraph.nodes()
        size_neighborhood_list = [(len(subgraph[sub_node]) + 1) 
                                  for sub_node in sub_nodes]
        size_neighborhood_list.sort(reverse=True)
        length = len(size_neighborhood_list) + 1
        for i in xrange(length):
            if size_neighborhood_list[i-1] <= i:
                ub = min(i, size_neighborhood_list[i-2])
                break
        upper_bounds_append((node, ub, subgraph))
    return upper_bounds 

"""
upperbound =
"""
def upper_bound_from_cardinality_2(graph):
    nodes = graph.nodes()
    get_induced_subgraph = graph.induced_subgraph
    upper_bounds = []
    upper_bounds_append = upper_bounds.append
    for node in nodes:
        subgraph = get_induced_subgraph(node)
        sub_nodes = subgraph.nodes()
        size_neighborhood_list = [(len(subgraph[sub_node]) + 1) 
                                  for sub_node in sub_nodes]
        size_neighborhood_list.sort(reverse=True)
        length = len(size_neighborhood_list) + 1
        for i in xrange(length):
            if size_neighborhood_list[i-1] <= i:
                ub = min(i, size_neighborhood_list[i-2])
                break
        upper_bounds_append((node, ub))
    return upper_bounds      
                
"""
upperbound = valore ricavato dall'algoritmo di eliminazione sequanziale
"""
def upper_bound_from_sequential_elimination_algorithm(graph, ub_function, init=0):
    remove_node = graph.remove_node
    getter = operator.itemgetter
    upper_bound_opt = init
    while 1:
        upper_bounds = ub_function(graph)
        upper_bounds.sort(key=getter(1))
        ignore, max_upper_bound = upper_bounds[len(upper_bounds) - 1]
        if upper_bound_opt >= max_upper_bound:
            break
        node, min_upper_bound = upper_bounds[0]
        upper_bound_opt = max(upper_bound_opt, min_upper_bound)
        remove_node(node)
    return upper_bound_opt

"""
upperbound = numero di colori ricavato dall'algoritmo DSATUR
implementazione dell'algoritmo DSATUR.
passi da eseguire:
1 - ordinare i nodi per grado
2 - colorare il nodo con grado maggiore con colore 0
3 - scegliere il nodo con il massimo grado di saturazione;
    se vi sono più nodi con lo stesso grado di saturazione -> scegliere
    un qualunque nodo con grado maggiore tra quelli non colorati
4 - colorare il nodo scelto con il minimo colore possibile
5 - se ultimo nodo colorato -> STOP, altrimenti torna a 3
"""
def upper_bound_from_dsatur(graph):
    #inizializzazione
    unique = usefulFunctions.unique
    nodes = graph.nodes()
    get_induced_subgraph = graph.induced_subgraph
    upper_bounds = []
    upper_bounds_append = upper_bounds.append
    for node in nodes:
        subgraph = get_induced_subgraph(node)
        dst = dsatur.DSaturClass(subgraph)
        dst_color = dst.color
        dst_get_node_max_degree = dst.get_node_max_degree
        dst_update = dst.update
        dst_ungraph = dst.uncolored_graph
        dst_get_node_max_satur_degree = dst.get_node_max_satur_degree
        colors_list = []
        append_color = colors_list.append
        #estriamo il nodo con grado maggiore
        sub_node_sel = dst_get_node_max_degree()
        #coloriamo il nodo e aggiorniamo
        ncolor = dst_color(sub_node_sel)
        #color_list: lista in cui verrano memorizzati i colori con cui verranno
        #colorati i nodi
        append_color(ncolor)
        iterations = len(dst_ungraph) - 1
        #si itera su tutti i nodi rimasti da colorare
        for i in xrange(iterations):
            #si trova il nodo di grado di saturazione minore
            sub_node_sel, check_eq = dst_get_node_max_satur_degree()
            if check_eq is False:
                #se vi è un solo nodo con grado di saturazione massimo
                ncolor = dst_color(sub_node_sel)
                append_color(ncolor)
                dst_update(sub_node_sel)
            else:
                #se vi è un conflitto si estrae un qualunque nodo di grado maggiore
                #nel sottografo di nodi non ancora colorati
                sub_node_sel = dst_get_node_max_degree()
                ncolor = dst_color(sub_node_sel)
                append_color(ncolor)
                dst_update(sub_node_sel)
        sub_node_sel = dst_ungraph.nodes()[0]
        ncolor = dst_color(sub_node_sel)
        append_color(ncolor)
        ub = len(unique(colors_list))
        upper_bounds_append((node, ub, subgraph))
    return upper_bounds

def upper_bound_from_dsatur_2(graph):
    #inizializzazione
    unique = usefulFunctions.unique
    nodes = graph.nodes()
    get_induced_subgraph = graph.induced_subgraph
    upper_bounds = []
    upper_bounds_append = upper_bounds.append
    for node in nodes:
        subgraph = get_induced_subgraph(node)
        dst = dsatur.DSaturClass(subgraph)
        dst_color = dst.color
        dst_get_node_max_degree = dst.get_node_max_degree
        dst_update = dst.update
        dst_ungraph = dst.uncolored_graph
        dst_get_node_max_satur_degree = dst.get_node_max_satur_degree
        colors_list = []
        append_color = colors_list.append
        #estriamo il nodo con grado maggiore
        sub_node_sel = dst_get_node_max_degree()
        #coloriamo il nodo e aggiorniamo
        ncolor = dst_color(sub_node_sel)
        #color_list: lista in cui verrano memorizzati i colori con cui verranno
        #colorati i nodi
        append_color(ncolor)
        iterations = len(dst_ungraph) - 1
        #si itera su tutti i nodi rimasti da colorare
        for i in xrange(iterations):
            #si trova il nodo di grado di saturazione minore
            sub_node_sel, check_eq = dst_get_node_max_satur_degree()
            if check_eq is False:
                #se vi è un solo nodo con grado di saturazione massimo
                ncolor = dst_color(sub_node_sel)
                append_color(ncolor)
                dst_update(sub_node_sel)
            else:
                #se vi è un conflitto si estrae un qualunque nodo di grado maggiore
                #nel sottografo di nodi non ancora colorati
                sub_node_sel = dst_get_node_max_degree()
                ncolor = dst_color(sub_node_sel)
                append_color(ncolor)
                dst_update(sub_node_sel)
        sub_node_sel = dst_ungraph.nodes()[0]
        ncolor = dst_color(sub_node_sel)
        append_color(ncolor)
        ub = len(unique(colors_list))
        upper_bounds_append((node, ub))
    return upper_bounds

"""
upperbound = numero di colori ricavato dall'algoritmo di Linear Coloring
"""
def upper_bound_from_linear_coloring(graph):
    nodes = graph.nodes()
    get_induced_subgraph = graph.induced_subgraph
    upper_bounds = []
    upper_bounds_append = upper_bounds.append
    unique = usefulFunctions.unique
    for node in nodes:
        subgraph = get_induced_subgraph(node)
        data = {}
        colors = set()
        adj_colors = []
        sub_nodes = subgraph.nodes()
        colors_add = colors.add
        for sub_node in sub_nodes:
            adj_colors = unique([data[adj_node] for adj_node in graph[sub_node]
                                if adj_node in data])
            length = len(adj_colors)
            if length == 0:
                data[sub_node] = 0
            else:
                if length != adj_colors[length - 1] + 1:
                    adj_colors.sort()
                    for i in xrange(length):
                        if i != adj_colors[i]:
                            data[sub_node] = i
                            break
                else:
                    data[sub_node] = length
            colors_add(data[sub_node])
        upper_bounds_append((node, len(colors), subgraph))
    return upper_bounds

"""
upperbound = numero di colori ricavato dall'algoritmo di Linear Coloring
"""
def upper_bound_from_linear_coloring_2(graph):
    nodes = graph.nodes()
    get_induced_subgraph = graph.induced_subgraph
    upper_bounds = []
    upper_bounds_append = upper_bounds.append
    unique = usefulFunctions.unique
    for node in nodes:
        subgraph = get_induced_subgraph(node)
        data = {}
        colors = set()
        adj_colors = []
        sub_nodes = subgraph.nodes()
        colors_add = colors.add
        for sub_node in sub_nodes:
            adj_colors = unique([data[adj_node] for adj_node in graph[sub_node]
                                if adj_node in data])
            length = len(adj_colors)
            if length == 0:
                data[sub_node] = 0
            else:
                if length != adj_colors[length - 1] + 1:
                    adj_colors.sort()
                    for i in xrange(length):
                        if i != adj_colors[i]:
                            data[sub_node] = i
                            break
                else:
                    data[sub_node] = length
            colors_add(data[sub_node])
        upper_bounds_append((node, len(colors)))
    return upper_bounds

#"""
#upperbound = massimo autovalore della matrice di adiacenza
#del grafo passato come parametro + 1
#"""
#def upper_bound_from_max_eigenvalue(graph):
#    matrix = graph.adjacency_matrix()
#    eigenvalues = linalg.eigvals(matrix)
#    max_eigenvalue = max(eigenvalues)
#    return round(max_eigenvalue + 1)
