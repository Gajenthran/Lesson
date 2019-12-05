#ifndef __CONFIG_H__
#define __CONFIG_H__

#define NCFG 13
#define CONFIG_FILE "som.cfg"

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
  int nb_label;
  float ph_1, ph_2;
};

#endif