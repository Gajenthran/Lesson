#!/usr/bin/python
# -*- coding: utf-8 -*-

import re
import sys
import dico

"""
TODO:
  # Erreur pour dissatisfaction
  # Ranger le dico dans l'ordre décroissant
  # Plus efficace
  # Eviter les décalages de balises
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
    position = position + len(string) + len(tag)

    # balise fermante
    tag = '</' + dico._words[string][0] + '>'
    source = source[:position] + tag + source[position:]
    position += len(tag)

    return source, len(tag) + len(string)

"""
def is_word(source, position, string):
    if position == 0:
        return True;
    # eviter les doublons ou les balises imbriquées
    for i in range(0, dico.maxWord):
        if source[position-i] == '>' or source[position + len(string) + i] == '<':
            return False;
    # si il s'agit d'un mot se terminant par une apostrophe, on ne vérifie pas la suite
    if source[position + len(string) - 1] == '\'':
        return True;
    # on vérifie si le début et la fin du mot n'est pas lié à un autre
    if not(source[position-1].isalnum()) or not(source[position + len(string)].isalnum()): # or source[position-1] == '>' or source[position + len(string)] == '<':
        return True;
    # dans n'importe quel autre cas, il ne s'agit pas d'un mot
    return False;
"""

def subtag(source, position):
    i = position
    while i > 0:
        if source[i] == '>':
            while source[i] != '<':
                i -= 1
                continue
            # verifie si il s'agit d'une balise fermante ou ouvrante
            if source[i + 1] == '/':
                return True
            else:
                return False
        i -= 1

    # par défaut à True, car il n'y a pas de sous-balise
    return True


def main(argv):
    if len(argv) != 3 or not(argv[2].endswith('.xml')):
        usage(argv)

    inputFile = read_file(argv[1])
    for w in dico._words:
        """
        beg = 0;
        while beg != len(inputFile):
            found = re.search(inputFile.lower().find(w, beg)
            if found == -1 or not(is_word(inputFile, found, w)): break
            inputFile, beg = insert_tag(inputFile, found, w)
        """ 
        for match in re.finditer(w, inputFile.lower()):
            if subtag(inputFile, match.start()):
                inputFile, beg = insert_tag(inputFile, match.start(), w)

    write_file(argv[2], inputFile)

if __name__ == '__main__':
    main(sys.argv)