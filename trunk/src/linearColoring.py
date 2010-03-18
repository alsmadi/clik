# -*- coding: utf-8 -*-

import usefulFunctions

"""
classe che implementa attraverso un dizionario, una struttura per gestire tutti
i dati necessari per l'esecuzione dell'algoritmo;
in particolare ogni elemento del dizionario avra come chiave il nodo e come valori
il grado del nodo nel sottografo non colorato o None se il nodo è stato colorato,
il grado di saturazione ed infine il colore assegnato al nodo o -1 se ancora il
nodo non è stato colorato.
"""
class LinearColoringClass(object):
    def __init__(self, graph):
        self.data = {}
        self.graph = graph
        nodes = graph.nodes()
        self.colors = set()
        for node in nodes:
            self.data[node] = None

    def color(self):
        data = self.data
        graph = self.graph
        unique = usefulFunctions.unique
        colors_add = self.colors.add
        nodes = graph.nodes()
        for node in nodes:
            adj_colors = unique([data[adj_node] for adj_node in graph[node]
                                if data[adj_node] is not None])
            length = len(adj_colors)
            adj_colors.sort()
            if length == 0:
                data[node] = 0
            else:
                if length != adj_colors[length - 1]:
                    for i in xrange(length):
                        if i != adj_colors[i]:
                            data[node] = i
                else:
                    data[node] = length
            colors_add(data[node])
        return len(self.colors)
