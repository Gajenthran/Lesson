/*!\file match.h
 *
 * \brief Vérification des différents pattern (différentes expressions) dans le programme.
 * L'ensemble des structures représentant les fonctions et les appels de fonctions ainsi que le
 * code ASCII des caractères.
 *
 * \author PANCHALINGAMOORTHY Gajenthran, gajenthran.panchalingamoorthy@etud.univ-paris8.fr
 */
#ifndef MATCH_H_
#define MATCH_H_

#define NCYCLES 32

typedef struct fprog fprog_t;
typedef struct fonction fonction_t;

/** \brief Structure représentant une fonction */
struct fonction {
  char * nom;               /*!< Nom de la fonction déclarée */
  struct fonction * succ;   /*!< Les fonctions appelées */
  int nbs;                  /*!< Le nombre de fonctions appelées */
};

/** \brief Structure représentant les fonctions déclarées du programme */
struct fprog {
  struct fonction * fcts;   /*!< L'ensemble des fonctions déclarées */
  int nfcts;                /*!< Le nombre de fonctions déclarées */
};

/** \brief Valeur des caractères */
enum keycode { 
  K_A = 65, K_z = 122, K_star = 42,     // K_A = 'A', K_z = 'z', K_star = '*'
  K_0 = 48, K_9 = 57,                   // K_0 = '0', K_9 = '9'
  K_dash = 95, K_nbsp = 10, K_sp = 32,  // K_dash = '_', K_nbsp = '\n', K_sp = ' '
  K_opar = 40, K_cpar = 41,             // K_opar = '(', K_cpar = ')'
  K_obra = 123, K_cbra = 125,           // K_obra = '{', K_cbra = '}'
  K_slash = 47, K_bslash = 92,          // K_slash = '/', K_bslash = '\'
  K_dquote = 34, K_quote = 39           // K_quote = '"', K_quote = '''
};


int   match_acc(char *s, int *is, int etat, fprog_t * fpr, int nfcts);
int   match_par(char *s, int *is);
int   match_symbole(char *s, int is, char c);
int   match_alphanum(char *s, int *is);
void  match_comment(char *s, int *is);
void  match_blanc(char *s, int *is);
void  match_chaine(char *s, int *is);
void  match_ignore(char *s, int *is);
int   match_fonction(char *s, int *is, int e, fprog_t * fpr, int nfcts);
int   match_args(char *s, int *is, int d, int e, fprog_t * fpr, int nfcts);
int   match_bloc(char *s, int *is, int d, int f, int e, fprog_t * fpr, int nfcts);
int   match_callf(char *s, int *is, fprog_t * fpr, int nfcts);
int   match_fichier(char *s, int *is, char ** fichier, int *nbf);
int   cmp_nchaines(fprog_t fpr, char * fct, int n);
int   cmp_chaines(fprog_t fpr, char * fct);
int   cmp_callf(fprog_t fpr, char * fct, int size, int n);
int   deja_visite(char * pile, int n, int in);

#endif