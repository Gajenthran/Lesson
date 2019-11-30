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
 * \param cfg      données de configuration
 * 
 * \return la structure de forme data_t qui représente 
 * les données formalisées
 */
data_t * read_file(char * filename, config_t * cfg) {
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
    data[line].v = (double *)malloc(cfg->nb_val * sizeof(*data[line].v));
    assert(data[line].v);

    j = 0;
    while(tok != NULL) {
      if(j < cfg->nb_val)
        data[line].v[j++] = strtod(tok, &end);
      label = tok;
      tok = strtok(NULL, ",");
    }

    label = strtok(label, "\n");
    data[line++].label = strdup(label);
  }

  cfg->data_sz = line;
  return data;
}

/** \brief Normalise les données
 *
 * \param data ensemble de données
 * \param cfg  données de configuration
 */
void normalize(data_t * data, config_t * cfg) {
  int i, j;
  double sum;

  for(i = 0; i < cfg->data_sz; i++) {
    sum = 0;
    for(j = 0; j < cfg->nb_val; j++)
      sum += pow(data[i].v[j], 2.0);
    data[i].norm = sqrt(sum);
    for(j = 0; j < cfg->nb_val; j++)
      data[i].v[j] /= data[i].norm;
  }
}

void print_data(data_t * data, config_t * cfg) {
  int i, j;
  for(i = 0; i < cfg->data_sz; i++) {
    for(j = 0; j < cfg->nb_val; j++) {
      printf("%.1f,", data[i].v[j]);
    }
    printf("%s\n", data[i].label);
  }
}