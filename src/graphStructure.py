# -*- coding: utf-8 -*-

class Graph(object):
    def __init__(self, edges=None):
        self.data = {}
        self.nodes = self.data.keys
        if edges is not None:
            dict_set = self.data.setdefault
            for edge in edges:
                dict_set(edge[0],[])
                dict_set(edge[1],[])
                if edge[1] not in self.data[edge[0]]:
                    self.data[edge[0]].append(edge[1])
                    self.data[edge[1]].append(edge[0])

    def __iter__(self):
        return self.data.iterkeys()

    def __len__(self):
        return len(self.data)

    def __getitem__(self, n):
        return self.data[n]

    def dcopy(self):
        graph = self.__class__()
        for k,v in self.data.iteritems():
            graph.data[k] = v[:]
        return graph

#    def edges(self):
#        return self._edges

    def number_of_edges(self):
        i = sum([len(self.data[node]) for node in self.__iter__()])
        return i/2

    def add_node(self, node):
        self.data.setdefault(node, [])

    def add_edge(self, edge):
        dict_set = self.data.setdefault
        dict_set(edge[0],[])
        dict_set(edge[1],[])
        if edge[1] not in self.data[edge[0]]:
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
            if edge[1] not in self.data[edge[0]]:
                self.data[edge[0]].append(edge[1])
                self.data[edge[1]].append(edge[0])

    def remove_node(self, node):
        adj_list = self.data[node]
        [self.data[adj_node].remove(node) for adj_node in adj_list]
        #for adj_node in adj_list:
            #self.data[adj_node].remove(node)
        del self.data[node]

    def remove_nodes(self, nodes):
        for node in nodes:
            adj_list = self.data[node]
            for adj_node in adj_list:
                self.data[adj_node].remove(node)
            del self.data[node]

    def remove_edge(self, edge):
        self.data[edge[0]].remove(edge[1])
        self.data[edge[1]].remove(edge[0])

    def closed_neighborhood(self, node):
        neighborhood = self.data[node][:]
        neighborhood.append(node)
        return neighborhood

    def subgraph(self, nodes):
        subgraph = self.__class__()
        nodes = set(nodes)
        adj_list = self.data
        sub_data = subgraph.data
        for node in nodes:
            sub_data[node] = [n for n in adj_list[node] if n in nodes]
        return subgraph

    def induced_subgraph(self, node):
        subgraph = self.__class__()
        adj_list = self.data
        nodes = set(adj_list[node])
        nodes.add(node)
        sub_data = subgraph.data
        for node in nodes:
            sub_data[node] = [n for n in adj_list[node] if n in nodes]
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
        n = len(self.data)
        e = sum([len(self.data[node]) for node in self.__iter__()])/2
        if e != (n * (n - 1) / 2) :
            return False
        else:
            return True