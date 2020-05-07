from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.model_selection import GridSearchCV
from sklearn.model_selection import RepeatedStratifiedKFold
from sklearn.tree import export_graphviz

from subprocess import call
import csv
import pandas as pd

from sklearn.metrics import mean_squared_error

def main():
	cb = pd.read_csv("files/CarteBancaire.csv")

	X_train, X_test, y_train, y_test = train_test_split(
		cb.drop(['Class'], axis=1), cb['Class'], test_size=0.4)

	rfc = RandomForestClassifier(
		n_estimators=50,
		max_depth=15,
		min_samples_leaf=1,
		min_samples_split=2,
		n_jobs=-1)

	print("clf")
	rfc.fit(X_train, y_train)

	estimator_errors = []
	for tree in rfc.estimators_:
		pred = tree.predict(X_test)
		estimator_errors.append(mean_squared_error(y_test, pred))

	print('Taux d\'erreur des estimators de notre modèle: \
		{}.\n\n'.format(estimator_errors))

	error_min = estimator_errors.index(min(estimator_errors))
	error_max = estimator_errors.index(max(estimator_errors))

	print('E1 = Taux d\'erreur de l\'estimator le plus performant: \
		{} (indice {}).'.format(estimator_errors[error_min], error_min))

	print('E2 = Taux d\'erreur de l\'estimator le moins performant: \
		{} (indice {}).'.format(estimator_errors[error_max], error_max))

	pred_rfc = rfc.predict(X_test)
	error_rfc = mean_squared_error(y_test, pred_rfc)

	print('RFC = Taux d\'erreur du modèle RandomForestClassifier: \
		{}.'.format(error_rfc))

	error_moy = (estimator_errors[error_min] + estimator_errors[error_max]) / 2.0

	print('ME = La moyenne des taux d\'erreur des deux estimator: \
		{}.'.format(error_moy))

	diff_error_moy_rfc = abs(error_rfc - error_moy)

	print('DIFF_MOY_RFC: \
		{}.'.format(diff_error_moy_rfc))

	# Visualisation
	export_graphviz(
		rfc.estimators_[error_min], 
		out_file='worst_tree.dot',
		feature_names=cb.drop(['Class'], axis=1).columns,
		class_names=['0', '1'])
	export_graphviz(
		rfc.estimators_[error_max], 
		out_file='best_tree.dot',
		feature_names=cb.drop(['Class'], axis=1).columns,
		class_names=['0', '1'])

	call(['dot', '-Tpng', 'worst_tree.dot', '-o', 'worst_tree.png', '-Gdpi=600'])
	call(['dot', '-Tpng', 'best_tree.dot', '-o', 'best_tree.png', '-Gdpi=600'])

	"""
	param_grid = {
		"n_estimators": [10, 50, 100],
		"max_depth": [5, 8],
	}

	CV_rfc = GridSearchCV(
		estimator=rfc, 
		param_grid=param_grid,
		verbose=1,
		n_jobs=-1,
		cv=2)

	CV_rfc.fit(X_train, y_train)

	print("Best: %f using %s" % (CV_rfc.best_score_, CV_rfc.best_params_))
	means = CV_rfc.cv_results_['mean_test_score']
	stds = CV_rfc.cv_results_['std_test_score']
	params = CV_rfc.cv_results_['params']
	for mean, stdev, param in zip(means, stds, params):
		print("%f (%f) with: %r" % (mean, stdev, param))
	"""

	"""
	export_graphviz(
		rfc, 
		out_file='tree.dot', 
		feature_names=['Meteo', 'Amis', 'Vent', 'Jour'],
	)
	"""

	return 0


if __name__ == '__main__':
	main()
