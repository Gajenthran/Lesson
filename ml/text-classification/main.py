import argparse
import re
import numpy as np

from sklearn import metrics, datasets
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.model_selection import train_test_split

from sklearn.neighbors import KNeighborsClassifier
from sklearn.model_selection import GridSearchCV

from sklearn.utils.extmath import randomized_svd
import umap
import matplotlib.pyplot as plt

def bow(data, stop_words=None):
	"""
		Bag of words pour calculer les occurences des mots
		:param data: 20-newsgroups textes.
		:param vocabulary: tous les mots utilisés, par défaut
		à None pour utiliser CountVectorizer
	"""

	cv = CountVectorizer(vocabulary=None, stop_words=stop_words)
	occ = cv.fit_transform(data)
	return occ, cv.vocabulary_


def histogram(files, nb_words=75):
	"""
		Histogramme pour calculer les mots les
		plus utilisés dans chaque thème.
		:param files: l'ensemble des données
		:param nb_words: le nombre de mots les plus
		utilisés à afficher
	"""

	print("Histogramme des mots utilisés dans 20-ng par catégorie:\n")
	for i in range(0, len(files.target_names)):
		class_20ng = []
		for j in range(0, len(files.data)):
			if files.target[j] == i:
				class_20ng.append(files.data[j])

		occ, vocabulary = bow(list(class_20ng), 'english')
		sum_words = occ.sum(axis=0)
		words_freq = [(word, sum_words[0, idx]) for word, idx in vocabulary.items()]
		words_freq = sorted(words_freq, key = lambda x: x[1], reverse=True)[:nb_words]
		print("{}: \n {}\n\n".format(files.target_names[i], words_freq))


def tf_idf(occ, use_idf=True):
	"""
		Transformer les occurences en fréquences.
		:param occ: les occurences de chaque mot dans
		les textes
		:param use_idf: activation du paramètes use_idf
		de TfidfTransformer
	"""

	tf = TfidfTransformer(smooth_idf=True, use_idf=use_idf)
	return tf.fit_transform(occ)


def calculate_error(y_test, pred, metric='mse'):
	"""
		Afficher le taux d'erreur en utilisant la méthode
		MSE (mais également d'autres comme MAE, RMSE mais
		la valeur retournée sera celle de MSE)
		:param y_test: classes des données tests
		:param pred: classes prédites par le modèle
	"""

	err = None
	if metric == 'mse':
		err = metrics.mean_squared_error(y_test, pred)
	elif metric == 'mae':
		err = metrics.mean_absolute_error(y_test, pred)
	else: 
		err = np.sqrt(((pred - y_test) ** 2).mean())

	print("- Calculate error for kNN model:")
	print("MSE (test) :  {}".format(metrics.mean_squared_error(y_test, pred)))
	print("MAE (test) :  {}".format(metrics.mean_absolute_error(y_test, pred)))
	print("RMSE (test):  {}\n".format(np.sqrt(((pred - y_test) ** 2).mean())))

	return err


def tuning_clf_param(param_grid, clf, X, y):
	"""
		Trouver les meilleurs paramètres pour avoir un 
		RFC performant.

		:param param_grid: les paramètres que l'on veut
			tester pour le RFC
		:param rfc: Random Forest Classifier
		:param X: données d'apprentissage
		:param y: classe des données
	"""

	CV_clf = GridSearchCV(
		estimator=clf, 
		param_grid=param_grid,
		verbose=1,
		n_jobs=-1,
		cv=2)

	CV_clf.fit(X, y)

	print("- Tuning kNN parameters with GridSearchCV...")
	print("Best: {} using {}".format(CV_clf.best_score_, CV_clf.best_params_))
	means = CV_clf.cv_results_['mean_test_score']
	params = CV_clf.cv_results_['params']
	for mean, param in zip(means, params):
		print("{} - {}".format(mean, param))
	print("")
	return CV_clf


def evaluate_k_value(X_train, X_test, y_train, y_test):
	"""
		Evaluer le taux d'erreur de selon le nombre 
		de voisins du kNN.
		:param X_train: données d'apprentissage
		:param X_test: données tests
		:param y_train: classes des données d'apprentissage
		:param y_test: classes des données tests
		:param pred: classes prédites par le modèle
	"""

	evals = []
	K_range = 20

	for i in range(1, K_range):
		knn = KNeighborsClassifier(n_neighbors=i)
		knn.fit(X_train, y_train)
		pred = knn.predict(X_test)
		evals.append(np.mean(pred != y_test))

	plt.title('Mean Error with different K value (w/ 20-ng datasets)')
	plt.xlabel('K-neighbors')
	plt.ylabel('Mean Error')
	plt.plot(range(1, K_range), evals, color='red', markersize=10)
	plt.show()


def plot_confusion_matrix(cm, target_names, y_test, pred):
	"""
		Afficher la matrice de confusion pour le modèle
		kNN.
		:param cm: matrice de confusion
		:param target_names: ensemble des classes
		:param y_test: classes de chaque texte
		:param pred: classes prédites par le modèle
	"""

	plt.figure(figsize=(10, 8))
	plt.imshow(cm, interpolation='nearest', cmap=plt.cm.Reds)
	plt.title('Confusion matrix (kNN w/ 20-ng datasets)')
	plt.ylabel('y_test')
	plt.xlabel('y_pred')
	plt.colorbar()
	tick_marks = np.arange(len(target_names))
	plt.xticks(tick_marks, target_names, rotation=45)
	plt.yticks(tick_marks, target_names)

	plt.show()

