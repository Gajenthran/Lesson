#ifndef __CONFIG_H__
#define __CONFIG_H__

#define _ALPHA 0.7
#define _WAVG_MIN 0.02
#define _WAVG_MAX 0.05
#define _NHD_RAD 3
#define _ITER 2000
#define _MAP_L 10
#define _MAP_C 6
#define _NB_VAL 4
#define _MARG_ERR 0.015

/* Structure représentant la configuration du programme */
typedef struct config config_t;
struct config {
  double alpha;                // coefficient d'apprentissage
  int nhd_rad;                 // rayon de voisinage
  int iter;                    // nombre d'itérations
  int map_l, map_c;            // dimension de la map
  double w_avg_min, w_avg_max; // interval pour w
  int nb_val;                  // nombre de valeurs dans les données
  int data_sz;                 // nombre de données
  float margin_err;            // marge d'erreur
};

#endif