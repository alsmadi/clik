# -*- coding: utf-8 -*-

from graphStructure import Graph
from UserDict import UserDict

"""
classe che implementa attraverso un dizionario, una struttura per gestire tutti
i dati necessari per l'esecuzione dell'algoritmo DSATUR;
in particolare ogni elemento del dizionario avra come chiave il nodo e come valori
il grado del nodo nel sottografo non colorato o -1 se il nodo è stato colorato,
il grado di saturazione ed infine il colore assegnato al nodo o -1 se ancora il 
nodo non è stato colorato.
"""
class NodeDict(UserDict):
    def __init__(self, graph):
        UserDict.__init__(self)
        self.graph = graph
        self.uncolored_graph = Graph(graph.edges())
        for node in graph.nodes():
            #dizionario - data[node] = [grado nel grafo non colorato, grado di 
            #saturazione, colore]; i valori di default sono per ogni nodo
            #rispettivamente: il grado nel grafo originale, 0, None.
            self.data[node] = [graph.degree(node), 0, None]
    
    """
    metodo che rimuove dal sottografo dei nodi non colorati il nodo che è stato
    appena colorato
    """
    def update_uncolored_graph(self, node_sel):
        try:
            self.uncolored_graph.remove_node(node_sel)
        except:
            print "Error: Node not present!"
    
    """
    metodo che aggiorna il grado di saturazione
    """        
    def update_degree_satur(self, node_sel):
        for node in self.graph[node_sel]:
            colors_list = [self.data[adj_node][2] for adj_node in self.graph[node] if self.data[adj_node][2] != None]
            colors_list = list(frozenset(colors_list))
            self.data[node][1] = len(colors_list)
    
    """
    metodo che aggiorna il grado dei nodi del sottografo dei nodi non colorati dopo
    che è avvenuta una rimozione in tale sottografo, cioè dopo che un nodo è stato
    colorato
    """
    def update_degree(self, node_sel):
        self.data[node_sel][0] = None   
        for node in self.uncolored_graph.nodes():
            self.data[node][0] = len(self.uncolored_graph[node]) 
    
    """
    metodo che trova il minor colore possibile da assegnare ad un nodo selezionato.
    itera sui nodi adiacenti e ritorna il primo colore diposnibile tra quelli 
    non usati
    """    
    def get_min_color_possible(self, node_sel):
        colors_list = [self.data[node][2] for node in self.graph[node_sel] if self.data[node][2] != None]
        colors_list = list(frozenset(colors_list))
        if len(colors_list) == 0:
            return 0
        colors_list.sort()
        maxcolor = colors_list[len(colors_list) - 1]
        for i in range(len(colors_list)):
            if i != colors_list[i]:
                return i
        return maxcolor + 1
    
    """
    metodo che colora il nodo selezionato
    """
    def color(self, node):
        self.data[node][2] = self.get_min_color_possible(node)
        return self.data[node][2]
    
    def get_sorted_nodes(self):
        return sorted(self.data.items(), key=lambda x: int(x[0]))
             
    def get_sorted_degree_satur(self):
        return sorted(self.data.items(), key=lambda x: x[1][1], reverse=True)
    
    def get_sorted_degree(self):
        return sorted(self.data.items(), key=lambda x: x[1][0], reverse=True)
    
    def get_sorted_color(self):
        return sorted(self.data.items(), key=lambda x: x[1][2], reverse=True)
    
    """
    metodo che ritorna il nodo con il massimo grado nel sottografo dei nodi non
    colorati
    """
    def get_node_max_degree(self):
        item = max(self.data.items(), key=lambda x: x[1][0])
        return item[0]
    
    """
    metodo che ritorna il nodo con il massimo grado di saturazione
    """
    def get_node_max_satur_degre(self):
        list = sorted(self.data.items(), key=lambda x: x[1][1], reverse=True)
        node_item_0 = list[0]
        node_item_1 = list[1]
        check_eq = False
        if node_item_0[1][1] == node_item_1[1][1]:
            check_eq = True
        return node_item_0[0], check_eq

"""
implementazione dell'algoritmo DSATUR.
passi da eseguire:
1 - ordinare i nodi per grado
2 - colorare il nodo con grado maggiore con colore 0
3 - scegliere il nodo con il massimo grado di saturazione; 
    se vi sono più nodi con lo stesso grado di saturazione -> scegliere 
    un qualunque nodo con grado maggiore tra quelli non colorati
4 - colorare il nodo scelto con il minimo colore possibile
5 - se ultimo nodo colorato -> STOP, altrimenti torna a 3 
"""        
def dsatur_algorithm(graph):
    #inizializzazione
    nodes = NodeDict(graph)
    colors_list = []
    append_color = colors_list.append
    #estriamo il nodo con grado maggiore
    node_sel = nodes.get_node_max_degree()
    #coloriamo il nodo e aggiorniamo 
    ncolor = nodes.color(node_sel)
    #color_list: lista in cui verrano memorizzati i colori con cui verranno 
    #colorati i nodi
    append_color(ncolor)
    iterations = len(nodes.uncolored_graph)
    #si itera su tutti i nodi rimasti da colorare
    for i in range(iterations):
        #si trova il nodo di grado di saturazione minore
        node_sel, check_eq = nodes.get_node_max_satur_degre()
        if check_eq is False:
            #se vi è un solo nodo con grado di saturazione massimo
            ncolor = nodes.color(node_sel)
            append_color(ncolor)
            nodes.update_degree_satur(node_sel)
            nodes.update_uncolored_graph(node_sel)
            nodes.update_degree(node_sel)
        else:
            #se vi è un conflitto si estrae un qualunque nodo di grado maggiore
            #nel sottografo di nodi non ancora colorati
            node_sel = nodes.get_node_max_degree()
            ncolor = nodes.color(node_sel)
            append_color(ncolor)
            nodes.update_degree_satur(node_sel)
            nodes.update_uncolored_graph(node_sel)
            nodes.update_degree(node_sel)
    return len(frozenset(colors_list))
