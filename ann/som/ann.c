#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "ll.h"

#define MAX 1024
#define NB_VAL 4
#define LINE 10
#define COL 6

/** \brief Structure représentant les données */
typedef struct data data_t;
struct data {
  double * v;   // vecteur de données 
  char * etq;   // étiquette
  double norm;  // norme
};

/** \brief Structure représentant les neurones */
typedef struct node node_t;
struct node {
  double * w;  // vecteur de données
  char * etq;  // étiquette
  double act;  // état d'activation
};

/** \brief Structure représentant le reseau de neurones */
typedef struct network network_t;
struct network {
  node_t ** map;     // réseau, map bidimensionnelle
  double alpha;      // coefficient d'activation
  int neighbor_rad;  // rayon de voisinage
};

// void read_(char * in);
char *       read_file(char * f);
data_t *     tokenize(char * t, int * size);
void         normalize(data_t * data, int size);
network_t *  init_network(data_t * data, int size);
int *        init_draw(int n);
double       euclidean_distance_vec(double * v, double * w, int size);
void         print_data(data_t * data, int size);
void         print_net(network_t * net);

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
  read_file(argv[1]);

  data = tokenize(t, &size);
  normalize(data, size);

  // int * draw = init_draw(size);
  net = init_network(data, size);
  
  print_net(net);
  // print_data(data, size);
  return 0;
}

/** \brief Lire le fichiers de données et retourner son contenu
 * sous forme de chaîne de caractère
 *
 * \param f fichier de données
 * 
 * \return le contenu des données sous forme de chaînes
 */
char * read_file(char * in) {
  char * d = NULL;
  size_t size = 0;

  FILE * fp = fopen(in, "r");
  if(!fp) {
    fprintf(stderr, "Can't open file %s\n", in);
    exit(0);
  }

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  d = (char *)malloc((size + 1) * sizeof *d);
  fread(d, size, 1, fp);

  d[size] = '\0';
  return d;
}

/** \brief Initialise le vecteur représentant le mélange de l'ordre
 * de passage des données lors de la phase d'apprentissage
 *
 * \param n nombre de données
 * 
 * \return vecteur représentant l'ordre de passage des données.
 */
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

/** \brief Tokenize le contenu du fichiers en le coupant
 * en lignes et en virgule pour récupérer les données 
 * utiles (valeurs + label)
 *
 * \param t fichier de données sous forme de chaîne de caractère
 * \param size taille du fichier de données
 * 
 * \return la structure de forme data_t qui représente les données 
 * formalisées
 */
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

/** \brief Normaliser les données
 *
 * \param data ensemble de données
 * \param size nombres de données
 * 
 */
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

  // double avg[NB_VAL] = {0};
  // sum = 0;
  // for(i = 0; i < NB_VAL; i++) {
  //   for(j = 0; j < size; j++)
  //     sum += data[j].v[i];
  //   avg[i] = sum / size;
  // }
  // (rand()/(double)RAND_MAX) * (b-a) + a;
}

/** \brief Calcule la distance euclidienne de deux vecteurs
 *
 * \param v vecteur v
 * \param w vecteur w
 * \param size taille des vecteurs v et w (partageant la même taille)
 * 
 * \return la distance de l'ensemble des deux vecteurs.
 */
double euclidean_distance_vec(double * v, double * w, int size) {
  double sum = 0;
  int i;
  for(i = 0; i < size; i++)
    sum += pow(v[i] - w[i], 2.0);
  return sqrt(sum);
}

network_t * init_network(data_t * data, int size) {
  int i, j, l, c;
  double avg, itvmax = 0.05, itvmin = 0.02, sum = 0;

  network_t * net = (network_t *)malloc(sizeof *net);
  assert(net);
  net->map = (node_t **)malloc(LINE * sizeof(*net->map));
  assert(net->map);

  for(l = 0; l < LINE; l++) {
    net->map[l] = (node_t *)malloc(COL * sizeof(*net->map[l]));
    assert(net->map[l]);
    for(c = 0; c < COL; c++) {
      for(i = 0; i < NB_VAL; i++) {
        net->map[l][c].w = (double *)malloc(NB_VAL * sizeof(net->map[l][c].w));
        assert(net->map[l][c].w);
      }
    }
  }

  for(i = 0; i < NB_VAL; i++) {
    for(j = 0; j < size; j++) 
      sum += data[j].v[i];
    avg = sum / size;

    for(l = 0; l < LINE; l++)
      for(c = 0; c < COL; c++) {
        net->map[l][c].w[i] = (rand()/(double)RAND_MAX) * 
          ((avg + itvmax) - (avg - itvmin)) + (avg - itvmin);
      }
  }

  net->alpha = 0.7;
  net->neighbor_rad = 3;
  return net;
}

void print_data(data_t * data, int size) {
  int i, j;
  for(i = 0; i < size; i++)
    for(j = 0; j < NB_VAL; j++)
      printf("%.1f,", data[i].v[j]);
    printf("%s\n", data[i].etq);
}

void print_net(network_t * net) {
  int l, c, i;
  printf("alpha:       %.2f\n", net->alpha);
  printf("n.rad:       %d\n", net->neighbor_rad);
  for(l = 0; l < LINE; l++)
    for(c = 0; c < COL; c++)
      for(i = 0; i < NB_VAL; i++)
        printf("[%d][%d].w[%d]: %.2f\n", l, c, i, net->map[l][c].w[i]);
}

/* void read_(char * in) {
  FILE * fp = fopen(in, "r");
  if(!fp) {
    fprintf(stderr, "Can't open file %s\n", in);
    exit(0);
  }

  unsigned int line = 0, size = MAX;
  char * buf = (char *)malloc(size * sizeof(*buf)), * tok, * last;
  assert(buf);
  data_t * data = (data_t *)malloc(size * sizeof(*data));
  assert(data);
  while(!feof(fp)) {
    fgets(buf, MAX, fp);
    if(ferror(fp)) {
      fprintf( stderr, "Error while reading file %s\n", in);
      exit(0);
    }

    tok = last = strtok(buf, ",");
    while(tok != NULL) {
      tok = strtok(NULL, ",");
      printf("%s\n", last);
      last = tok;
    }

    if(line == size-1) {
      data = (data_t *)realloc(data, (size *= 2) * sizeof(*data));
      assert(data);
    }

    line++;
  }
} */