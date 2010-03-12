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
        comp = lambda x: (x[0], x[1]) if int(x[0]) < int(x[1]) else (x[1], x[0])
        edges = [comp((node_a, node_b)) for node_a in self.nodes() 
                      for node_b in self.data[node_a]] 
        edges = list(frozenset(edges))
        return edges
    
    def number_of_edges(self):
        i = 0
        for node in self.nodes():
            i += len(self.data[node])
        return i/2
    
    def add_node(self, node):
        self.data[node] = []
        
    def add_edge(self, edge):
        #---------------------------------------------------- if len(edge) != 2:
            #------------------------------------------------------------ return
        node_a = edge[0]
        node_b = edge[1]
        if not node_a in self.nodes():
            self.data[node_a] = []
        if not node_b in self.nodes():
            self.data[node_b] = []
        if not node_b in self.data[node_a]:
            self.data[node_a].append(node_b)
            self.data[node_b].append(node_a)
    
    def add_nodes(self, nodes):
        for node in nodes:
            self.data[node] = []
          
    def add_edges(self, edges_list):
        for edges in edges_list:
            #----------------------------------------------- if len(edges) != 2:
                #-------------------------------------------------------- return
            node_a = edges[0]
            node_b = edges[1]
            if not node_a in self.nodes():
                self.data[node_a] = []
            if not node_b in self.nodes():
                self.data[node_b] = []
            if not node_b in self.data[node_a]:
                self.data[node_a].append(node_b)
                self.data[node_b].append(node_a)
    
    def remove_node(self, node):
        for adjacent_node in self.data[node]:
            self.data[adjacent_node].remove(node)
        del self.data[node]
    
    def remove_nodes(self, nodes):
        for node in nodes:
            self.remove_node(node)
            
    def remove_edge(self, edge):
        #---------------------------------------------------- if len(edge) != 2:
            #------------------------------------------------------------ return
        node_a = edge[0]
        node_b = edge[1]
        if node_a not in self.nodes() or node_b not in self.nodes():
            return
        if node_b in self.data[node_a]:
            self.data[node_a].remove(node_b)
            self.data[node_b].remove(node_a)

    def closed_neighborhood(self, node):
        try:
            neighborhood = deepcopy(self.data[node])
            neighborhood.append(node)
            return neighborhood
        except:
            print "Error: Node not present!"
    
    def subgraph(self, nodes_list):
        #-------------------------------------------- subgraph_ = deepcopy(self)
        # for node in [node_rem for node_rem in subgraph_.nodes() if node_rem not in nodes_list]:
            #--------------------------------------- subgraph_.remove_node(node)
        #------------------------------------------------------ return subgraph_
        edges = [(node_1, node_2) for node_1 in nodes_list 
                 for node_2 in nodes_list if node_2 in self.data[node_1]]
        return Graph(edges)
    
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