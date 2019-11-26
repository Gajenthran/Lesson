#ifndef _SOM_H_
#define _SOM_H_

#include "parser.h"
#include "ll.h"
#include "config.h"

/** \brief Structure représentant les neurones */
typedef struct node node_t;
struct node {
  double * w;   // vecteur de données
  char * label; // étiquette
  double act;   // état d'activation
};

/** \brief Structure représentant le réseau de neurones */
typedef struct network network_t;
struct network {
  node_t ** map; // réseau, map bidimensionnelle
  double alpha;  // coefficient d'activation
  int nhd_rad;   // rayon de voisinage
};

/** \brief Structure représentant le best match unit */
typedef struct bmu bmu_t;
struct bmu {
  double act; // état d'activation
  int l, c;   // ligne, colonne
};

int *       init_shuffle(int);
void        shuffle(int *, int);
network_t * init_network(data_t *, config_t *);
void        train(network_t *, int *, data_t *, config_t *);
void        label(network_t * net, data_t * data, config_t *);
bmu_t       find_bmu(network_t *, double *, config_t *);
void        apply_nhd(network_t *, double *, bmu_t, config_t *);
double      euclidean_dist(double *, double *, int);
double      my_rand(double min, double max);
void        print_net(network_t *, config_t *);
void        print_shuffle(int *, int);

#endif