/**
 * NB_VAL à changer par variable
 * size argument
 * alpha, neigbor constantes
 * liste chaînée + bmu
 * result
 * dimension des vecteurs en argv
 * shuffle dans l'ordre premier fois ou chq iter
 * unsigned int
 * fichier deconfig
 * stat
 * initialiser avec nblignes avec sqrt
 * nhd
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "ll.h"
#include "som.h"
#include "parser.h"

void usage(char * msg) {
  fprintf(stderr, "%s\n", msg);
  exit(0);
}

int main(int argc, char *argv[]) {
  if(argc != 2)
    usage("Usage: ./ann <file>.");

  int size;
  data_t * data = NULL;
  network_t * net = NULL;

  char * t = read_file(argv[1]);
  data = tokenize(t, &size);
  normalize(data, size);

  int * sh = init_shuffle(size);
  net = init_network(data, size);

  train(NB_ITER(1), net, sh, data, size);
  train(NB_ITER(2), net, sh, data, size);
  label(net, data, size);

  // print_shuffle(sh, size);
  // print_net(net);
  // print_data(data, size);
  return 0;
}