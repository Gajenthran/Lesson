#!/usr/bin/python
# -*- coding: utf-8 -*-

import re
import sys
import dico

"""
TODO:
  # Rajouter l'encodage XML
  # Rajouter d'autres balises
  # Essayer sur d'autres fichiers textes longs
  # Optimiser certaines instructions (whitespace, caractères spéciaux)
"""

_annotation_filename = "Fusion_Animal_Hum_Corpus_v3_annotation_FR_utf8.txt"

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
    """
        Insertion des balises ouvrantes et fermante en question selon 
        le string passé en paramètre, dans le fichier source à une position
        donnée

        :param source: le fichier source
        :param position: position à laquelle nous devons ajouter la balise ouvrante
        :param string: chaîne reconnue dans le fichier grâce à notre dictionnaire
    """
    # balise ouvrante
    tmp_string = string.replace('  ', ' ')
    tag = '<' + dico._words[tmp_string][0] + ' int=' + str(dico._words[tmp_string][1]) + '>'
    source = source[:position] + tag + source[position:]
    position = position + len(string) + len(tag)

    # balise fermante
    tago = tag
    tag = '</' + dico._words[tmp_string][0] + '>'
    source = source[:position] + tag + source[position:]
    position = len(tag) + len(tago)
    return source, position


def subtag(source, position):
    """
        Vérification des sous-balises. Pour cela, on vérifie simplement si une balise est 
        contenu dans une balise ou pas. On regarde la dernière balise à partir de la position
        actuelle : si il s'agit d'une balise ouvrante alors il s'agit d'une sous-balise sinon
        il ne s'agit pas d'une s'agit balise.

        :param source: le fichier source
        :param position: position à laquelle nous devons ajouter la balise ouvrante
    """
    i = position
    while i > 0 and (source[i] != '<' or (source[i] == '<' and source[i + 1] == ' ')):
        i -= 1
    if(source[i + 1] == '/' or i == 0):
        return False
    return True

def filter_file(source):
    """
        Filtrage du fichier source afin d'éviter le retour chariot et ainsi
        faciliter la manipulation des caractères

        :param source: fichier source
    """
    source = source.replace('\n', ' ')
    source = source.replace('\t', ' ')
    source = source.replace('\r', ' ')
    return source

def filter_string(string):
    """
        Filtrage d'un mot du dictionnaire pour pouvoir placer des expressions régulières
        et permettre une reconnaissance améliorée des mots dans le fichier

        :param string: mot du dictionnaire
    """
    string = string.replace(' ', '\s+')
    string = '(' + string + ')'
    string = "(?<!\w)" + string
    # -2 car on prend le dernier caractère du string sans compter la parenthèse fermante
    if string[len(string)-2] != '\'':
        string = string + "(?!\w)"
    return string


def txt_to_xml(txt):
    """
        Transformer le fichier texte donnée en entrée en fichier .xml en balisant les mots 
        du dictionnaire

        :param txt: fichier texte       
    """
    for w in range(0, len(dico._word_list)):
        beg = 0;
        filtered_word = filter_string(dico._word_list[w])
        regex = re.compile(filtered_word.lower(), re.MULTILINE)
        for match in regex.finditer(filter_file(txt.lower()), re.IGNORECASE):
            matched_word = str(match.group(0))
            pos = match.start() + beg
            if not(subtag(txt, pos)):
                txt, end = insert_tag(txt, pos, matched_word)
                beg += end

    return txt


def main(argv):
    if len(argv) != 3 or not(argv[2].endswith('.xml')):
        usage(argv)

    # fichier avec les balises permettant de créer le dictionnaire
    annot_file = read_file(_annotation_filename)
    dico.create_dico(annot_file)

    # fichier à baliser
    txt_file = read_file(argv[1])
    xml_file = txt_to_xml(txt_file)
    write_file(argv[2], xml_file)

if __name__ == '__main__':
    main(sys.argv)