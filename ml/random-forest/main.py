import argparse

from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.model_selection import GridSearchCV
from sklearn.tree import export_graphviz, _tree

from subprocess import call
import pandas as pd

from sklearn.metrics import mean_squared_error

def read_file(filename):
    """
        Lire le fichier donné en paramètre.

        :param filename: nom du fichier
    """

    cb = pd.read_csv("files/CarteBancaire.csv")
    return cb.drop(['Class'], axis=1), cb['Class']


def calculate_estimators_errors(rfc, X, y, debug=True):
    """
        Calculer le taux d'erreur des estimators de notre
        Random Forest.

        :param rfc: Random Forest Classifier
        :param X: données d'apprentissage
        :param y: classe des données
        :PARAM debug: afficher les print
    """

    estimator_errors = []
    for tree in rfc.estimators_:
        pred = 1.0 - tree.score(X, y)
        estimator_errors.append(pred)

    error_min = estimator_errors.index(min(estimator_errors))
    error_max = estimator_errors.index(max(estimator_errors))

    if(debug):
        print('Taux d\'erreur des estimators de notre modèle: \
            {}.\n\n'.format(estimator_errors))

        print('E1 = Taux d\'erreur de l\'estimator le plus performant: \
            {} (indice {}).'.format(estimator_errors[error_min], error_min))

        print('E2 = Taux d\'erreur de l\'estimator le moins performant: \
            {} (indice {}).'.format(estimator_errors[error_max], error_max))

        error_rfc = 1.0 - rfc.score(X, y)

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
    """
        Visualiser l'arbre le plus performant et le moins performant
        de notre Random Forest.

        :param estimators: l'arbre le plus performant et 
        le moins performant
        :param out_files: fichiers de sortie
        :param feature_names: fonctionnalités des données
        :param class_names: classe des données
    """

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
    """
        Afficher les règles de decision de l'arbre donné.

        :param tree: arbre de décision
        :param feature_names: les fonctionnalités des données
    """

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
    """
        Trouver les meilleurs paramètres pour avoir un 
        RFC performant.

        :param param_grid: les paramètres que l'on veut
        tester pour le RFC
        :param rfc: Random Forest Classifier
        :param X: données d'apprentissage
        :param y: classe des données
    """

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
        print("{} - {}".format(mean, param))


def argument_parser(filename):
    """
        Parser les arguments de la ligne de commande
        pour récupérer le fichier et l'option debugging
        :param filename: nom du fichier
        :param debug: debugging
    """

    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--filename", type=str, default=filename)
    parser.add_argument("-d", "--debug", default=True)
    args = parser.parse_args()

    args.debug = False if args.debug == "False" else True
    return args.filename, args.debug


def main():
    # Constante
    FILE_OPT = "files/CarteBancaire.csv"

    # Parser les arguments 
    filename, debug = argument_parser(FILE_OPT)

    # Lecture du fichier CarteBancaire.csv
    X, y = read_file("files/CarteBancaire.csv")

    # Séparation des fichiers en données d'apprentissage
    # et données d'entraînement
    X_train, X_test, y_train, y_test = \
        train_test_split(X, y, test_size=0.4)

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

    # Random Forest Classifier 
    rfc = RandomForestClassifier(
        n_estimators=50,
        max_depth=15,
        min_samples_leaf=1,
        min_samples_split=2,
        n_jobs=-1)

    rfc.fit(X_train, y_train)

    # Calculer le taux d'erreur des estimators du RFC
    error_min, error_max = calculate_estimators_errors(
        rfc, X_test, y_test, debug)

    # Visualiser les arbres du RFC
    vizualize_estimators(
        [rfc.estimators_[error_min], rfc.estimators_[error_max]],
        ['best_tree.dot', 'worst_tree.dot'],
        X_train.columns,
        ['0', '1'])

    # Afficher les règles de decision des estimators
    if(debug == True):
        print("Règle de décision pour l'estimator le plus performant: \n")
        show_rules(rfc.estimators_[error_min], X.columns)
        print("Règle de décision pour l'estimator le moins performant: \n")
        show_rules(rfc.estimators_[error_max], X.columns)

    return 0


if __name__ == '__main__':
    main()
