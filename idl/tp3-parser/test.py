#!/usr/bin/python
# -*- coding: utf-8 -*-

import re
import sys

_tags = ["Satisfaction", "Dissatisfaction", "Disagreement", "Agreement", "Pleasure"]
words = [];
tags = [];

def read_file(filename):
    with open(filename, 'r') as file:
        data = file.read()
    return data

def sort_words():
    for i in range(0, len(words)):
        for j in range(0, len(words)):
            if len(words[i]) > len(words[j]):
                tmp = words[i]
                words[i] = words[j]
                words[j] = tmp
                tmp = tags[i]
                tags[i] = tags[j]
                tags[j] = tmp

def print_words():
    for w in tags:
        print(w)

def find_all_words_with_tag(data):
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
    sort_words()
    print_words()

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
    for match in re.finditer(r"\bque\b", data):
        print(match.start())
        print(match.end())
        print(match.group())
        print("")
    """
    """
    for w in dico._words:
        beg = 0;
        while beg != len(inputFile):
            # found = re.search(inputFile.lower().find(w, beg)
            if found == -1 or not(is_word(inputFile, found, w)): break
            inputFile, beg = insert_tag(inputFile, found, w)
    """

def find_all_tags(data):
    tag = re.findall("<(.*?)>", data)
    tag = set(tag)
    tag = list(tag)
    for i in range(0, len(tag)):
        print("- " + tag[i])

def main(argv):
    data = read_file(argv[1])
    find_all_words(data)
    # find_all_tags(data)

if __name__ == '__main__':
    main(sys.argv)
