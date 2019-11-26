#ifndef __CONFIG_H__
#define __CONFIG_H__

#define _ALPHA 0.7
#define _WAVG_MIN 0.02
#define _WAVG_MAX 0.05
#define _NHD_RAD 4
#define _ITER 2000
#define _MAP_L 10
#define _MAP_C 6
#define _NB_VAL 4

typedef struct config config_t;
struct config {
  double alpha;
  int nhd_rad;
  int iter;
  int map_l, map_c;
  double w_avg_min, w_avg_max;
  int nb_val;
  int data_sz;
};

#endif