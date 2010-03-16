# -*- coding: utf-8 -*-

from graphStructure import Graph
from UserDict import UserDict
from usefulFunctions import unique
from copy import deepcopy

"""
classe che implementa attraverso un dizionario, una struttura per gestire tutti
i dati necessari per l'esecuzione dell'algoritmo DSATUR;
in particolare ogni elemento del dizionario avra come chiave il nodo e come valori
il grado del nodo nel sottografo non colorato o -1 se il nodo è stato colorato,
il grado di saturazione ed infine il colore assegnato al nodo o -1 se ancora il 
nodo non è stato colorato.
"""
class DSaturClass(UserDict):
    def __init__(self, graph):
        UserDict.__init__(self)
        self.graph = graph
        self.uncolored_graph = graph.dcopy()
        nodes = graph.nodes()
        for node in nodes:
            #dizionario - data[node] = [grado nel grafo non colorato, grado di 
            #saturazione, colore]; i valori di default sono per ogni nodo
            #rispettivamente: il grado nel grafo originale, 0, None.
            self.data[node] = [len(graph[node]), 0, None]
    
    # aggiorniamo
    def update(self, node_sel):
        data = self.data
        graph = self.graph
        ugraph = self.uncolored_graph
        # aggiorniamo il grafo dei nodi non colorati
        ugraph.remove_node(node_sel)
        # aggiorniamo il grado di saturazione
        for node in graph[node_sel]:
            colors_list = unique([data[adj_node][2] for adj_node in graph[node] 
                                  if not data[adj_node][2] is None])
            data[node][1] = len(colors_list)
        # aggiorniamo il grado dei nodi non colorati
        data[node_sel][0] = None
        unodes = ugraph.nodes()
        for node in unodes:
            data[node][0] = len(ugraph[node]) 
        
    """
    metodo che determina il colore e colora il nodo selezionato
    """
    def color(self, node):
        data = self.data
        graph = self.graph
        colors_list = unique([data[adj_node][2] for adj_node in graph[node] 
                             if not data[adj_node][2] is None])
        length = len(colors_list)
        if length == 0:
            data[node][2] = 0
            return 0
        colors_list.sort()
        for i in range(length):
            if i != colors_list[i]:
                data[node][2] = i
                return data[node][2]
        data[node][2] = i + 1
        return data[node][2]
    
    """
    metodo che ritorna il nodo con il massimo grado nel sottografo dei nodi non
    colorati
    """
    def get_node_max_degree(self):
        items = self.data.items()
        item = max(items, key=lambda x: x[1][0])
        return item[0]
    
    """
    metodo che ritorna il nodo con il massimo grado di saturazione
    """
    def get_node_max_satur_degree(self):
        unodes = set(self.uncolored_graph.nodes())
        items = self.data.items()
        list = [item for item in items if item[0] in unodes]
        list.sort(key=lambda x: x[1][1], reverse=True)
        node_item_0 = list[0]
        node_item_1 = list[1]
        if node_item_0[1][1] == node_item_1[1][1]:
            return node_item_0[0], True
        else:
            return node_item_0[0], False

"""
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
def dsatur_algorithm(graph):
    #inizializzazione
    dsatur = DSaturClass(graph)
    colors_list = []
    append_color = colors_list.append
    #estriamo il nodo con grado maggiore
    node_sel = dsatur.get_node_max_degree()
    #coloriamo il nodo e aggiorniamo 
    ncolor = dsatur.color(node_sel)
    #color_list: lista in cui verrano memorizzati i colori con cui verranno 
    #colorati i nodi
    append_color(ncolor)
    iterations = len(dsatur.uncolored_graph) - 1 
    #si itera su tutti i nodi rimasti da colorare
    for i in xrange(iterations):
        #si trova il nodo di grado di saturazione minore
        node_sel, check_eq = dsatur.get_node_max_satur_degree()
        if check_eq is False:
            #se vi è un solo nodo con grado di saturazione massimo
            ncolor = dsatur.color(node_sel)
            append_color(ncolor)
            dsatur.update(node_sel)
        else:
            #se vi è un conflitto si estrae un qualunque nodo di grado maggiore
            #nel sottografo di nodi non ancora colorati
            node_sel = dsatur.get_node_max_degree()
            ncolor = dsatur.color(node_sel)
            append_color(ncolor)
            dsatur.update(node_sel)
    node_sel = dsatur.uncolored_graph.nodes()[0]
    ncolor = dsatur.color(node_sel)
    append_color(ncolor)
    dsatur.update(node_sel)
    return len(unique(colors_list))
