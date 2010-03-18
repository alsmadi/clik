# -*- coding: utf-8 -*-

import usefulFunctions
import scipy.weave as weave

"""
classe che implementa attraverso un dizionario, una struttura per gestire tutti
i dati necessari per l'esecuzione dell'algoritmo DSATUR;
in particolare ogni elemento del dizionario avra come chiave il nodo e come valori
il grado del nodo nel sottografo non colorato o -1 se il nodo è stato colorato,
il grado di saturazione ed infine il colore assegnato al nodo o -1 se ancora il
nodo non è stato colorato.
"""
class DSaturClass(object):
    def __init__(self, graph):
        self.data = {}
        self.graph = graph
        self.uncolored_graph = graph.dcopy()
        nodes = graph.nodes()
        for node in nodes:
            #dizionario - data[node] = [grado nel grafo non colorato, grado di
            #saturazione, colore]; i valori di default sono per ogni nodo
            #rispettivamente: il grado nel grafo originale, 0, None.
            self.data[node] = [len(graph[node]), 0, -1]

    # aggiorniamo
    def update(self, node_sel):
        data = self.data
        graph = self.graph
#        gdata = graph.data
        ugraph = self.uncolored_graph
#        udata = ugraph.data
        unique = usefulFunctions.unique
        # aggiorniamo il grafo dei nodi non colorati
        ugraph.remove_node(node_sel)
        unodes = ugraph.nodes()
        # aggiorniamo il grado di saturazione
        for node in graph[node_sel]:
            colors_list = unique([data[adj_node][2] for adj_node in graph[node]
                                  if not data[adj_node][2] is -1])
            data[node][1] = len(colors_list)
        # aggiorniamo il grado dei nodi non colorati
        data[node_sel][0] = -1
        for node in unodes:
            data[node][0] = len(ugraph[node])
#        code="""
#        #line 48 "dsatur.py" 
#        int i, j, L_1, L_2, size;
#        PyObject *adj_node_sel;
#        PyObject *colors_set;
#        PyObject *adj_list;
#        PyObject *node_1;
#        PyObject *node_2;
#        PyObject *item;
#        PyObject *color;
#        
#        adj_node_sel = PyDict_GetItem(py_gdata, py_node_sel);
#        L_1 = PyList_Size(adj_node_sel);
#        for (i = 0; i < L_1; i++)
#        {   
#            colors_set = PySet_New(NULL);
#            node_1 = PyList_GetItem(adj_node_sel, i);
#            adj_list = PyDict_GetItem(py_gdata, node_1);
#            L_2 = PyList_Size(adj_list);
#            for (j = 0; j < L_2; j++)
#            {
#                node_2 = PyList_GetItem(adj_list, j);
#                color = PyList_GetItem(PyDict_GetItem(py_data, node_2), 2);
#                PySet_Add(colors_set, color);
#            }
#            PySet_Discard(colors_set, PyInt_FromLong(-1));
#            size = PySet_Size(colors_set);
#            item = PyDict_GetItem(py_data, node_1);
#            PyList_SetItem(item, 1, PyInt_FromSsize_t(size));
#            PyDict_SetItem(py_data, node_1, item);
#        }
#        item = PyDict_GetItem(py_data, py_node_sel);
#        PyList_SetItem(item, 0, PyInt_FromLong(-1));
#        PyDict_SetItem(py_data, py_node_sel, item);
#        L_1 = PyList_Size(py_unodes);
#        for (i = 0; i < L_1; i++)
#        {
#            node_1 = PyList_GetItem(py_unodes, i);
#            item = PyDict_GetItem(py_data, node_1);
#            size = PyList_Size(PyDict_GetItem(py_udata, node_1));
#            PyList_SetItem(item, 0, PyInt_FromSsize_t(size));
#            PyDict_SetItem(py_data, node_1, item);
#        }
#        return_val = 0;
#        """
#        weave.inline(code, ['data', 'gdata', 'unodes', 'udata', 'node_sel'])

    """
    metodo che determina il colore e colora il nodo selezionato
    """
    def color(self, node):
        data = self.data
        graph = self.graph
        unique = usefulFunctions.unique
        colors_list = unique([data[adj_node][2] for adj_node in graph[node]
                             if data[adj_node][2] is not -1])
        length = len(colors_list)
        if length == 0:
            data[node][2] = 0
            return data[node][2]
        else:
            if length != colors_list[length -1] + 1:
                colors_list.sort()
                for i in range(length):
                    if i != colors_list[i]:
                        data[node][2] = i
                        return data[node][2]
            else:
                data[node][2] = length
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
        items_list = [item for item in items if item[0] in unodes]
        items_list_sort = items_list.sort
        items_list_sort(key=lambda x: x[1][1], reverse=True)
        node_item_0 = items_list[0]
        node_item_1 = items_list[1]
        if node_item_0[1][1] == node_item_1[1][1]:
            return node_item_0[0], True
        else:
            return node_item_0[0], False
