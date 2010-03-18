#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
questo file contiene il main; ho aggiunto qualche funzione per misurare
il tempo di esecuzione.
il primo algoritmo è quello originale.
le definizioni degli algoritmi si trovano nel file sequentialEliminationAlgorithm.py
NB: probabilmente all'inizio dell'esecuzione vedrà alcuni warning; riguardano
una libreria usata non l'applicazione.
"""

import sys
import os
import psyco
from src.graphStructure import Graph
from src.sequentialEliminationAlgorithm import \
                                        sequential_elimination_algorithm_1, \
                                        sequential_elimination_algorithm_2
import src.upperBoundAlgorithms as UBA
import logging

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

"""
main
"""
def main(args):
    psyco.full()
    cwd = os.getcwd()
    ext = ".clq"
#    testname = "graph_test" # 7 nodes
#    testname = "myciel6(col)"
#    testname = "myciel4(col)"
#    testname = "myciel7(col)"
#    testname = "queen9(col)"
#    testname = "keller4" # 171 nodes
#    testname = "hamming6-2" # 64 nodes
#    testname = "MANN_a9" # 45 nodes
#    testname = "sanr200_0_7" # 200 nodes
#    testname = "hamming6-4" # 64 nodes
#    testname = "johnson16-2-4" # 120 nodes
#    testname = "johnson8-4-4" # 70 nodes
#    testname = "johnson8-2-4" # 28 nodes
#    testname = "Inprova2_3"
#    testname = "brock200_1" # 200 nodes
#    testname = "brock400_4" # 400 nodes
#    testname = "brock800_2" # 800 nodes
#    testname = "c-fat500-2" # 500 nodes
#    filename1 = cwd + "/benchmarks/" + "sanr200_0.7" + ext
#    filename2 = cwd + "/benchmarks/" + "brock200_1" + ext
#    filenames = [filename1, filename2]
#    filename = cwd + "/benchmarks/" + testname + ext
    LOG_FILENAME = './result_cardinality.log'
    logging.basicConfig(filename=LOG_FILENAME,level=logging.INFO)
    print "*****************************"
    for dirpath, ignore, files in os.walk("./test/"):
        if dirpath == "./test/":
            files.sort()
            for nfile in files:
                if nfile != ".directory":
                    filename = os.path.join(dirpath, nfile)
                    graph = Graph(get_edges_from_file(filename))
                    text = ''.join(["graph: ", str(nfile), ", nodes: ", str(len(graph)), ", edges: ",
                                    str(graph.number_of_edges())])
                    logging.info(text)
                    print text
                    #upper_bound_fun = UBA.upper_bound_from_linear_coloring
                    #sequential_elimination_algorithm_1(graph, upper_bound_fun)
                    #print "--"
                    #graph = Graph(get_edges_from_file(filename))
                    upper_bound_fun_1 = UBA.upper_bound_from_cardinality
                    upper_bound_fun_2 = UBA.upper_bound_from_cardinality
                    sequential_elimination_algorithm_2(graph, upper_bound_fun_1, upper_bound_fun_2)
                    logging.info('---')
                    print "----"

if __name__ == "__main__":
    main(sys.argv)
