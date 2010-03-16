# -*- coding: utf-8 -*-

from graphStructure import Graph
from UserDict import UserDict
from usefulFunctions import unique

"""
classe che implementa attraverso un dizionario, una struttura per gestire tutti
i dati necessari per l'esecuzione dell'algoritmo DSATUR;
in particolare ogni elemento del dizionario avra come chiave il nodo e come valori
il grado del nodo nel sottografo non colorato o -1 se il nodo è stato colorato,
il grado di saturazione ed infine il colore assegnato al nodo o -1 se ancora il 
nodo non è stato colorato.
"""
class LinearColoringClass(UserDict):
    def __init__(self, graph):
        UserDict.__init__(self)
        self.graph = graph
        nodes = graph.nodes()
        for node in nodes:
            self.data[node] = None

    def color(self, node):
        data = self.data
        graph = self.graph
        adj_colors = unique([data[adj_node] for adj_node in graph[node] 
                             if not adj_node is None])
        length = len(adj_colors)
        if length == 0:
            data[node] = 0
            return data[node]
        adj_colors.sort()
        for i in xrange(length):
            if i != adj_colors[i]:
                data[node] = i
                return data[node]
        data[node] = i + 1
        return data[node] 

def linear_coloring_algorithm(graph):
    nodes = graph.nodes()
    lin_color = LinearColoringClass(graph) 
    color = lin_color.color
    return len(unique([color(node) for node in nodes]))
