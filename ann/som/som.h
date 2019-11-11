#ifndef _SOM_H_
#define _SOM_H_
#include "parser.h"

#define MAX(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define MIN(a,b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define NB_ITER(phase) \
  ({ __typeof__ (phase) _phase = (phase); \
    _phase > 1 ? 1500 : 500; })

#define LINE 10
#define COL 6

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

int *        init_shuffle(int);
void         shuffle(int *, int);
network_t *  init_network(data_t *, int);
void         train(int, network_t *, int *, data_t *, int);
void         label(network_t * net, data_t * data, int size);
bmu_t        find_bmu(network_t * net, double * v);
void         apply_nhd(network_t * net, double * v, bmu_t bmu);
double       euclidean_dist(double *, double *, int);
double       my_rand(double min, double max);
void         print_net(network_t *);
void         print_shuffle(int *, int);

#endif