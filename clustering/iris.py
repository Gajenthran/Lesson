from sklearn.datasets import make_blobs
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
from scipy.cluster.hierarchy import dendrogram, linkage
from sklearn.cluster import AgglomerativeClustering
from sklearn import datasets
from sklearn.decomposition import PCA
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis
from sklearn.metrics import silhouette_samples, silhouette_score
import matplotlib.cm as cm
import numpy as np

iris = datasets.load_iris()

X = iris.data
y = iris.target
target_names = iris.target_names

n_clusters = [2, 3, 4, 5, 6, 7, 8, 9]
best_cluster = 0
msc = -1.0

for nc in n_clusters:
    clusterer = KMeans(n_clusters=nc)
    cluster_labels = clusterer.fit_predict(X)

    silhouette_avg = silhouette_score(X, cluster_labels)
    if silhouette_avg > msc:
    	best_cluster = nc
    	msc = silhouette_avg
    print("For n_clusters =", nc,
          "The average silhouette_score is :", silhouette_avg)


if best_cluster == 0:
	exit(0)

clusterer = KMeans(n_clusters=3)
cluster_labels = clusterer.fit_predict(X)

ACP = PCA(n_components=2).fit(X).transform(X)
ADL = LinearDiscriminantAnalysis(n_components=2).fit(X, y).transform(X)

plt.figure()
colors = ['red', 'green', 'blue']

for color, i, target_name in zip(colors, [0, 1, 2], target_names):
    plt.scatter(ACP[y == i, 0], ACP[y == i, 1], color=color, label=target_name)
plt.legend(loc='best', shadow=False, scatterpoints=1)
plt.title('PCA of IRIS dataset (Original)')

plt.figure()
for color, i, target_name in zip(colors, [0, 1, 2], target_names):
    plt.scatter(ACP[cluster_labels == i, 0], ACP[cluster_labels == i, 1], color=color, label=target_name)
plt.legend(loc='best', shadow=False, scatterpoints=1)
plt.title('PCA of IRIS dataset (KMeans)')

plt.figure()
for color, i, target_name in zip(colors, [0, 1, 2], target_names):
    plt.scatter(ADL[y == i, 0], ADL[y == i, 1], color=color, label=target_name)
plt.legend(loc='best', shadow=False, scatterpoints=1)
plt.title('LDA of IRIS dataset (Original')

plt.figure()
for color, i, target_name in zip(colors, [0, 1, 2], target_names):
    plt.scatter(ADL[cluster_labels == i, 0], ADL[cluster_labels == i, 1], color=color, label=target_name)
plt.legend(loc='best', shadow=False, scatterpoints=1)
plt.title('LDA of IRIS dataset (KMeans)')

plt.show()