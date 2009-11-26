# -*- coding: utf-8 -*-

from UserDict import UserDict

class NodeDict(UserDict):
    def __init__(self, graph):
        UserDict.__init__(self)
        self.graph = graph
        for node in graph.nodes():
            #dizionario - data[node] =  [colore del nodo o -1 se il nodo non è 
            #stato ancora colorato]
            self.data[node] = None
 
    """
    metodo che trova il minor colore possibile da assegnare ad un nodo selezionato.
    itera sui nodi adiacenti e ritorna il primo colore diposnibile tra quelli 
    non usati
    """    
    def get_min_color_possible(self, node_sel):
        colors_list = [self.data[node] for node in self.graph[node_sel] if self.data[node] != None]
        colors_list = list(frozenset(colors_list))
        if len(colors_list) == 0:
            return 0
        colors_list.sort()
        maxcolor = colors_list[len(colors_list) - 1]
        for i in range(len(colors_list)):
            if i != colors_list[i]:
                return i
        return maxcolor + 1
  
    """
    metodo che colora il nodo selezionato
    """
    def color(self, node):
        self.data[node] = self.get_min_color_possible(node)
        return self.data[node]

"""
implementazione dell'algoritmo Linear Coloring.
ad ogni nodo del grafo, dato nell'ordine memorizzato, assegniamo il minimo 
colore possibile
"""        
def linear_coloring_algorithm(graph):
    #inizializzazione
    nodes = NodeDict(graph)
    colors_list = [nodes.color(node) for node in graph.nodes()]
    return len(list(frozenset(colors_list)))
