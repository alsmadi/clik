# -*- coding: utf-8 -*-

from UserDict import UserDict

def unique(seq):
    keys = {}
    for e in seq:
        if not e is None: 
            keys[e] = 1
    return keys.keys()

class NodeDict(UserDict):
    def __init__(self, graph):
        UserDict.__init__(self)
        self.graph = graph
        for node in graph.nodes():
            #dizionario - data[node] =  [colore del nodo o None se il nodo non è 
            #stato ancora colorato]
            self.data[node] = None
 
    """
    metodo che trova il minor colore possibile da assegnare ad un nodo selezionato.
    itera sui nodi adiacenti e ritorna il primo colore disponibile tra quelli 
    non usati
    """    
    def get_min_color_possible(self, node_sel):
        adj_colors = unique([self.data[node] for node in self.graph[node_sel]])
        if len(adj_colors) == 0:
            return 0
        adj_colors.sort()
        for i in xrange(len(adj_colors)):
            if i != adj_colors[i]:
                return i
        return  i + 1
  
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
    clr_nodes = NodeDict(graph)
    nodes = clr_nodes.graph.nodes()
    color = clr_nodes.color
    return len(unique([color(node) for node in nodes]))
