import pandas as pd
from graphviz import Source
from sklearn import tree
from sklearn import tree

def barbecue():
	data = pd.read_csv("barbecue.csv")
	print(data)
	print();
	print(data['barbecue'])
	x_train = data
	y_train = data['barbecue']
	del x_train['barbecue']
	classifier = tree.DecisionTreeClassifier(criterion='entropy')
	classifier.fit(x_train, y_train)
	tree.export_graphviz(classifier, out_file='tree.dot', feature_names=['Meteo', 'Amis', 'Vent', 'Jour'])

def glass(input, output, target, features):
	data = pd.read_csv(input)
	x_train = data
	y_train = data[target]
	del x_train[target]
	classifier = tree.DecisionTreeClassifier(criterion='entropy')
	classifier.fit(x_train, y_train)
	tree.export_graphviz(classifier, out_file=output, feature_names=features)

def main():
	glass("glass.csv", 
		  "glass.dot", 
		  'Type', 
		  ['Id', 
		   'refractive index', 
		   'Sodium', 
		   'Magnesium', 
		   'Aluminium', 
		   'Silicon', 
		   'Potassium', 
		   'Calcium', 
		   'Barium', 
		   'Iron'
		  ]);

if __name__ == '__main__':
 main()