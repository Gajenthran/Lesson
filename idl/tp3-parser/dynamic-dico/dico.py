#!/usr/bin/python
# -*- coding: utf-8 -*-

import re 
import sys

_tags = ["Satisfaction", "Dissatisfaction", "Disagreement", "Agreement", "Pleasure"]

_words = {}
_word_list = []

def find_all_words(txt):
    """
        Trouver tous les mots du fichier source 'annotation' qui va permettre de 
        remplir notre dictionnaire qui contiendra le mot en question et la balise
        associée

        :param txt: fichier source 'annotation'
    """
    global _word_list
    global _words
    global _tags
    for tag in _tags:
        for v in range(1, 11):
            matchs = re.findall("<" + tag + " int=" + str(v) + ">(.*?)</" + tag + ">", txt)
            matchs = set(matchs)
            matchs = list(matchs)
            for i in range(0, len(matchs)):
                _words[matchs[i].lower()] = [tag, v]

def create_word_list():
    """
        Créer une liste des mots à trouver dans le texte, à l'aide des mots dans
        le dictionnaire. A l'aide de cette liste, nous pourrons gérer l'ordre dans laquelle
        nous chercherons les mots sans pour autant modifier le dictionnaire
    """
    global _word_list
    global _words
    for w in _words:
        _word_list.append(w)
    _word_list = set(_word_list)
    _word_list = list(_word_list)
    _word_list = sorted(_word_list, key=len)
    _word_list.reverse()

def create_dico(txt):
    find_all_words(txt)
    create_word_list()

