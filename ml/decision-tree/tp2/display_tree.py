import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import load_iris
from sklearn.tree import DecisionTreeClassifier

def main():
	# Paramètres
	n_classes = 3
	plot_colors = "bry" # blue-red-yellow
	plot_step = 0.02

	# Charger les données
	iris = load_iris()

	# Choisir les attributs longueur et largeur des pétales
	pair = [1, 2]

	# Garder seulement les deux attributs
	X = iris.data[:, pair]
	y = iris.target

	clf, xx, yy, Z = prediction(iris, X, y, plot_step)
	display(iris, X, y, xx, yy, Z, pair, n_classes, plot_colors)

def prediction(iris, X, y, plot_step):
	# Apprentissage de l'arbre
	clf = DecisionTreeClassifier().fit(X, y)

	# Affichage de la surface de décision
	x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
	y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
	xx, yy = np.meshgrid(np.arange(x_min, x_max, plot_step), np.arange(y_min,
	y_max, plot_step))

	Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
	Z = Z.reshape(xx.shape)

	return clf, xx, yy, Z

def display(iris, X, y, xx, yy, Z, pair, n_classes, plot_colors):
	cs = plt.contourf(xx, yy, Z, cmap=plt.cm.Paired)
	plt.xlabel(iris.feature_names[pair[0]])
	plt.ylabel(iris.feature_names[pair[1]])
	plt.axis("tight")

	# Affichage des points d'apprentissage
	for i, color in zip(range(n_classes), plot_colors):
	 idx = np.where(y == i)
	 plt.scatter(X[idx, 0], X[idx, 1], c=color, label=iris.target_names[i],
	cmap=plt.cm.Paired)
	plt.axis("tight")
	plt.suptitle("Decision surface of a decision tree using paired features")
	plt.legend()
	plt.savefig('fig.png')
	plt.show()

if __name__ == '__main__':
	main()