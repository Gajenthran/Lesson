#ifndef _PARSER_H_
#define _PARSER_H_

#include "config.h"

/** \brief Structure représentant les données */
typedef struct data data_t;
struct data {
  double * v;   // vecteur de données 
  char * label; // étiquette
  double norm;  // norme
};

data_t * read_file(char *, config_t *);
data_t * tokenize(char *, config_t *);
void     normalize(data_t *, config_t *);
void     print_data(data_t *, config_t *);

#endif