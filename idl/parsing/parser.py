import nltk, nltk.data, nltk.tag
from nltk import FreqDist
from nltk.tokenize import word_tokenize
from nltk.tokenize.regexp import RegexpTokenizer
from nltk.chunk import RegexpParser
import codecs
import os
import re

# ponctuation
# nature en francais
# stopwords
# probabilité words
# det != pp
# preposition

dico = {
	"PREP" : ["à", "derrière", "malgré", "sauf", "selon", "avant", "devant", "sous", "avec", 
		     "en", "par", "sur", "entre", "parmi", "envers", "pendant", "vers", "dans", "pour", 
		     "de", "près", "depuis", "sans"],

    "ADV"  : ['bien', 'vite', 'mal', 'beaucoup', 'moins', 'trop', 'hier', 'aujourd\'hui', 'demain'],

    "PRON" : ["je", "tu", "il", "elle", "on", "nous", "vous", "ils", "elles", "me", "m'", "moi", "te", 
             "t'", "toi", "se", "y", "le", "lui", "soi", "leur", "eux", "lui", "qui", "que", "quoi", 
             "dont" "où"],

	"CONJ" : ["mais", "ou", "et", "donc", "or", "ni", "car", "que", "quand", "comme", "si", "lorsque", 
	          "quoique", "puisque"],

	"DET" : ["le", "la", "les", "l'", "un", "une", "des", "d'", "du", "de", "au", "aux", "ce", "cet", 
	        "cette", "ces", "mon", "son", "ma", "ta", "sa", "mes", "ses", "notre", "votre", "leur", 
	        "nos", "vos", "leurs", "aucun", "aucune", "aucuns", "tel", "telle", "tels", "telles", 
	        "tout", "toute", "tous", "toutes", "chaque"]
};

adv = ['bien', 'vite', 'mal', 'beaucoup', 'moins', 'trop', 'hier', 'aujourd\'hui', 'demain'];
pp = ['je', 'tu', 'il', 'elle', 'nous', 'vous', 'ils', 'elles'];
prep = ['je', 'tu', 'il', 'elle', 'nous', 'vous', 'ils', 'elles'];
det = ['un', 'une', 'le', 'la', 'les'];
ponctuation = ['\\.', '\\!', '\\?', '\\:', '\\;', '\\,'];
get_det = [];
ex = 'Les français, c\'est-à-dire les hommes vivant en France.';


def mw(words):
	combineWord = "";
	for i in range(0, len(words)):
		combineWord += words[i] + "|";

	combineWord = combineWord[:-1];
	return "\\b(?:" + combineWord + ")\\b";

def mp(words):
	combineWord = "";
	for i in range(0, len(words)):
		combineWord += words[i] + "|";

	combineWord = combineWord[:-1];
	return "(?:" + combineWord + ")";

rules = [
	mw(prep) + "\\s+" + mw(det),
	mw(adv) + "\\s+" + mw(det),
	mp(ponctuation) + "\\s*" + mw(det), # (?:\!|\,|\.|\;|\?|\:|\.\.\.)\s*(?:le|la|un)\b
	mw(pp) + "\\s+" + mw(det)
];

forbidden_rules = [3];

def matchPattern(words1, words2):
	global rules;
	for i in range(0, len(rules)):
		reg = re.compile(rules[i], re.IGNORECASE);
		print(re.findall(reg, ex));

def matchSimple():
	reg = re.compile(r"\b(?:le|la|un)\b", re.IGNORECASE);
	print(re.findall(reg, ex));

def transform(words):
	for i in range(0, len(words)):
		words[i] = words[i].lower();
	return words;


def parse_det(txt):
	words = transform(nltk.word_tokenize(txt));
	for w in words:
		if w in det:
			get_det.append(w);
	return words;


def read_file(filename):
	file = codecs.open(filename);
	words = file.read().split();
	file.close();
	return words;

def freq_word(words):
	freq = FreqDist();
	for w in words:
		freq[w.lower] += 1;
	print(freq);

def main():
	global ex;
	txt = ex.split()
	reg_words = r"quelqu'un|\w+[-\w+]+|[+-]?[[0-9]*[.,]]?[0-9]+|aujourd'hui|prud'hom\w+|\d+|\w['´`]|\$[\d\.]+|\w+|\S+"
	tok = RegexpTokenizer(reg_words);
	print(tok.tokenize(ex));
	# print(re.findall(ex));
	# print(tok.tokenize(txt));
	# t.concordance("je", width=50, lines=10)
	# matchPattern(adv, det);

if __name__ == '__main__':
	main()