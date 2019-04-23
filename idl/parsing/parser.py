import nltk
from nltk.tokenize import word_tokenize
from nltk.chunk import RegexpParser
from nltk.tag import pos_tag
import os
import re

# ponctuation
# nature en francais
# stopwords
# probabilité words
# det != pp
# preposition

prep = ["à", "de", "dans", "par" "pour", "sur", "en", "vers"];
adv = ['bien', 'vite', 'mal', 'beaucoup', 'moins', 'trop', 'hier', 'aujourd\'hui', 'demain'];
pp = ['je', 'tu', 'il', 'elle', 'nous', 'vous', 'ils', 'elles'];
det = ['un', 'une', 'le', 'la', 'les'];
ponctuation = ['\\.', '\\!', '\\?', '\\:', '\\;', '\\,'];
get_det = [];
ex = 'Je le veux mais je ne sais pas si cela est une bonne idée sur une montage. Un moyen ?';

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
	mp(ponctuation) + "\\s*" + mw(det) # (?:\!|\,|\.|\;|\?|\:|\.\.\.)\s*(?:le|la|un)\b
];

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


def main():
	matchPattern(adv, det);

if __name__ == '__main__':
	main()