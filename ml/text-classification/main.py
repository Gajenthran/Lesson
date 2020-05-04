import os
import numpy as np
import pandas as pd

from sklearn import tree, metrics, datasets
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.model_selection import train_test_split

from sklearn.naive_bayes import MultinomialNB
from sklearn.linear_model import SGDClassifier
from sklearn.neighbors import KNeighborsClassifier

from sklearn.utils.extmath import randomized_svd
from sklearn.decomposition import TruncatedSVD
import umap
import matplotlib.pyplot as plt
import seaborn as sns

def myMAE(predict,target):
	return (abs(predict-target)).mean()

def load_files():
	files = []
	cat = []
	categories = []
	catId = 0

	for file in os.listdir("20-newsgroups"):
		if file.endswith(".txt"):
			c = os.path.splitext(file)[0]
			categories.append(c)
			files.append("20-newsgroups/" + file)
			cat.append(catId)
			catId += 1

	return { 'filenames': files, 'cat': cat }, categories

def read_file(filename, cat):
	with open(filename, encoding="utf8", errors="ignore") as f:
		reader = { 'data': f.read(), 'cat': cat }
	return reader

def bow(data, vocabulary=None):
	cv = CountVectorizer(vocabulary=vocabulary)
	occ = cv.fit_transform(data)
	return occ, cv.vocabulary_

def tf_idf(occ, use_idf=True):
	tf = TfidfTransformer(smooth_idf=True, use_idf=use_idf)
	return tf.fit_transform(occ)

def classifier(clf, train, test, target):
	clf.fit(train, target)
	return clf.predict(test)

def main():
	# Charger les données 20-newsgroups
	files = datasets.load_files('20-newsgroups', encoding="utf8", decode_error='ignore')

	# Pré-traitement des données
	occ, vocabulary = bow(files.data)
	tfidf = tf_idf(occ)

	# Split des données en 2: apprentissage, test
	X_train, X_test, y_train, y_test = train_test_split(
		tfidf, files.target, test_size=0.0025)

	# Apprentissage
	clf = KNeighborsClassifier(n_neighbors=3, weights='uniform')
	"""
	clf = KNeighborsClassifier(n_neighbors=3, weights='uniform')
	clf = SGDClassifier(
		loss='hinge', penalty='l2', alpha=1e-3, random_state=42,
		max_iter=5, tol=None)
	clf = MultinomialNB(), X_train, X_test, y_train
	"""
	clf.fit(X_train, y_train)
	pred_train = clf.predict(X_train)
	pred_test = clf.predict(X_test)

	# Erreur quadratiques moyennes
	err_train = metrics.mean_squared_error(pred_train, y_train)
	err_test = metrics.mean_squared_error(pred_test, y_test)

	"""
	err_train = metrics.mean_squared_log_error(pred_train, y_train)
	err_test = metrics.mean_squared_log_error(pred_test, y_test)

	err_train = metrics.mean_absolute_error(pred_train, y_train)
	err_test = metrics.mean_absolute_error(pred_test, y_test)

	err_train = myMAE(pred_train, y_train)
	err_test = myMAE(pred_test, y_test)
	"""

	print("Mean square error (train): %.3f " % err_train)
	print("Mean square error (test):  %.3f " % err_test)

	# Visualisation
	U, Sigma, VT = randomized_svd(X_test, n_components=2, n_iter=10, random_state=122)
	X_topics = U * Sigma
	embedding = umap.UMAP().fit_transform(X_topics)
	plt.title('UMAP projection of 20-newsgroups datasets', fontsize=12);
	plt.scatter(embedding[:, 0], embedding[:, 1], c=files.target[:len(embedding)], edgecolor='none')
	plt.show()


if __name__ == '__main__':
	main()