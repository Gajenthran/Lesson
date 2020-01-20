#!/usr/bin/python
import sys
import pandas as pd
from MR_Popular import MR_Popular
from MR_ContentBased import MR_ContentBased
from MR_KNN import MR_KNN

# L'ensemble des fichiers en entrée
FILES = {
	"movies"  : "../files/movies_metadata.csv",
	"ratings" : "../files/ratings.csv",
	"links"   : "../files/links_small.csv"
};

# Le nombre de recommendations
POPULAR_N = 10;

def csv_reader(filename, usecols=None, dtype=None):
	"""
		Lecteur de fichier csv avec aucune précision des
		types par défaut.

		:param filename: le nom du fichier
	"""
	data = None
	if usecols and dtype:
		data = pd.read_csv(filename, \
			usecols=usecols, \
			dtype=dtype)
	else:
		data = pd.read_csv(filename);
	return data;

def main():
	movies = csv_reader(FILES["movies"]);
	links = csv_reader(FILES["links"]);
	ratings = csv_reader(FILES["ratings"]);

	mrp = MR_Popular(movies, POPULAR_N);
	mrp.get_popular_movies();

	# mrcb = MR_ContentBased(movies, links);
	# mrcb.get_content_based_movies();

if __name__ == "__main__":
    main();