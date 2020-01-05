import pandas as pd
from sklearn.feature_extraction.text import CountVectorizer, TfidfVectorizer
from sklearn.metrics.pairwise import linear_kernel, cosine_similarity

class MR_ContentBased:
	def __init__(self, movies_db, links_db, n=5):
		links_db = links_db[links_db['tmdbId'].notnull()]['tmdbId'].astype('int')
		movies_db = movies_db.drop([19730, 29503, 35587])
		movies_db['id'] = movies_db['id'].astype('int')
		self.data = movies_db[movies_db['id'].isin(links_db)];
		self.features = ['tagline', 'title', 'overview'];
		self.n = n;

	def set_indices(self):
		self.data = self.data.reset_index()
		titles = self.data['title']
		return pd.Series(self.data.index, index=self.data['title']);

	def set_nb_recommandations(self, n):
		self.n = n;

	def get_content_based_movies(self, movie='Batman Forever', tfid=True):
		self.data['tagline'] = self.data['tagline'].fillna('')
		self.data['content_based'] = (self.data['overview'] + self.data['tagline']).fillna('');
		cs = None;
		if(tfid):
			tfid = TfidfVectorizer(analyzer='word',ngram_range=(1, 2),min_df=0, stop_words='english')
			tfidm = tfid.fit_transform(self.data['content_based']);
			cs = linear_kernel(tfidm, tfidm)
		else:
			cv = CountVectorizer();
			cm = cv.fit_transform(self.data["content_based"]);
			cs = cosine_similarity(cm);

		indices = self.set_indices();
		index = indices[movie];
		similar_mv = list(enumerate(cs[index]));
		sorted_mv = sorted(similar_mv, key=lambda x: x[1], reverse=True);
		movie_indices = [i[0] for i in sorted_mv];
		res = self.data.iloc[movie_indices][['title']].head(self.n);
		print(res);
