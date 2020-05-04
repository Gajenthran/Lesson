from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.model_selection import GridSearchCV
from sklearn.model_selection import RepeatedStratifiedKFold

import csv
import pandas as pd

cb = pd.read_csv("CarteBancaire.csv")

X_train, X_test, y_train, y_test = train_test_split(
	cb.drop(['Class'], axis=1), cb['Class'], test_size=0.4)

clf = RandomForestClassifier()

param_grid = {
	"n_estimators": [10, 50, 100],
	"max_depth": [5, 8, 15],
	"min_samples_split": [2, 5, 10, 30],
	"min_samples_leaf": [1, 2, 5, 10]
}

CV_rfc = GridSearchCV(
	estimator=clf, 
	param_grid=param_grid,
	verbose=1,
	n_jobs=-1,
	cv=3)

CV_rfc.fit(X_train, y_train)

print("Best: %f using %s" % (CV_rfc.best_score_, CV_rfc.best_params_))
means = CV_rfc.cv_results_['mean_test_score']
stds = CV_rfc.cv_results_['std_test_score']
params = CV_rfc.cv_results_['params']
for mean, stdev, param in zip(means, stds, params):
    print("%f (%f) with: %r" % (mean, stdev, param))
