#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parser.h"


/** \brief Lire le fichiers de données et retourner son contenu
 * sous forme de chaîne de caractère
 *
 * \param filename nom du fichier
 * 
 * \return le contenu des données sous forme de chaînes
 */
char * read_file(char * filename) {
  char * d = NULL;
  size_t size = 0;

  FILE * fp = fopen(filename, "r");
  if(!fp) {
    fprintf(stderr, "Can't open file %s\n", filename);
    exit(0);
  }

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  d = (char *)malloc((size + 1) * sizeof *d);
  fread(d, size, 1, fp);

  d[size] = '\0';
  fclose(fp);
  return d;
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
        data[i].label = strdup(tok);
      } else {
        v = strtod(tok, &end);
        if(end == tok) {
          fprintf(stderr, "Error while converting data.\n");
          exit(0);
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
}

void print_data(data_t * data, int size) {
  int i, j;
  for(i = 0; i < size; i++)
    for(j = 0; j < NB_VAL; j++)
      printf("%.1f,", data[i].v[j]);
    printf("%s\n", data[i].label);
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