#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parser.h"


/** \brief Lire le fichiers de données, tokenizer son 
 * contenu où chaque valeur est séparée par une virgule
 * placer les éléments dans la struct data_t
 *
 * \param filename nom du fichier
 * 
 * \return la structure de forme data_t qui représente 
 * les données formalisées
 */
data_t * read_file(char * filename, int * size) {
  const int MAX = 1024;
  FILE * fp = fopen(filename, "r");
  if(!fp) {
    fprintf(stderr, "Can't open file %s\n", filename);
    exit(1);
  }

  int line = 0, j = 0;
  char * buf = (char *)malloc(MAX * sizeof(*buf)), * tok, * end;
  assert(buf);
  data_t * data = (data_t *)malloc(MAX * sizeof(*data));
  assert(data);

  while(!feof(fp)) {
    fgets(buf, MAX, fp);
    if(ferror(fp)) {
      fprintf( stderr, "Error while reading file %s\n", filename);
      exit(1);
    }

    // tokenizer la ligne récupérée par fgets
    char * label;
    tok = strtok(buf, ",");
    data[line].v = (double *)malloc(NB_VAL * sizeof(*data[line].v));
    assert(data[line].v);

    j = 0;
    while(tok != NULL) {
      if(j < NB_VAL)
        data[line].v[j++] = strtod(tok, &end);
      label = tok;
      tok = strtok(NULL, ",");
    }

    label = strtok(label, "\n");
    data[line++].label = strdup(label);
  }

  *size = line;
  return data;
}

/** \brief Normalise les données
 *
 * \param data ensemble de données
 * \param size nombres de données
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
  for(i = 0; i < size; i++) {
    for(j = 0; j < NB_VAL; j++) {
      printf("%.1f,", data[i].v[j]);
    }
    printf("%s\n", data[i].label);
  }
}