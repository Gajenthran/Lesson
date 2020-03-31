import pandas as pd
import numpy as np
from scipy.sparse import csr_matrix
from sklearn.neighbors import NearestNeighbors

class MR_KNN:
	"""
		Système de recommendation basé sur l'algorithme KNN (K plus proches voisins).
		On prendra en compte les films ayant un certain nombre de votes pour éviter 
		des résultats biasés.
	"""
	def __init__(self, movies_db, ratings_db, nb_ratings=20, K=5):
		"""
			Initialisation des données en fusionnant la base de données
			ratings et movies, ainsi que les valeurs pour KNN.
		"""
		movies_db = movies_db.drop([19730, 29503, 35587])
		movies_db['movieId'] = movies_db['movieId'].astype('int')
		ratings_db['movieId'].isin(movies_db['movieId']).sum()
		self.data = pd.merge(ratings_db, movies_db [['title','movieId']], \
			                  left_on='movieId', \
			                  right_on='movieId')
		self.K = K
		self.nb_ratings = nb_ratings


	def set_ratings_count(self):
		"""
			Calculer le nombre de votes pour les films.
		"""
		ratings_count = self.data.groupby(by="title")['rating'].count().reset_index().rename(columns={'rating':'nb_ratings'})[['title','nb_ratings']]
		self.data = pd.merge(self.data, ratings_count, on='title', how='left')

	def knn(self, movie='Batman Returns'):
		"""
			Appliquer l'algorithme de KNN à l'aide de la librairie sklearn.
		"""
		self.set_ratings_count()

		self.data = self.data[(self.data['nb_ratings'] >= self.nb_ratings)]
		if not self.data[self.data.duplicated(['userId','title'])].empty:
			self.data = self.data.drop_duplicates(['userId','title'])
		self.data = self.data.pivot(index='title',columns='userId',values='rating').fillna(0)

		data_csr_matrix = csr_matrix(self.data)
		knn = NearestNeighbors(metric='cosine', algorithm='brute')
		knn.fit(data_csr_matrix)
		dist, idx = knn.kneighbors(self.data.loc['Batman Returns'].values.reshape(1, -1), n_neighbors=self.K)

		print("Les films recommandés pour " + movie)
		for i in range(1, len(dist.flatten())):
			print(str(i) + '. ' + self.data.index[idx.flatten()[i]])




