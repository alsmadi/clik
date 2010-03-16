# -*- coding: utf-8 -*-

from UserDict import UserDict
from copy import deepcopy

class Graph(UserDict):
    def __init__(self, edges=None):
        UserDict.__init__(self)
        self.nodes = self.keys
        if not edges is None:
            dict_set = self.data.setdefault
            for edge in edges:
                dict_set(edge[0],[])
                dict_set(edge[1],[])
                if not edge[1] in self.data[edge[0]]:
                    self.data[edge[0]].append(edge[1])
                    self.data[edge[1]].append(edge[0])
    
    def dcopy(self):
        graph = self.__class__()
        graph.data = deepcopy(self.data)
        return graph
    
#    def edges(self):
#        return self._edges
    
    def number_of_edges(self):
        nodes = self.nodes()
        i = sum([len(self.data[node]) for node in nodes])
        return i/2
    
    def add_node(self, node):
        self.data.setdefault(node, [])
        
    def add_edge(self, edge):
        dict_set = self.data.setdefault
        dict_set(edge[0],[])
        dict_set(edge[1],[])
        if not edge[1] in self.data[edge[0]]:
            self.data[edge[0]].append(edge[1])
            self.data[edge[1]].append(edge[0])

    def add_nodes(self, nodes):
        dict_set = self.data.setdefault
        [dict_set(node, []) for node in nodes]
          
    def add_edges(self, edges):
        dict_set = self.data.setdefault
        for edge in edges:
            dict_set(edge[0],[])
            dict_set(edge[1],[])
            if not edge[1] in self.data[edge[0]]:
                self.data[edge[0]].append(edge[1])
                self.data[edge[1]].append(edge[0])
    
    def remove_node(self, node):
        adj_list = self.data[node]
        for adj_node in adj_list:
            self.data[adj_node].remove(node)
        del self.data[node]
    
    def remove_nodes(self, nodes):
        for node in nodes:
            adj_list = self.data[node]
            for adj_node in adj_list:
                self.data[adj_node].remove(node)
            del self.data[node] 
            
    def remove_edge(self, edge):
        try:
            self.data[edge[0]].remove(edge[1])
            self.data[edge[1]].remove(edge[0])
        except:
            print "Errore nella rimozione di un arco!"

    def closed_neighborhood(self, node):
        try:
            neighborhood = list(self.data[node])
            neighborhood.append(node)
            return neighborhood
        except:
            print "Errore: nodo non presente!"
    
    def subgraph(self, nodes):
        subgraph = self.__class__()
        nodes = set(nodes)
        adj_list = self.data
        for node in nodes:
            subgraph.data[node] = [n for n in adj_list[node] if n in nodes]
        return subgraph
    
    def degree(self, node):
        return len(self.data[node])
   
#    def adjacency_matrix(self):
#        edges = set(self.edges())
#        nodes = self.nodes()
#        def f(x):
#            if x in edges:
#                return 1
#            else:
#                return 0
#        matrix = [[f((node_a, node_b)) for node_b in nodes] 
#                  for node_a in nodes]
#        return matrix
    
    def is_complete(self):
        n = len(self.nodes())
        if self.number_of_edges() != (n * (n - 1) / 2) :
            return False
        else:
            return True