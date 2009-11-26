# -*- coding: utf-8 -*-

from UserDict import UserDict
from copy import deepcopy

class Graph(UserDict):
    def __init__(self, edges=None):
        UserDict.__init__(self)
        if edges != None:
            self.add_edges(edges)

    def nodes(self):
        return self.keys()
    
    def edges(self):
        comp = lambda x: (x[0], x[1]) if int(x[0]) < int(x[1]) else (x[1], x[0])
        edges = [comp((node_a, node_b)) for node_a in self.nodes() 
                      for node_b in self.data[node_a]] 
        edges = frozenset(edges)
        return edges
    
    def number_of_edges(self):
        i = 0
        for node in self.nodes():
            i += len(self.data[node])
        return i/2
    
    def add_node(self, node):
        if node not in self.nodes(): 
            self.data[node] = []
        
    def add_edge(self, edge):
        if len(edge) != 2:
            return
        node_a = edge[0]
        node_b = edge[1]
        self.add_node(node_a)
        self.add_node(node_b)
        if node_b not in self.data[node_a]:
            self.data[node_a].append(node_b)
            self.data[node_b].append(node_a)
    
    def add_nodes(self, nodes):
        for node in nodes:
            self.data[node] = []
          
    def add_edges(self, edges):
        for edge in edges:
            if len(edge) != 2:
                return
            node_a = edge[0]
            node_b = edge[1]
            self.add_node(node_a)
            self.add_node(node_b)
            if node_b not in self.data[node_a]:
                self.data[node_a].append(node_b)
                self.data[node_b].append(node_a)
    
    def remove_node(self, node):
        neighborhood = self.data[node]
        del self.data[node]
        for adjacent_node in neighborhood:
            self.data[adjacent_node].remove(node)
    
    def remove_edge(self, edge):
        if len(edge) != 2:
            return
        node_a = edge[0]
        node_b = edge[1]
        if node_a not in self.nodes():
            return
        if node_b not in self.nodes():
            return
        if node_b in self.data[node_a]:
            self.data[node_a].remove(node_b)
            self.data[node_b].remove(node_a)
           
    def closed_neighborhood(self, node):
        if node not in self.data:
            return
        neighborhood = deepcopy(self.data[node])
        neighborhood.append(node)
        return neighborhood
    
    def subgraph(self, nodes):
        subgraph_ = Graph()
        for node_a in nodes:
            subgraph_.add_node(node_a)
            for node_b in self.data[node_a]:
                if node_b in nodes:
                    subgraph_.add_node(node_b)
                    if node_b not in subgraph_[node_a]:
                        subgraph_[node_a].append(node_b)
                        subgraph_[node_b].append(node_a)
        return subgraph_       
    
    def degree(self, node):
        return len(self.data[node])
    
    def adjacency_matrix(self):
        f = lambda x: 1 if x[1] in self.data[x[0]] else 0
        matrix = [[f((node_a, node_b)) for node_b in self.nodes()] 
                  for node_a in self.nodes()]
        return matrix
    
    def is_complete(self):
        n = len(self.nodes())
        if self.number_of_edges() != (n * (n - 1) / 2) :
            return False
        return True