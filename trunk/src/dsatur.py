# -*- coding: utf-8 -*-

from graphStructure import Graph
import operator
from UserDict import UserDict

class NodeDict(UserDict):
    def __init__(self, graph):
        UserDict.__init__(self)
        self.graph = graph
        self.uncolored_graph = Graph(graph.edges())
        for node in graph.nodes():
            self.data[node] = [graph.degree(node), 0, 0]
    
    def update_uncolored_graph(self, node_selected):
        self.uncolored_graph.remove(node_selected)
            
    def update_degree_satur(self, node):
        for adj_node in self.graph[node]:
            self.data[adj_node][1] += 1
    
    def update_degree(self, node_del):
        self.data[node_del][0] = -1
        for node in self.uncolored_graph.nodes():
            self.data[node][0] = self.uncolored_graph.degree(node)
    
    def get_min_color_possible(self):
        list = self.get_sorted_color()
        return list[0][3] + 1
    
    def color(self, node):
        min_color_possible = self.get_min_color_possible()
        self.data[node][2] = min_color_possible
        self.update_uncolored_graph(node)
        self.update_degree(node)
        return min_color_possible
            
    def get_sorted_degree_satur(self):
        list = sorted(self.data.items(), key=operator.itemgetter(2), reverse=True)
        return list
    
    def get_sorted_degree(self):
        list = sorted(self.data.items(), key=operator.itemgetter(1), reverse=True)
        return list
    
    def get_sorted_color(self):
        list = sorted(self.data.items(), key=operator.itemgetter(3), reverse=True)
        return list
    
    def get_node_max_degree(self):
        list = self.get_sorted_degree()
        return list[0][0]
    
    def get_node_max_satur_degre(self):
        list = self.get_sorted_degree_satur()
        check = self.check_max_satur_degree(list)
        return list[0][0], check
        
    def check_max_satur_degree(self, list):
        if list[0][2] == list[1][2]:
            return False
        else:
            return True
        
def dsatur_algorithm(graph):
    """
    1 - ordinare i nodi per grado
    2 - colorare il nodo con grado maggiore con colore 1
    3 - scegliere il nodo con il massimo grado di saturazione; 
        se vi sono più nodi con lo stesso grado di saturazione -> scegliere 
        un qualunque nodo con grado maggiore tra quelli non colorati
    4 - colorare il nodo scelto con il colore di numero più basso
    5 - se ultimo nodo colorato -> STOP, altrimenti torna a 3 
    """
    #inizializzazione
    nodes = NodeDict(graph)
    #estriamo il nodo con grado maggiore
    node_sel = nodes.get_node_max_degree()
    #coloriamo il nodo e aggiorniamo 
    nodes.color(node_sel)
    iterations = len(nodes.uncolored_graph)
    i = 0
    #si itera su tutti i nodi rimasti da colorare
    while i < iterations:
        #si trova il nodo di grado di saturazione minore
        node_max_satur_degree, check = nodes.get_node_max_satur_degre()
        if check is True:
            #se vi è un solo nodo con grado di saturazione massimo
            cur_color = nodes.color(node_max_satur_degree)
        else:
            #se vi è un conflitto si estra un qualunque nodo di grado maggiore
            #nel sottografo di nodi non ancora colorati
            node_max_degree = nodes.get_node_max_degree()
            cur_color = nodes.color(node_max_degree)
        i += 1
    print "color: ", cur_color
    return cur_color