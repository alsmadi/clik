# -*- coding: utf-8 -*-

from UserDict import UserDict
import sys
import operator
import numpy as NU

class Graph(UserDict):
    def __init__(self):
        UserDict.__init__(self)

    def nodes(self):
        return self.keys()
    
    def edges(self):
        edges = []
        for node in self.nodes():
            node_a = node
            for node_b in self.data[node]:
                edge = (node_a, node_b)
                if edge not in edges:
                    edge_rev = (node_b, node_a)
                    if edge_rev not in edges:
                        edges.append(edge)
        return edges 
    
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
            self.add_node(node)
          
    def add_edges(self, edges):
        for edge in edges:
            self.add_edge(edge)
    
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
        neighborhood = self.data[node]
        closed_neighborhood = neighborhood.append(node)
        return closed_neighborhood
    
    def subgraph(self, nodes):
        subgraph_ = Graph()
        subgraph_.add_nodes(nodes)
        rem_nodes = nodes[:]
        for node_a in nodes:
             rem_nodes.remove(node_a)
             for node_b in rem_nodes:
                 if node_b in self.data[node_a]:
                     subgraph_.add_edge((node_a, node_b))
        return subgraph_       
    
    def adjacency_matrix(self):
        matrix = [[0 for col in range(len(self.nodes()))] 
                  for row in range(len(self.nodes()))]
        i = 0
        for node_a in self.nodes():
            j = 0
            for node_b in self.nodes():
                if node_b in self.data[node_a]:
                    if matrix[i][j] == 0:
                        matrix[i][j] = 1
                j += 1
            i +=1
        return matrix
    
    def is_complete(self):
        n = len(self.nodes())
        if len(self.edges()) != (n * (n - 1) / 2) :
            return False
        return True