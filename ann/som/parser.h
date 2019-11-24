#ifndef _PARSER_H_
#define _PARSER_H_

#define NB_VAL 4

/** \brief Structure représentant les données */
typedef struct data data_t;
struct data {
  double * v;   // vecteur de données 
  char * label; // étiquette
  double norm;  // norme
};

data_t * read_file(char *, int *);
data_t * tokenize(char *, int *);
void     normalize(data_t *, int);
void     print_data(data_t *, int);

#endif