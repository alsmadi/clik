# -*- coding: utf-8 -*-

import sys
import numpy as NU
import networkx as NX

"""
funzione usata per calcolare il min/max della lista di upperbounds
"""
def upper_bounds_compare(x):
    return x[1]

"""
determina una lista di tuple (nodo, upperbound) con upperbound cacolato 
dal grafo indotto dal closed neighborhood di nodo, mediante la funzione 
passata come secondo parametro   
"""
def get_upper_bounds(graph, upper_bound_function):
    upper_bounds = []
    for node in graph:
        node_closed_neighborhood = graph.neighbors(node)
        node_closed_neighborhood.append(node)
        subgraph = graph.subgraph(node_closed_neighborhood)
        upper_bound = upper_bound_function(subgraph)
        upper_bounds.append((node, upper_bound))
    return upper_bounds

"""
seleziona il nodo con l'upperbound di valore minore da una lista 
di tuple (nodo, upperbound)
"""
def select_node(upper_bounds):
    min_ = min(upper_bounds, key=upper_bounds_compare)
    return min_[0]
    
"""
seleziona il nodo con l'upperbound di valore maggiore da una lista 
di tuple (nodo, upperbound) e ritorna il valore dell'upperbound
"""
def get_max_upper_bound(upper_bounds):
    max_ = max(upper_bounds, key=upper_bounds_compare)
    return max_[1]

def get_node_closed_neighborhood(graph, node):
    node_neighborhood = graph.neighbors(node)
    node_neighborhood.append(node)
    return node_neighborhood

"""
algoritmo di eliminazione sequenziale  
"""
def sequential_elimination_algorithm(graph, upper_bound_function):
    #varie inizializzazioni
    graph_cur = NX.Graph(data=graph)
    graph_opt = NX.Graph(data=graph)
    upper_bound_opt = 0
    #determiniamo la lista degli upperbounds
    upper_bounds = get_upper_bounds(graph_cur, upper_bound_function)
    max_upper_bound = get_max_upper_bound(upper_bounds)
    print "max0: ", max_upper_bound 
    print "opt0: ", upper_bound_opt
    while upper_bound_opt < max_upper_bound:
        #selezioniamo il nodo associato al minor upperbound
        node = select_node(upper_bounds)
        print "node: ", node
        #determiniamo il closed neighborhood del nodo selezionato
        node_closed_neighborhood = get_node_closed_neighborhood(graph_cur, node)
        induced_upper_bound = upper_bound_function(graph.subgraph(
                                                node_closed_neighborhood))
        print "opt: ", upper_bound_opt
        print "induced: ", induced_upper_bound
        if upper_bound_opt < induced_upper_bound:
            upper_bound_opt = induced_upper_bound
            graph_opt = NX.Graph(data=graph_cur)
            print "optNew: ", upper_bound_opt
            print graph_opt.nodes()
            print graph_opt.edges()
        #bisogna aggiornare i dati utili nelle iterazioni
        remaing_nodes = graph_cur.remove_node(node)
        graph_cur = graph_cur.subgraph(remaing_nodes)
        upper_bounds = get_upper_bounds(graph_cur, upper_bound_function)
        max_upper_bound = get_max_upper_bound(upper_bounds)
        print "max: ", max_upper_bound
        print "--------------------------------"
    return upper_bound_opt, graph_opt
