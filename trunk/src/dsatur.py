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
            #dizionario - data[node] = [grado nel grafo non colorato, grado di 
            #saturazione, colore]; i valori di default sono per ogni nodo
            #rispettivamente: il grado nel grafo originale, 0, -1.
            self.data[node] = [graph.degree(node), 0, -1]
    
    def update_uncolored_graph(self, node_sel):
        if node_sel in self.uncolored_graph.nodes():
            self.uncolored_graph.remove_node(node_sel)
            
    def update_degree_satur(self, node_sel):
        for node in self.graph[node_sel]:
            colors_list = []
            for adj_node in self.graph[node]:
                col = self.data[adj_node][2]
                if col != -1:
                    if col not in colors_list:
                        colors_list.append(col)
            self.data[node][1] = len(colors_list)
    
    def update_degree(self, node_sel):
        print "--------------------------"
        print "node sel: ", node_sel
        self.data[node_sel][0] = -1
        for node in self.uncolored_graph.nodes():
            self.data[node][0] = self.uncolored_graph.degree(node)
            print "node: " + str(node) + " degree: " + str(self.data[node][0])
    
    def get_min_color_possible(self, node_sel):
        colors_list = []
        for node in self.graph[node_sel]:
            col = self.data[node][2]
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
    
    def color(self, node):
        min_color_possible = self.get_min_color_possible(node)
        self.data[node][2] = min_color_possible
    
    def get_sorted_nodes(self):
        list = sorted(self.data.items(), key=lambda x: int(x[0]))
        return list
             
    def get_sorted_degree_satur(self):
        list = sorted(self.data.items(), key=lambda x: x[1][1], reverse=True)
        return list
    
    def get_sorted_degree(self):
        list = sorted(self.data.items(), key=lambda x: x[1][0], reverse=True)
        return list
    
    def get_sorted_color(self):
        list = sorted(self.data.items(), key=lambda x: x[1][2], reverse=True)
        return list
    
    def get_node_max_degree(self):
        list = self.get_sorted_degree()
        node = list[0]
        return node[0]
    
    def get_node_max_satur_degre(self):
        list = self.get_sorted_degree_satur()
        node = list[0]
        check_eq = self.check_eq_max_satur_degree(list)
        return node[0], check_eq
        
    def check_eq_max_satur_degree(self, list):
        node_0 = list[0]
        node_1 = list[1]
        if node_0[1][1] == node_1[1][1]:
            return True
        else:
            return False
        
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
        node_sel, check_eq = nodes.get_node_max_satur_degre()
        if check_eq is False:
            #se vi è un solo nodo con grado di saturazione massimo
            nodes.color(node_sel)
            nodes.update_degree_satur(node_sel)
            nodes.update_uncolored_graph(node_sel)
            nodes.update_degree(node_sel)
        else:
            #se vi è un conflitto si estrae un qualunque nodo di grado maggiore
            #nel sottografo di nodi non ancora colorati
            node_sel = nodes.get_node_max_degree()
            nodes.color(node_sel)
            nodes.update_degree_satur(node_sel)
            nodes.update_uncolored_graph(node_sel)
            nodes.update_degree(node_sel)
        i += 1
    list = nodes.get_sorted_nodes()
    colors_list = []
    for item in list:
        print item[0] + ": " + str(item[1][2])
        col = item[1][2]
        if col not in colors_list:
            colors_list.append(col)
    return len(colors_list)
