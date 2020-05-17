import argparse

from sklearn.datasets import make_blobs
from sklearn.datasets import load_iris
from sklearn.cluster import KMeans
from scipy.cluster.hierarchy import dendrogram, linkage
from sklearn.cluster import AgglomerativeClustering
from sklearn.decomposition import PCA
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis
from sklearn.metrics import silhouette_samples, silhouette_score
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np


class ClusteringBlob:
  def __init__(self, n_samples, n_features, n_clusters):
    self.X, self.y = make_blobs(
      n_samples=n_samples, 
      n_features=n_features, 
      centers=n_clusters
    )

    self.kmeans = KMeans(n_clusters)
    self.km_models = self.kmeans.fit(self.X)
    self.km_labels = self.kmeans.predict(self.X)

    self.linkage = linkage(self.X, method='ward')
    self.ahc = AgglomerativeClustering(
      n_clusters=n_clusters, 
      affinity='euclidean', 
      linkage='ward'
    )

    self.acp = PCA(
      n_components=2).fit(self.X).transform(self.X)

    self.adl = LinearDiscriminantAnalysis(
      n_components=n_features).fit(self.X, self.y).transform(self.X)

  def plt_blobs(self):
    plt.figure()
    plt.title('Blobs (modèle original)')
    plt.scatter(
      self.X[:, 0], self.X[:, 1], 
      c=self.y, edgecolor='k'
    )
    plt.show()

  def plt_kmeans(self):
    C = self.kmeans.cluster_centers_

    plt.figure()
    plt.title('KMeans')
    plt.scatter(
      C[:, 0],
      C[:, 1],
      marker='*',
      c='red',
      s=1000
    )
    plt.show()

  def plt_hierarchy_classification(self):
    plt.figure()
    plt.title('Fonction de classification hiérarchique')
    dendrogram(self.linkage)
    plt.show()

  def plt_ahc(self):
    self.ahc.fit_predict(self.X)
    plt.figure()
    plt.title('AHC')
    plt.scatter(
      self.X[:, 0],
      self.X[:, 1], 
      c=self.ahc.labels_
    )
    plt.show()

  def plt_acp(self):
    colors = ['red', 'green', 'blue', 'magenta'];
    plt.figure()
    plt.title('ACP (KMeans)')
    for c, k in zip(colors, [0, 1, 2, 3]):
      plt.scatter(
        self.acp[self.kmeans.labels_ == k, 0], 
        self.acp[self.kmeans.labels_ == k, 1], 
        c=c
      )

    plt.figure()
    plt.title('ACP (Original)')
    for c, k in zip(colors, [0, 1, 2, 3]):
      plt.scatter(
        self.acp[self.y == k, 0], 
        self.acp[self.y == k, 1], 
        c=c
      )
    plt.show()

  def plt_adl(self):
    colors = ['red', 'green', 'blue', 'magenta'];
    plt.figure()
    plt.title('ADL (KMeans)')
    for c, k in zip(colors, [0, 1, 2, 3]):
      plt.scatter(
        self.adl[self.kmeans.labels_ == k, 0],
        self.adl[self.kmeans.labels_ == k, 1],
        c=c
      )

    plt.figure()
    plt.title('ADL (Original)')
    for c, k in zip(colors, [0, 1, 2, 3]):
      plt.scatter(
        self.adl[self.y == k, 0],
        self.adl[self.y == k, 1],
        c=c
      )
    plt.show()

  def silhouette_clusters(self, draw=True):
    range_clusters = np.arange(2, 10)
    for nc in range_clusters:
      kmeans = KMeans(n_clusters=nc)
      km_labels = kmeans.fit_predict(self.X)
      silhouette_avg = silhouette_score(self.X, km_labels)
      print("For n_clusters =", nc,
          "The average silhouette_score is :", silhouette_avg)

      fig, (ax1, ax2) = plt.subplots(1, 2)
      fig.set_size_inches(18, 7)
      ax1.set_ylim([0, len(self.X) + (nc + 1) * 10])
      sample_silhouette_values = silhouette_samples(self.X, km_labels)
      y_lower = 10

      if draw == True:
        for i in range(nc):
          # Aggregate the silhouette scores for samples belonging to
          # cluster i, and sort them
          ith_cluster_silhouette_values = \
            sample_silhouette_values[km_labels == i]

          ith_cluster_silhouette_values.sort()

          size_cluster_i = ith_cluster_silhouette_values.shape[0]
          y_upper = y_lower + size_cluster_i

          color = cm.nipy_spectral(float(i) / nc)
          ax1.fill_betweenx(np.arange(y_lower, y_upper),
                    0, ith_cluster_silhouette_values,
                    facecolor=color, edgecolor=color, alpha=0.7)

          # Label the silhouette plots with their cluster numbers at the middle
          ax1.text(-0.05, y_lower + 0.5 * size_cluster_i, str(i))

          # Compute the new y_lower for next plot
          y_lower = y_upper + 10  # 10 for the 0 samples

        ax1.set_title("The silhouette plot for the various clusters.")
        ax1.set_xlabel("The silhouette coefficient values")
        ax1.set_ylabel("Cluster label")

        # The vertical line for average silhouette score of all the values
        ax1.axvline(x=silhouette_avg, color="red", linestyle="--")

        ax1.set_yticks([])  # Clear the yaxis labels / ticks
        ax1.set_xticks([-0.1, 0, 0.2, 0.4, 0.6, 0.8, 1])

        # 2nd Plot showing the actual clusters formed
        colors = cm.nipy_spectral(km_labels.astype(float) / nc)
        ax2.scatter(
          self.X[:, 0], 
          self.X[:, 1], 
          marker='.', s=30, 
          lw=0, alpha=0.7,
          c=colors, edgecolor='k'
        )

        # Labeling the clusters
        centers = kmeans.cluster_centers_
        # Draw white circles at cluster centers
        ax2.scatter(
          centers[:, 0], 
          centers[:, 1], 
          marker='o', c="white", 
          alpha=1, s=200, edgecolor='k'
        )

        for i, c in enumerate(centers):
          ax2.scatter(
            c[0], 
            c[1], 
            marker='$%d$' % i, alpha=1,
            s=50, edgecolor='k'
          )

        ax2.set_title("The visualization of the clustered data.")
        ax2.set_xlabel("Feature space for the 1st feature")
        ax2.set_ylabel("Feature space for the 2nd feature")

        plt.suptitle(("Silhouette analysis for KMeans clustering on sample data "
                "with n_clusters = %d" % nc),
               fontsize=14, fontweight='bold')
        plt.show()


