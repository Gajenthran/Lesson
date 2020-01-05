#!/usr/bin/python

import sys
import os
import pandas as pd
from MR_Popular import MR_Popular

FILENAMES = {
	"movies"  : "files/movies_metadata.csv",
	"ratings" : "files/ratings.csv",
	"links"   : "files/links_small.csv"
};

POPULAR_N = 10;


def csv_reader(filename):
	data = pd.read_csv(filename);
	return data;

def main():
	movies = csv_reader(FILENAMES["movies"]);
	mrp = MR_Popular(movies, POPULAR_N);
	mrp.get_popular_movies();

if __name__ == "__main__":
    main();