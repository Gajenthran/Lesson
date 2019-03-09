#!/usr/bin/python
# -*- coding: utf-8 -*-

import re
import sys
import dico


"""
TODO:
  # Rajouter l'encodage XML
  # Eliminer les redondances (sous-balises)
"""

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
    # balise ouvrante
    tag = '<' + dico._words[string][0] + ' int=' + str(dico._words[string][1]) + '>'
    source = source[:position] + tag + source[position:]
    tmp_string = filter_file(string)
    position = position + len(tmp_string) + len(tag)

    # balise fermante
    tago = tag
    tag = '</' + dico._words[string][0] + '>'
    source = source[:position] + tag + source[position:]
    position = len(tag) + len(tago)
    return source, position


def subtag(source, position):
    i = position
    while i > 0 and (source[i] != '<' or (source[i] == '<' and source[i + 1] == ' ')):
        i -= 1
    if(source[i + 1] == '/' or i == 0):
        return True
    return False

def filter_file(file, forTag=False):
    file = file.replace('\n', ' ')
    file = file.replace('\t', ' ')
    file = file.replace('\r', ' ')
    if forTag : file = file.replace('  ', ' ')
    return file

def filter_string(string):
    string = string.replace(' ', '\s+')
    string = "(?<!\w)" + string
    if string[len(string)-1] != '\'':
        string = string + "(?!\w)"
    return string


def txt_to_xml(inputFile):
    for w in range(0, len(dico._word_list)):
        beg = 0;
        filtered_word = filter_string(dico._word_list[w])
        for match in re.finditer(filtered_word.lower(), filter_file(inputFile.lower()), re.MULTILINE):
            pos = match.start() + beg
            if subtag(inputFile, pos):
                inputFile, end = insert_tag(inputFile, pos, dico._word_list[w])
                beg += end

    return inputFile


def main(argv):
    if len(argv) != 3 or not(argv[2].endswith('.xml')):
        usage(argv)

    inputFile = read_file(argv[1])
    inputFile = txt_to_xml(inputFile)
    write_file(argv[2], inputFile)

if __name__ == '__main__':
    main(sys.argv)