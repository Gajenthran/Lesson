/**
 * unsigned int
 * fichier de config
 * stat
 * initialiser avec nblignes avec sqrt
 * nhd
 * debugging print
 * free
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "som.h"
#include "parser.h"
#include "config.h"

void usage(char * msg) {
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

config_t * init_config(void) {
  config_t * cfg = (config_t *)malloc(sizeof(*cfg));
  assert(cfg);
  cfg->alpha = _ALPHA;
  cfg->nhd_rad = _NHD_RAD;
  cfg->iter = _ITER;
  cfg->w_avg_min = _WAVG_MIN;
  cfg->w_avg_min = _WAVG_MAX;
  cfg->map_c = _MAP_C;
  cfg->map_l = _MAP_L;
  cfg->nb_val = _NB_VAL;
  cfg->margin_err = _MARG_ERR;
  return cfg;
}

int main(int argc, char *argv[]) {
  if(argc != 2)
    usage("Usage: ./som <file>.");

  config_t * cfg = NULL;
  data_t * data = NULL;
  network_t * net = NULL;

  cfg = init_config();
  data = read_file(argv[1], cfg);
  normalize(data, cfg);

  int * sh = init_shuffle(cfg->data_sz);
  net = init_network(data, cfg); 

  train(net, sh, data, cfg);
  label(net, data, cfg);
  print_map(net, cfg);

#ifdef DEBUG
  print_shuffle(sh, cfg->data_sz);
  print_net(net, cfg);
  print_data(data, cfg);
#endif

  // free_data(data);
  // free_shuffle(shuffle);
  // free_network(net);
  // free_cfg(cfg);
  return 0;
}