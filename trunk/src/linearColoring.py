# -*- coding: utf-8 -*-

from UserDict import UserDict

class NodeDict(UserDict):
    def __init__(self, graph):
        UserDict.__init__(self)
        self.graph = graph
        for node in graph.nodes():
            #dizionario - data[node] =  [colore del nodo o -1 se il nodo non è 
            #stato ancora colorato]
            self.data[node] = [-1]
 
    """
    metodo che trova il minor colore possibile da assegnare ad un nodo selezionato.
    itera sui nodi adiacenti e ritorna il primo colore diposnibile tra quelli 
    non usati
    """    
    def get_min_color_possible(self, node_sel):
        colors_list = []
        for node in self.graph[node_sel]:
            col = self.data[node]
            if col != -1:
                if col not in colors_list:
                    colors_list.append(col)
        if len(colors_list) == 0:
            return 0
        colors_list.sort()
        maxcolor = colors_list[len(colors_list) - 1]
        for i in range(len(colors_list)):
            if i != colors_list[i]:
                return i
        return maxcolor + 1
#    
    """
    metodo che colora il nodo selezionato
    """
    def color(self, node):
        min_color_possible = self.get_min_color_possible(node)
        self.data[node] = min_color_possible
        return min_color_possible

"""
implementazione dell'algoritmo Linear Coloring.
ad ogni nodo del grafo, dato nell'ordine memorizzato, assegniamo il minimo 
colore possibile
"""        
def linear_coloring_algorithm(graph):
    #inizializzazione
    nodes = NodeDict(graph)
    colors_list = []
    for node in graph.nodes():
        ncolor = nodes.color(node)
        if ncolor not in colors_list:
            colors_list.append(ncolor)
    return len(colors_list)
