#!/usr/bin/python
# -*- coding: utf-8 -*-

import re
import sys

"""
TODO: 
  # Mettre les mots en miniscules


""" 

_xml_satisfaction_tag = "Satisfaction"
_xml_dissatisfaction_tag = "Dissatisfaction"
_xml_disagreement_tag = "Disagreement"
_xml_agreement_tag = "Agreement"

_words = {
    "profonde injustice" : [_xml_dissatisfaction_tag, 6],
    "bien soignés" : [_xml_satisfaction_tag, 6],
    "pas grand" : [_xml_dissatisfaction_tag, 6],
    "bien grand" : [_xml_satisfaction_tag, 7],
    "divisions absolues" : [_xml_disagreement_tag, 6],
    "aucune différence" : [_xml_agreement_tag, 6],
    "si simple" : [_xml_satisfaction_tag, 4],
    "le plus grand" : [_xml_satisfaction_tag, 8],
    "assez importante" : [_xml_satisfaction_tag, 4],
    "si enchanté" : [_xml_satisfaction_tag, 6],
    "si souple" : [_xml_agreement_tag, 4],
    "si élégant" : [_xml_satisfaction_tag, 6]
}

def usage(argv):
    print("Usage: " + str(argv[0]) + " <source.txt> <destination.xml>")
    sys.exit()

def read_file(filename):
    with open(filename, 'r') as file:
        data = file.read()
    return data

def write_file(filename, data):
    with open(filename, 'w') as file:
        file.write(data)

def insert_tag(source, position, string):
    source = source[:position] + '<' + string + '>' + source[position:]
    return source 


def main(argv):
    if len(argv) != 3:
        usage(argv)

    inputFile = read_file(argv[1])
    for w in _words:
        beg = 0;
        while beg != len(inputFile):
            found = inputFile.find(w, beg)
            if found == -1: break
            # inputFile[:found] + '<' + _words[w][0] + '>' + inputFile[found:]
            inputFile = insert_tag(inputFile, found, _words[w][0])
            beg = found + len(w)
            print(found)
            # print(found + len(w), beg) # Nouvelle taille du inputFile à rajouter sinon on reste sur l'ancien
            # inputFile = insert_tag(inputFile, beg, _words[w][0])

    write_file(argv[2], inputFile)

if __name__ == '__main__':
    main(sys.argv)