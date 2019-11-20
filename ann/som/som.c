#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "som.h"
#include "ll.h"

/** \brief Initialise le vecteur représentant l'ordre
 * de passage des données lors de la phase d'apprentissage
 *
 * \param size nombre de données
 * 
 * \return vecteur représentant l'ordre de passage des données.
 */
int * init_shuffle(int size) {
  srand(time(NULL));
  int i;
  int * sh = (int *)malloc(size * sizeof(*sh));
  assert(sh);

  for(i = 0; i < size; i++)
    sh[i] = i;
  shuffle(sh, size);
  return sh;
}

/** \brief Mélange le vecteur représentant l'ordre de passage des données
 * lors de la phase d'apprentissage
 *
 * \param sh   vecteur représentant l'ordre de passage des données
 * \param size taille du vecteur
 */
void shuffle(int * sh, int size) {
  int i, r;
  for(i = 0; i < size; i++) {
    r = rand() % size;
    if(sh[i] != sh[r]) {
      sh[i] ^= sh[r];
      sh[r] ^= sh[i];
      sh[i] ^= sh[r];
    }
  }
}

/** \brief Initialise le réseau de neurones à partir des
 * des données de la bd, en initialisant le coefficient
 * d'apprentissage, le vecteur de données et le rayon
 * de voisinage.
 *
 * \param data données de la bd
 * \param size nombre de données
 * 
 * \return structure de type network_t représentant le 
 * réseau de neurones.
 */
network_t * init_network(data_t * data, int size) {
  int i, j, l, c;
  double avg, itvmax = 0.05, itvmin = 0.02, sum;

  network_t * net = (network_t *)malloc(sizeof *net);
  assert(net);
  net->map = (node_t **)malloc(LINE * sizeof(*net->map));
  assert(net->map);

  for(l = 0; l < LINE; l++) {
    net->map[l] = (node_t *)malloc(COL * sizeof(*net->map[l]));
    assert(net->map[l]);

    for(c = 0; c < COL; c++) {
      for(i = 0; i < NB_VAL; i++) {
        net->map[l][c].w = (double *)malloc(NB_VAL * sizeof(*net->map[l][c].w));
        assert(net->map[l][c].w);
      }
    }
  }

  for(i = 0; i < NB_VAL; i++) {
    sum = 0;
    for(j = 0; j < size; j++) 
      sum += data[j].v[i];
    avg = sum / size;

    for(l = 0; l < LINE; l++)
      for(c = 0; c < COL; c++) {
        net->map[l][c].w[i] = my_rand(avg - itvmin, avg + itvmax);
      }
  }

  net->alpha = 0.7;
  net->nhd_rad = 4;
  return net;
}

/** \brief Phase d'apprentissage des neurones coupé en deux
 * parties: partie compétitive avec la recherche du bmu 
 * et partie coopérative avec la diffusion des règles
 * d'apprentissage au voisinage.
 *
 * \param iterations nombre d'itérations
 * \param net        réseau de neurones
 * \param sh         vecteur représentant l'ordre de passage des données
 * \param data       données
 * \param size       nombre de données
 */
void train(int iterations, network_t * net, int * sh, data_t * data, int size) {
  // modulo, div, itération à faire
  bmu_t bmu;
  int i, it;
  // nombre d'itérations
  for(it = 0; it < iterations; it++) {
    shuffle(sh, size);
    // pour tout i appartenant aux données v de la bd
    for(i = 0; i < size; i++) {
      bmu = find_bmu(net, data[i].v);
      apply_nhd(net, data[i].v, bmu);
    }
    net->alpha = 1.0 - ((double)it / (double)iterations);
  }
}

/** \brief Etiqueter les données après la phase d'apprentissage.
 *
 * \param net  réseau de neurones
 * \param data données
 * \param size nombre de données
 */
void label(network_t * net, data_t * data, int size) {
  int l, c, i, min_i;
  double dist, min_dist;
  for(l = 0; l < LINE; l++) {
    for(c = 0; c < COL; c++) {
      // voir avec bmu structure
      min_dist = euclidean_dist(net->map[l][c].w, data[0].v, NB_VAL);
      for(i = 0; i < size; i++) {
        dist = euclidean_dist(net->map[l][c].w, data[i].v, NB_VAL);
        if(min_dist > dist) {
          min_dist = dist;
          min_i = i;
        }
      }
      net->map[l][c].label = strdup(data[min_i].label);
      // setosa = 1, versicolor = 5, virginica = 4
      printf(" %lu ", strlen(net->map[l][c].label) % 10);
    }
    printf("\n");
  }
}

