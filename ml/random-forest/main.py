from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.model_selection import GridSearchCV
from sklearn.model_selection import RepeatedStratifiedKFold
from sklearn.tree import export_graphviz, _tree

from subprocess import call
import csv
import pandas as pd

from sklearn.metrics import mean_squared_error

def read_file(filename):
    cb = pd.read_csv("files/CarteBancaire.csv")
    return cb.drop(['Class'], axis=1), cb['Class']

def calculate_estimators_errors(rfc, X, y, debug=True):
    estimator_errors = []
    for tree in rfc.estimators_:
        pred = tree.predict(X)
        estimator_errors.append(mean_squared_error(y, pred))

    error_min = estimator_errors.index(min(estimator_errors))
    error_max = estimator_errors.index(max(estimator_errors))

    if(debug):
        print('Taux d\'erreur des estimators de notre modèle: \
            {}.\n\n'.format(estimator_errors))

        print('E1 = Taux d\'erreur de l\'estimator le plus performant: \
            {} (indice {}).'.format(estimator_errors[error_min], error_min))

        print('E2 = Taux d\'erreur de l\'estimator le moins performant: \
            {} (indice {}).'.format(estimator_errors[error_max], error_max))

        pred_rfc = rfc.predict(X)
        error_rfc = mean_squared_error(y, pred_rfc)

        print('RFC = Taux d\'erreur du modèle RandomForestClassifier: \
            {}.'.format(error_rfc))

        error_moy = (estimator_errors[error_min] + estimator_errors[error_max]) / 2.0

        print('ME = La moyenne des taux d\'erreur des deux estimator: \
            {}.'.format(error_moy))

        diff_error_moy_rfc = abs(error_rfc - error_moy)

        print('DIFF_MOY_RFC: \
            {}.'.format(diff_error_moy_rfc))

    return error_min, error_max

def vizualize_estimators(estimators, out_files, feature_names, class_names):
    for i in range(0, len(estimators)):
        export_graphviz(
            estimators[i],
            out_file=out_files[i],
            feature_names=feature_names,
            class_names=class_names
        )

    call(['dot', '-Tpng', 'worst_tree.dot', '-o', 'worst_tree.png'])
    call(['dot', '-Tpng', 'best_tree.dot', '-o', 'best_tree.png'])

def show_rules(tree, feature_names):
    tree_ = tree.tree_
    feature_name = [
        feature_names[i] if i != _tree.TREE_UNDEFINED else "undefined!"
        for i in tree_.feature
    ]
    print("\n\ndef tree({}):".format(", ".join(feature_names)))

    def recurse(node, depth):
        indent = "  " * depth
        if tree_.feature[node] != _tree.TREE_UNDEFINED:
            name = feature_name[node]
            threshold = tree_.threshold[node]
            print("{}if {} <= {}:".format(indent, name, threshold))
            recurse(tree_.children_left[node], depth + 1)
            print("{}else:  # if {} > {}".format(indent, name, threshold))
            recurse(tree_.children_right[node], depth + 1)
        else:
            print("{}return {}".format(indent, tree_.value[node]))

    recurse(0, 1)

def tuning_rfc_param(param_grid, rfc, X, y):
    CV_rfc = GridSearchCV(
        estimator=rfc, 
        param_grid=param_grid,
        verbose=1,
        n_jobs=-1,
        cv=2)

    CV_rfc.fit(X, y)

    print("Best: {} using {}".format(CV_rfc.best_score_, CV_rfc.best_params_))
    means = CV_rfc.cv_results_['mean_test_score']
    params = CV_rfc.cv_results_['params']
    for mean, param in zip(means, params):
        print("{} - {}".format(mean, stdev, param))

def main():
    X, y = read_file("files/CarteBancaire.csv")

    X_train, X_test, y_train, y_test = \
        train_test_split(X, y, test_size=0.4)

    print(X_train.columns)
    """
    param_grid = {
        "n_estimators": [10, 50, 100],
        "max_depth": [5, 8, 15],
        "min_samples_split": [2, 5, 10, 30],
        "min_samples_leaf": [1, 2, 5, 10]
    }
    rfc = RandomForestClassifier(
        n_estimators=50,
        max_depth=15,
        min_samples_leaf=1,
        min_samples_split=2,
        n_jobs=-1)
    tuning_rfc_param(param_grid, rfc, X_train, y_train)
    """

    rfc = RandomForestClassifier(
        n_estimators=50,
        max_depth=15,
        min_samples_leaf=1,
        min_samples_split=2,
        n_jobs=-1)

    rfc.fit(X_train, y_train)

    error_min, error_max = calculate_estimators_errors(rfc, X_test, y_test)

    vizualize_estimators(
        [rfc.estimators_[error_min], rfc.estimators_[error_max]],
        ['best_tree.dot', 'worst_tree.dot'],
        X_train.columns,
        ['0', '1'])

    show_rules(rfc.estimators_[error_min], X.columns)
    show_rules(rfc.estimators_[error_max], X.columns)

    return 0


if __name__ == '__main__':
    main()
