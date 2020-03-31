import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from sklearn import tree
from sklearn import metrics
from sklearn.tree import DecisionTreeRegressor
from sklearn.datasets import load_diabetes
from sklearn.model_selection import train_test_split
from sklearn.model_selection import GridSearchCV

def main():
	# X, y, X_test, y_1 = default_db()
	X, y, X_test, y_1 = diabetes_db()
	graph(X, y, X_test, y_1)

def _get_best_depth(X, y):
	X = pd.DataFrame(X)

	parameters = {
		'splitter':('best','random'),
		'max_depth':np.arange(1,10),
		'min_samples_split':np.arange(2,10),
		'min_samples_leaf':np.arange(1,5)
	}

	X, X_test, y, y_test = train_test_split(X, y, test_size=0.30)
	gs = GridSearchCV(DecisionTreeRegressor(), parameters, cv=5)
	gs.fit(X, y)

	return gs.best_params_['max_depth']

def diabetes_db():
	diabetes = load_diabetes()

	max_depth = 2;
	n_attr = 10
	n_data = (int)(diabetes.data.size / n_attr)
	n_train = (int)(70 * n_data / 100)

	X = diabetes.data[:150]
	y = diabetes.target[:150]

	max_depth = _get_best_depth(X, y)
	print("Profondeur maximale optimale: ", str(max_depth))

	X = diabetes.data[:, np.newaxis, 2][:150]
	y = diabetes.target[:150]
	X, X_test, y, y_test = train_test_split(X, y, test_size=0.30)
	regr = DecisionTreeRegressor(max_depth=max_depth)
	regr.fit(X, y)

	y_1 = regr.predict(X_test)

	print('Erreur quadratique moyenne: ', metrics.mean_squared_error(y_test, y_1))
	return X, y, X_test, y_1

def default_db():
	X = [[0, 0], [2, 2]]
	y = [0.5, 2.5]
	clf = tree.DecisionTreeRegressor()
	clf = clf.fit(X, y)
	clf.predict([[1, 1]])

	# Créer les données d'apprentissage
	rng = np.random.RandomState(1)
	X = np.sort(5 * rng.rand(80, 1), axis=0)
	y = np.sin(X).ravel()
	y[::5] += 3 * (0.5 - rng.rand(16))
	
	# Apprendre le modèle
	regr_1 = DecisionTreeRegressor(max_depth=2)
	regr_1.fit(X, y)

	# Prédiction
	X_test = np.arange(0.0, 5.0, 0.01)[:, np.newaxis]
	y_1 = regr_1.predict(X_test)

	return X, y, X_test, y_1

def graph(X, y, X_test, y_1):
	# Affichage des résultats
	plt.figure()
	plt.scatter(X, y, c="darkorange", label="data")
	plt.plot(X_test, y_1, color="cornflowerblue", label="max_depth=2",
	linewidth=2)
	plt.xlabel("data")
	plt.ylabel("target")
	plt.title("Decision Tree Regression")
	plt.legend()
	plt.show()

if __name__ == '__main__':
	main()