/** \brief Applique la règle d'apprentissage selon aux voisins
 * du bmu selon le rayon de voisinage attribué. Concernant la
 * règle d'apprentisage, celle-ci fera abstraction de la fonction
 * de voisinage; ainsi, tous les voisins auront le même taux
 * d'apprentissage que les autres.
 *
 * \param net réseau de neurones
 * \param v   vecteur de données de la bd
 * \param bmu structure représentant le bmu (best match unit)
 */
void apply_nhd(network_t * net, double * v, bmu_t bmu) {
  int iv, l, c, l0, c0;
  // pour tout node l, c appartenant à Nhd(i)
  for(l = -(net->nhd_rad - 1); l < net->nhd_rad; l++) {
    for(c = -(net->nhd_rad - 1); c < net->nhd_rad; c++) {
      l0 = bmu.l + l;
      c0 = bmu.c + c;
      if(l0 < 0 || l0 >= LINE || c0 < 0 || c0 >= COL)
        continue;
      for(iv = 0; iv < NB_VAL; iv++) {
        net->map[l0][c0].w[iv] = net->map[l0][c0].w[iv] +
          net->alpha * (v[iv] - net->map[l0][c0].w[iv]);
      }
    }
  }
}

/** \brief Trouver le bmu (best match unit) en comparant les 
 * neurones aux valeurs des données de la bd.
 *
 * \param iterations nombre d'itérations
 * \param net        réseau de neurones
 * \param sh         vecteur représentant l'ordre de passage des données
 * \param data       données
 * \param size       nombre de données
 *
 * \return structure réprésentant le bmu (best match unit)
 */

bmu_t find_bmu(network_t * net, double * v) {
  list_t * bmu_lis = init_list();
  int l, c;
  bmu_t bmu;
  double dist;
  bmu.act = euclidean_dist(v, net->map[0][0].w, NB_VAL);
  insert_list(bmu_lis, 0, 0);
  for(l = 0; l < LINE; l++) {
    for(c = 0; c < COL; c++) {
      dist = euclidean_dist(v, net->map[l][c].w, NB_VAL);
      if(bmu.act > dist) {
        reinit_list(bmu_lis, l, c);
        bmu.act = dist;
        bmu.l = l; bmu.c = c;
      }
      if(bmu.act == dist)
        insert_list(bmu_lis, l, c);
    }
  }

  int * idx = get_index(bmu_lis);
  bmu.l = idx[0]; bmu.c = idx[1];
  return bmu;
}


/** \brief Calcule la distance euclidienne de deux vecteurs
 *
 * \param v vecteur v
 * \param w vecteur w
 * \param size taille des vecteurs v et w (partageant la même taille)
 *
 * \return la distance de l'ensemble des deux vecteurs.
 */
double euclidean_dist(double * v, double * w, int size) {
  double sum = 0;
  int i;
  for(i = 0; i < size; i++)
    sum += pow(v[i] - w[i], 2.0);
  return sqrt(sum);
}

/** \brief Fonction retournant un nombre aléatoire entre deux
 * intervalles.
 *
 * \param min intervalle minimal
 * \param max intervalle maximal
 * 
 * \return Nombre aléatoire entre les intervalles min-max.
 */
double my_rand(double min, double max) {
  return (rand()/(double)RAND_MAX) * (max - min) + min;
}

void print_shuffle(int * sh, int size) {
  int i;
  for(i = 0; i < size; i++)
    printf("%d - ", sh[i]);
  printf("\n");
}

void print_net(network_t * net) {
  int l, c, i;
  printf("alpha:       %.2f\n", net->alpha);
  printf("n.rad:       %d\n", net->nhd_rad);
  for(l = 0; l < LINE; l++)
    for(c = 0; c < COL; c++)
      for(i = 0; i < NB_VAL; i++)
        printf("[%d][%d].w[%d]: %.2f\n", l, c, i, net->map[l][c].w[i]);
}