class ClusteringIris:
  def __init__(self, n_clusters=3):
    self.n_clusters = n_clusters

    iris = load_iris()
    self.X = X = iris.data
    self.y = iris.target
    self.target_names = iris.target_names
    
    self.kmeans = KMeans(n_clusters=n_clusters)
    self.km_labels = self.kmeans.fit_predict(self.X)

    self.linkage = linkage(self.X, method='ward')

    self.acp = PCA(n_components=2).fit(self.X).transform(self.X)

    self.adl = LinearDiscriminantAnalysis(
      n_components=2).fit(self.X, self.y).transform(self.X)

  def plt_iris(self):
    plt.figure()
    plt.title('Blobs (modèle original)')
    plt.scatter(
      self.X[:, 0], self.X[:, 1], 
      c=self.y, edgecolor='k'
    )
    plt.show()

  def plt_kmeans(self):
    C = self.kmeans.cluster_centers_

    plt.figure()
    plt.title('KMeans')
    plt.scatter(
      C[:, 0],
      C[:, 1],
      marker='*',
      c='red',
      s=1000
    )
    plt.show()

  def plt_hierarchy_classification(self):
    plt.figure()
    plt.title('Fonction de classification hiérarchique')
    dendrogram(self.linkage)
    plt.show()

  def silhouette_clusters(self):
    range_clusters = np.arange(2, 10)
    cluster = 0
    msc = -1.0

    for nc in range_clusters:
      kmeans = KMeans(n_clusters=nc)
      km_labels = kmeans.fit_predict(self.X)

      silhouette_avg = silhouette_score(self.X, km_labels)
      if silhouette_avg > msc:
        cluster = nc
        msc = silhouette_avg

      print("For n_clusters =", nc,
              "The average silhouette_score is :", silhouette_avg)


    if cluster == 0:
      return 3; # retourne 3 par défaut
    return cluster;

  def plt_acp(self):
    plt.figure()
    colors = ['red', 'green', 'blue']

    for color, i, target_name in zip(colors, [0, 1, 2], self.target_names):
      plt.scatter(
        self.acp[self.y == i, 0], 
        self.acp[self.y == i, 1], 
        color=color, 
        label=target_name
      )
    plt.legend(loc='best', shadow=False, scatterpoints=1)
    plt.title('PCA of IRIS dataset (Original)')

    plt.figure()
    for color, i, target_name in zip(colors, [0, 1, 2], self.target_names):
        plt.scatter(
          self.acp[self.km_labels == i, 0],
          self.acp[self.km_labels == i, 1],
          color=color,
          label=target_name)
    plt.legend(loc='best', shadow=False, scatterpoints=1)
    plt.title('PCA of IRIS dataset (KMeans)')

    plt.show()

  def plt_adl(self):
    colors = ['red', 'green', 'blue']
    plt.figure()
    for color, i, target_name in zip(colors, [0, 1, 2], self.target_names):
      plt.scatter(
        self.adl[self.y == i, 0], 
        self.adl[self.y == i, 1], 
        color=color, 
        label=target_name
      )
    plt.legend(loc='best', shadow=False, scatterpoints=1)
    plt.title('LDA of IRIS dataset (Original')

    plt.figure()
    for color, i, target_name in zip(colors, [0, 1, 2], self.target_names):
      plt.scatter(
        self.adl[self.km_labels == i, 0], 
        self.adl[self.km_labels == i, 1], 
        color=color, 
        label=target_name
      )
    plt.legend(loc='best', shadow=False, scatterpoints=1)
    plt.title('LDA of IRIS dataset (KMeans)')

    plt.show()

def argument_parser():
    """
      Parser les arguments de la ligne de commande
      pour choisir l'exercice. Par défaut "iris" mais on
      peut choisir de tester blob avec l'option -b ou --blob.
    """

    parser = argparse.ArgumentParser()
    parser.add_argument("-b", "--blob", help="clustering blobs",
                        action="store_true")
    args = parser.parse_args()

    print(args.blob)
    return args.blob

def main():
  # par défaut à iris
  CLUSTER_OPT = "iris"

  cluster_iris = argument_parser(CLUSTER_OPT)

  if(cluster_iris):
    ci = ClusteringIris(
      n_clusters=3
    )

    ci.plt_iris()
    ci.plt_kmeans()
    ci.plt_hierarchy_classification()
    ci.silhouette_clusters()
    ci.plt_acp()
    ci.plt_adl()

  else:
    cb = ClusteringBlob(
      n_samples=300,
      n_features=2,
      n_clusters=4
    )
    cb.plt_blobs()
    cb.plt_kmeans()
    cb.plt_hierarchy_classification()
    cb.plt_acp()
    cb.plt_adl()
    cb.plt_ahc()
    cb.silhouette_clusters(draw=False)

  return 0

if __name__ == '__main__':
  main()