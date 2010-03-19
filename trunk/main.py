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
import src.upperBoundAlgorithms as UBA
import logging

"""
main
"""
def main(args):
    psyco.full()
    logging.basicConfig(filename='./results.log', level=logging.INFO, format=" %(message)s")
    print "------------------------------"
    fun_1 = [UBA.upper_bound_from_cardinality, UBA.upper_bound_from_linear_coloring, 
             UBA.upper_bound_from_dsatur]
    fun_2 = [UBA.upper_bound_from_cardinality_2, UBA.upper_bound_from_linear_coloring_2, 
             UBA.upper_bound_from_dsatur_2]
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
