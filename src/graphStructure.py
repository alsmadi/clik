# -*- coding: utf-8 -*-

import numpy
from copy import deepcopy
from scipy import sparse
import scipy.weave as weave

"""
classe per gestire i grafi
"""
class Graph(object):
    def __init__(self, edges=None):
        self.data = {}
        #if isinstance(edges, list):
            #if edges is not None:
                #dict_set = self.data.setdefault
                #for edge in edges:
                    #dict_set(edge[0],[])
                    #dict_set(edge[1],[])
                    #if edge[1] not in self.data[edge[0]]:
                        #self.data[edge[0]].append(edge[1])
                        #self.data[edge[1]].append(edge[0])
        #elif isinstance(edges, dict):
            #self.data.update(edges)

    def nodes(self):
        return self.data.keys()

    def __iter__(self):
        return self.data.iterkeys()

    def __len__(self):
        return len(self.data)

    def __getitem__(self, n):
        return self.data[n]

    def dcopy(self):
        graph = self.__class__()
        data = graph.data
        iter = self.data.iteritems
        for k,v in iter():
            data[k] = v[:]
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
            #dict_set(edge[0],set())
            #dict_set(edge[1],set())
            #if edge[1] not in self.data[edge[0]]:
                #self.data[edge[0]].add(edge[1])
                #self.data[edge[1]].add(edge[0])
            dict_set(edge[0],[])
            dict_set(edge[1],[])
            if edge[1] not in self.data[edge[0]]:
                self.data[edge[0]].append(edge[1])
                self.data[edge[1]].append(edge[0])

    def remove_node(self, node):
        data = self.data
        adj_list = data[node]
        for adj_node in adj_list:
            data[adj_node].remove(node)
        del data[node]

    def remove_nodes(self, nodes):
        data = self.data
        for node in nodes:
            for adj_node in data[node]:
                data[adj_node].remove(node)
            del data[node]

    def remove_edge(self, edge):
        self.data[edge[0]].remove(edge[1])
        self.data[edge[1]].remove(edge[0])

    def closed_neighborhood(self, node):
        neighborhood = deepcopy(self.data[node])
        #neighborhood.add(node)
        #neighborhood = self.data[node][:]
        neighborhood.append(node)
        return neighborhood

    def subgraph(self, nodes):
        subgraph = self.__class__()
        nodes = set(nodes)
        adj_list = self.data
        sub_data = subgraph.data
        for adj_node in nodes:
            sub_data[adj_node] = [x for x in adj_list[adj_node] if x in nodes]
        return subgraph

    def induced_subgraph(self, node):
        adj_list = self.data
        nodes = set(adj_list[node])
        nodes.add(node)
        subgraph = self.__class__()
        sub_data = subgraph.data
        #nodes = deepcopy(adj_list[node])
        #nodes.add(node)
        #for n in nodes:
        #nodes = set([node]).union(adj_list[node])
        for adj_node in nodes:
            #subgraph.data[adj_node] = set(adj_list[adj_node]).intersection(nodes)
            sub_data[adj_node] = [x for x in adj_list[adj_node] if x in nodes]
        #subgraph.data = dict([(adj_node, [x for x in adj_list[adj_node].intersection(adj_list[node])]) for adj_node in adj_list[node]])
        #sub_data[node] = deepcopy(adj_list[node])
        return subgraph

    def induced_subgraph_(self, node):
        subgraph = self.__class__()
        adj_dict = self.data
        nodes = self.closed_neighborhood(node)
        sub_data = subgraph.data
        code="""
        int L_1 = PyList_Size(py_nodes);
        int L_2 = 0;
        int i, j = 0;
        PyObject *data;
        PyObject *adj_list;
        PyObject *node_1;
        PyObject *node_2;
        PyObject* nodes_set = PySet_New(py_nodes);

        for (i = 0; i < L_1; i++){
            data = PyList_New(0);
            node_1 = PyList_GetItem(py_nodes, i);
            adj_list = PyDict_GetItem(py_adj_dict, node_1);
            L_2 = PyList_Size(adj_list);
            for (j = 0; j < L_2; j++){
                node_2 = PyList_GetItem(adj_list, j);
                if (PySet_Contains(nodes_set, node_2)){
                    PyList_Append(data, node_2);
                    Py_DECREF(node_2);
                }
            }
            PyDict_SetItem(sub_data, node_1, data);
        }
        return_val = 0;
        """
        weave.inline(code, ['nodes', 'adj_dict', 'sub_data'])
        return subgraph

    def degree(self, node):
        return len(self.data[node])

    def adjacency_matrix(self):
        nodes = self.nodes()
        len_nodes = len(nodes)
        matrix = numpy.zeros((len_nodes, len_nodes), dtype=numpy.int)
#        matrix = sparse.lil_matrix((len_nodes, len_nodes), dtype=numpy.int)
        index=dict(zip(nodes,range(len_nodes)))
        data = self.data
        for a in nodes:
            for b in data[a]:
                i,j = index[a], index[b]
                if j > i:
                    matrix[i, j] = 1
                    matrix[j, i] = 1
        return matrix

    def is_complete(self):
        n = len(self.data)
        e = sum([len(self.data[node]) for node in self.__iter__()])/2
        if e != (n * (n - 1) / 2) :
            return False
        else:
            return True

"""
funzione per leggere il grafo dal file su disco
"""
def get_edges_from_file(filename):
    try:
        file = open(filename, 'r')
        def get_edge(list):
            return list[1], list[2]
        edges = [get_edge(line.split()) for line in file
                 if line.find('e', 0, 1) >= 0]
        return edges
    except:
        print "Errorre nel aprire il file!"