def plot_model_label(X_test, y_test, pred):
	"""
		Afficher les labels pour le modèle
		kNN dans un graphique 2D.
		:param X_test: données tests
		:param y_test: classes de chaque texte
		:param pred: classes prédites par le modèle
	"""

	U, Sigma, VT = randomized_svd(X_test, n_components=2, n_iter=10, random_state=122)
	X_topics = U * Sigma
	embedding = umap.UMAP().fit_transform(X_topics)
	plt.title('UMAP projection (kNN w/ 20-ng datasets)', fontsize=12)
	plt.subplot(211)
	plt.scatter(embedding[:, 0], embedding[:, 1], c=y_test, edgecolor='none')
	plt.subplot(212)
	plt.scatter(embedding[:, 0], embedding[:, 1], c=pred, edgecolor='none')
	plt.show()


def find_confusion_names(cm, target_names, threshold):
	"""
		Afficher les classes qui comportent des confusions
		importantes selon le seuil donnée.
		:param cm: matrice de confusion
		:param target_names: nom de l'ensemble des classes
		:param threshold: seuil pour juger les confusions
	"""

	confusions = [-1] * len(target_names)

	print("- Confusion names for kNN model:")
	for i in range(0, len(cm)):
		if(cm[i][i] < threshold):
			s_best = sorted(cm[i], reverse=True)[1]
			s_idx = cm[i].tolist().index(s_best)
			print("CM between {} ({}) and {} ({})".format(
				target_names[i], cm[i][i],
				target_names[s_idx], s_best))
			confusions[cm[i].tolist().index(s_best)]
	print("")
	return confusions


def argument_parser(test_size, threshold):
	"""
		Parser les arguments de la ligne de commande
		pour choisir les différents paramètres. 
		:param test_size: part des données tests
		:param threshold: seuil pour CM
	"""

	parser = argparse.ArgumentParser()
	parser.add_argument("-f", "--filename", type=str, default="")
	parser.add_argument("-g", "--grid",  action="store_true")
	parser.add_argument("-ts", "--test_size", type=float, default=0.3)
	parser.add_argument("-th", "--threshold", type=float, default=threshold)
	parser.add_argument("-hist", "--histogram",  action="store_true")
	parser.add_argument("-eval", "--eval_k",  action="store_true")

	args = parser.parse_args()

	print(args)
	return \
		args.filename, \
		args.grid, \
		args.test_size, \
		args.threshold, \
		args.histogram, \
		args.eval_k


def main():
	# paramètres par défaut
	threshold = 0.75
	filename = '20-newsgroups'
	test_size = 0.3

	files = None

	# Parser les arguments
	filename, grid_opt, test_size, threshold, histogram_, eval_k = \
		argument_parser(test_size, threshold)

	# Charger les données 20-newsgroups
	if filename != '':
		files = datasets.load_files(filename, encoding="utf8", decode_error='ignore')
	else:
		files = datasets.fetch_20newsgroups(subset='all')

	if histogram_:
		histogram(files)

	# Pré-traitement des données
	occ, vocabulary = bow(files.data)
	tfidf = tf_idf(occ)

	# Split des données en 2: apprentissage, test
	X_train, X_test, y_train, y_test = train_test_split(
		tfidf, files.target, test_size=test_size)

	# Apprentissage
	clf = KNeighborsClassifier()
	# clf = KNeighborsClassifier(leaf_size=1, n_neighbors=1, p=2)
	clf.fit(X_train, y_train)
	pred = clf.predict(X_test)

	# Erreur quadratiques moyennes
	calculate_error(y_test, pred)

	# Visualisation
	plot_model_label(X_test, y_test, pred)

	# Afficher la matrice de confusion
	cm = metrics.confusion_matrix(y_test, pred, normalize='pred')
	plot_confusion_matrix(cm, files.target_names, y_test, pred)
	find_confusion_names(cm, files.target_names, threshold)

	# Evaluer les erreurs selon le nombre de voisins K
	if eval_k:
		evaluate_k_value(X_train, X_test, y_train, y_test)

	tuned_clf = None
	print("-- Improving kNN model...")
	if grid_opt == True:
		# Chercher les meilleurs paramètres de KNN
		grid_param = dict(
			leaf_size=list(range(1,20)),
			n_neighbors=list(range(1, 10)),
			p=[1, 2])

		tuned_clf = tuning_clf_param(grid_param, clf, X_train, y_train)
		tuned_clf.fit(X_train, y_train)
	else: 
		# Résultat obtenu par le GridSearchCV pour éviter le temps de calcul
		tuned_clf = KNeighborsClassifier(leaf_size=1, n_neighbors=1, p=2)
		tuned_clf.fit(X_train, y_train)

	tuned_pred = tuned_clf.predict(X_test)

	# Erreur quadratiques moyennes
	calculate_error(y_test, tuned_pred)

	# Visualisation
	plot_model_label(X_test, y_test, tuned_pred)

	# Matrice de confusion
	cm = metrics.confusion_matrix(y_test, tuned_pred, normalize='pred')
	plot_confusion_matrix(cm, files.target_names, y_test, pred)
	find_confusion_names(cm, files.target_names, threshold)

	return 0


if __name__ == '__main__':
	main()