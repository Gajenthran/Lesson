#!/usr/bin/python
# -*- coding: utf-8 -*-

import re
import sys


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
    "si élégant" : [_xml_satisfaction_tag, 6],
    "la plus brillante" : [_xml_satisfaction_tag, 9],
    "ni plus intéressant" : [_xml_dissatisfaction_tag, 2],
    "si belles" : [_xml_satisfaction_tag, 4],
    "plus vive approbation" : [_xml_agreement_tag, 6],
    "discussion sérieuse" : [_xml_disagreement_tag, 5],
    "la plus grande" : [_xml_satisfaction_tag, 8],
    "parfaitement ennuyeuses" : [_xml_dissatisfaction_tag, 7],
    "le plus beau" : [_xml_satisfaction_tag, 5],
    "plus de charme" : [_xml_satisfaction_tag, 6],
    "plus ultra de la belle" : [_xml_dissatisfaction_tag, 2],
    "souveraine injustice" : [_xml_dissatisfaction_tag, 7],
    "peu convenable" : [_xml_satisfaction_tag, 1],
    "bien jolie" : [_xml_satisfaction_tag, 4]
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
    # opening tag
    tag = '<' + _words[string][0] + ' int=' + str(_words[string][1]) + '>'
    source = source[:position] + tag + source[position:]
    position = position + len(string) + len(tag)

    # closing tag
    tag = '</' + _words[string][0] + '>'
    source = source[:position] + tag + source[position:]
    position += len(tag)

    return source, position


def main(argv):
    if len(argv) != 3 or not(argv[2].endswith('.xml')):
        usage(argv)

    inputFile = read_file(argv[1])
    for w in _words:
        beg = 0;
        while beg != len(inputFile):
            found = inputFile.lower().find(w, beg)
            if found == -1: break
            inputFile, beg = insert_tag(inputFile, found, w)

    write_file(argv[2], inputFile)

if __name__ == '__main__':
    main(sys.argv)