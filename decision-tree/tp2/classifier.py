from sklearn.datasets import load_iris
from sklearn import tree
import pydotplus


def main():
	iris = load_iris()

	n_attr = 4
	n_data = iris.data.size / 4
	n_train = (int)(70 * n_data / 100)

	clf = tree.DecisionTreeClassifier(max_depth=2,  min_samples_leaf=4)

	clf = clf.fit(iris.data[:n_train], iris.target[:n_train])
	print(clf.predict(iris.data[n_train:, :]))
	generate_pdf(iris, clf)


def generate_pdf(iris, clf):
	dot_data = tree.export_graphviz(clf, out_file=None,
	 feature_names=iris.feature_names,
	 class_names=iris.target_names,
	 filled=True, rounded=True,
	 special_characters=True)
	graph = pydotplus.graph_from_dot_data(dot_data)
	graph.write_pdf("iris.pdf")


if __name__ == '__main__':
	main()