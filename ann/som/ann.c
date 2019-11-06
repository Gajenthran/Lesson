#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "ll.h"

#define NB_VAL 4

typedef struct data data_t;
struct data {
  double * v;
  char * etq;
  double norm;
};

typedef struct node node_t;
struct node {
  double * w;
  char * etq;
  double act;
};

typedef struct network network_t;
struct network {
  node_t ** map;
  double alpha;
  int neighbor_rad;
};

int *    init_draw(int n);
char *   read_file(char * f);
data_t * tokenize(char * t, int * size);
void     print_data(data_t * data, int size);
void     normalize(data_t * data, int size);
double   euclidean_distance_vec(double * v, double * w, int size);

void usage(char * msg) {
  fprintf(stderr, "%s\n", msg);
  exit(0);
}

int main(int argc, char *argv[]) {
  if(argc != 2)
    usage("Usage: ./ann <file>.");

  int size;
  data_t * data = NULL;

  char * t = read_file(argv[1]);
  data = tokenize(t, &size);
  normalize(data, size);
  int * draw = init_draw(size);
  print_data(data, size);
  return 0;
}

char * read_file(char * f) {
  char * d = NULL;
  size_t size = 0;

  FILE * fp = fopen(f, "r");
  if(!fp)
    usage("Can't open file.");

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  d = (char *)malloc((size + 1) * sizeof *d);
  fread(d, size, 1, fp);

  d[size] = '\0';
  return d;
}

int * init_draw(int n) {
  srand(time(NULL));
  int * draw = (int *)malloc(n * sizeof *draw);
  assert(draw);
  int * seen = (int *)calloc(0, n * sizeof *seen);
  assert(seen);

  int i, offset, r;
  for(i = 0; i < n; i++) {
    r = rand() % n;
    if(seen[r]) {
      offset = 1;
      while(1) {
        if(r + offset < n && !seen[r + offset]) {
          draw[i] = r + offset;
          seen[r + offset] = 1;
          break;
        } else if(r - offset >= 0 && !seen[r - offset]) {
          draw[i] = r - offset;
          seen[r - offset] = 1;
          break;
        }
        offset++;
      }
    } else {
      draw[i] = r;
      seen[r] = 1;
    }
  }
  return draw;
}

data_t * tokenize(char * t, int * size) {
  int i = 0, j = 0, c = 1;
  while(t[i]) {
    if(t[i] == '\n') c++;
    i++;
  }

  char * tok = strtok(t, "\n");
  char ** lines = (char **)malloc(c * sizeof *lines);
  i = 0;

  while(tok != NULL) {
    lines[i++] = strdup(tok);
    tok = strtok(NULL, "\n");
  }

  data_t * data = (data_t *)malloc(c * sizeof *data);
  assert(data);
  char * end;
  double v;
  for(i = 0; i < c; i++) {
    data[i].v = (double *)malloc(NB_VAL * sizeof *data[i].v);
    assert(data[i].v);
    tok = strtok(lines[i], ",");
    j = 0;
    while(tok != NULL) {
      if(j & NB_VAL) {
        data[i].etq = strdup(tok);
      } else {
        v = strtod(tok, &end);
        if(end == tok) {
          usage("Error while converting data.");
        } else {
          data[i].v[j] = v;
        }
      }
      tok = strtok(NULL, ",");
      j++;
    }
  }
  *size = c;
  return data;
}

void normalize(data_t * data, int size) {
  int i, j;
  double sum;

  for(i = 0; i < size; i++) {
    sum = 0;
    for(j = 0; j < NB_VAL; j++)
      sum += pow(data[i].v[j], 2.0);
    data[i].norm = sqrt(sum);
    for(j = 0; j < NB_VAL; j++)
      data[i].v[j] /= data[i].norm;
  }

  double avg[NB_VAL] = {0};
  sum = 0;
  for(i = 0; i < NB_VAL; i++) {
    for(j = 0; j < size; j++)
      sum += data[j].v[i];
    avg[i] = sum / size;
  }
  //  (rand()/(double)RAND_MAX) * (b-a) + a;
}

double euclidean_distance_vec(double * v, double * w, int size) {
  double sum = 0;
  int i;
  for(i = 0; i < size; i++)
    sum += pow(v[i] - w[i], 2.0);
  return sqrt(sum);
}

/* node_t * initNodes(data_t * data, int index) {
  node_t * node = (node_t *)malloc(sizeof *node);
  assert(node);

  node->act = 0;
  node->etq = strdup(data[index].etq);

  int i, j;
  double avg[NB_VAL] = {0}, itvmax = 0.05, itvmin = -0.02;
  sum = 0;
  for(i = 0; i < NB_VAL; i++) {
    for(j = 0; j < size; j++)
      sum += data[j].v[i];
    avg[i] = sum / size;
  }
  // (rand()/(double)RAND_MAX) * (b-a) + a;
  node->w 
} */

void print_data(data_t * data, int size) {
  int i, j;
  for(i = 0; i < size; i++) {
    for(j = 0; j < NB_VAL; j++) {
      printf("%.1f,", data[i].v[j]);
    }
    printf("%s\n", data[i].etq);
  }
}