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
from src.graphStructure import Graph, get_edges_from_file
from src.sequentialEliminationAlgorithm import \
                                        sequential_elimination_algorithm_1, \
                                        sequential_elimination_algorithm_2
from src.upperBoundAlgorithms import *
import logging

"""
main
"""
def main(args):
    psyco.full()
#    cwd = os.getcwd()
#    ext = ".clq"
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
    logging.basicConfig(filename='./results.log', level=logging.INFO, format=" %(message)s")
    print "------------------------------"
    fun_1 = [upper_bound_from_cardinality, upper_bound_from_linear_coloring, upper_bound_from_dsatur]
    fun_2 = [upper_bound_from_cardinality_2, upper_bound_from_linear_coloring_2, upper_bound_from_dsatur_2]
    log = ['cardinality', 'linear coloring', 'dsatur']
    for dirpath, ignore, files in os.walk("./test/"):
        if dirpath == "./test/":
            for i in range(3):
                text = "---------- " + log[i]
                logging.info(text)
                print text
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
                        upper_bound_fun_1 = fun_1[i]
                        upper_bound_fun_2 = fun_2[i]
                        sequential_elimination_algorithm_2(graph, upper_bound_fun_1, upper_bound_fun_2)
                logging.info("")
                print ""

if __name__ == "__main__":
    main(sys.argv)
