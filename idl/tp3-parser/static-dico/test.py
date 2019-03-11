#!/usr/bin/python
# -*- coding: utf-8 -*-

import re
import sys
import dico

_tags = ["Satisfaction", "Dissatisfaction", "Disagreement", "Agreement", "Pleasure"]
words = [];
tags = [];

def read_file(filename):
    with open(filename, 'r') as file:
        data = file.read()
    return data

def find_all_dico(data):
    dic = []
    for w in dico._words:
        dic.append(w)
    dic = set(dic)
    dic = list(dic)
    dic = sorted(dic, key=len)
    dic.reverse()

    for i in range(0, len(dico._word_list)):
        print(dico._words.get(dico._word_list[i]))

    """
    print("word_list = [", end='')
    for i in range(0, len(dic)):
        print("\"" + dic[i] +  "\", ", end='')
    print("]")
    """

def find_all_words_with_tag(data):
    global _tags
    global words
    global tags

    for tag in _tags:
        for v in range(1, 11): 
            word = re.findall("<" + tag + " int=" + str(v) + ">(.*?)</" + tag + ">", data)
            word = set(word)
            word = list(word)
            word.sort(lambda x,y: cmp(len(y), len(x)))
            for w in word:
                words.append(w)
            for i in range(0, len(word)):
                tags.append("    \"" + word[i] + "\"" + ": [_xml_" + tag.lower() + "_tag, " + str(v) + "],")



def find_all_words(data):
    rg = re.search("de", data)
    beg = 0;
    while rg is not None:
        print(beg)
        d = data[beg:]
        rg = re.search("de", d)
        if rg is not None: 
            beg = rg.start() + 2

    """
    for w in dico._words:
        beg = 0;
        while beg != len(inputFile):
            # found = re.search(inputFile.lower().find(w, beg)
            if found == -1 or not(is_word(inputFile, found, w)): break
            inputFile, beg = insert_tag(inputFile, found, w)
    """
def filter_tag(data):
    for match in re.finditer("<\w+ int=\d>(.*?)<\w+ int=\d>(.*?)</(.*)>(.*)</(.*?)*>", data.lower()):
        position = find_subtag(data.lower(), match.start(), match.end())
        print(match.start(), match.end())
        data = remove_subtag(data, position)
    return data

def find_all_tags(data):
    tag = re.findall("<(.*?)>", data)
    tag = set(tag)
    tag = list(tag)
    for i in range(0, len(tag)):
        print("- " + tag[i])


def read_character(data):
    chars = []
    for line in data:
       for c in line:
           chars.append(c)
    print(chars)
    return chars

def print_words():
    global words
    for key in words:
        print(key, i)

def main(argv):
    data = read_file(argv[1])
    find_all_words_with_tag(data)
    print_words()
    # remove_subtag(data)
    # read_character(data)
    # data = filter_tag(data)


if __name__ == '__main__':
    main(sys.argv)
