# -*- coding: utf-8 -*-

from UserDict import UserDict
from copy import deepcopy

class Graph(UserDict):
    def __init__(self, edges=None):
        UserDict.__init__(self)
        self.nodes = self.keys
        if edges != None:
            self.add_edges(edges)
    
    def __deepcopy__(self, memo):
        graph = Graph()
        graph.data = deepcopy(self.data, memo)
        return graph
    
    def edges(self):
        def comp(x):
            if int(x[0]) < int(x[1]):
                return (x[0], x[1])
            else:
                return (x[1], x[0]) 
        edges = [comp((node_a, node_b)) for node_a in self.nodes() 
                 for node_b in self.data[node_a]] 
        edges = list(set(edges))
        return edges
    
    def number_of_edges(self):
        i = sum([len(self.data[node]) for node in self.nodes()])
        return i/2
    
    def add_node(self, node):
        self.data.setdefault(node, [])
        
    def add_edge(self, edge):
        self.data.setdefault(edge[0],[])
        self.data.setdefault(edge[1],[])
        if not edge[1] in self.data[edge[0]]:
            self.data[edge[0]].append(edge[1])
            self.data[edge[1]].append(edge[0])

    def add_nodes(self, nodes):
        map(self.add_node, nodes)
          
    def add_edges(self, edges_list):
        [self.add_edge(edge) for edge in edges_list]
    
    def remove_node(self, node):
        [self.data[adj_node].remove(node) for adj_node in self.data[node]]
        del self.data[node]
    
    def remove_nodes(self, nodes):
        map(self.remove_node, nodes)
            
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
            print "Error: Node not present!"
    
    def subgraph(self, nodes_list):
        subgraph = Graph()
        for key in nodes_list:
            subgraph.data[key] = [n for n in self.data[key] if n in nodes_list]
#        [subgraph.data.setdefault(key, [n for n in self.data[key] if n in nodes_list]) 
#                                        for key in nodes_list]
        return subgraph
    
    def degree(self, node):
        return len(self.data[node])
    
    def adjacency_matrix(self):
        def f(x):
            if x[1] in self.data[x[0]]:
                return 1
            else:
                return 0
        matrix = [[f((node_a, node_b)) for node_b in self.nodes()] 
                  for node_a in self.nodes()]
        return matrix
    
    def is_complete(self):
        n = len(self.nodes())
        if self.number_of_edges() != (n * (n - 1) / 2) :
            return False
        else:
            return True