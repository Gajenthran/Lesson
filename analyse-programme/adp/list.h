/*!\file list.h
 *
 * \brief Graphe : liste de successeurs. 
 * L'ensemble des structures représentant le graphe Lis
 *
 * \author PANCHALINGAMOORTHY Gajenthran, gajenthran.panchalingamoorthy@etud.univ-paris8.fr
 */
#ifndef LIST_H_
#define LIST_H_

typedef struct noeud noeud_t;
typedef struct vec vec_t;
typedef struct lis lis_t;

/** \brief Structure représentant la liste de successeurs */
struct lis {
  struct noeud * an;   /*!< Adresse du noeud */
  struct lis * succ;   /*!< Pointeur vers le successeur suivant */
};

/** \brief Structure représentant un noeud */
struct noeud {
  int in;              /*!< Indice du noeud */
  struct lis * l;      /*!< Liste des successeurs */
};

/** \brief Structure représentant un vecteur de tous les noeuds */
struct vec {
  struct noeud * n;    /*!< Ensemble des noeuds */
  int nbn;             /*!< Nombre de noeuds */
};


vec_t     creer_lis(fprog_t fpr);
char *    calculer_cc_lis(vec_t v, fprog_t fpr);
char **   calculer_cycle_lis(vec_t v, fprog_t fpr, char * dejavu, int nbcc, int *nbcyc);
void      afficher_lis(vec_t v, fprog_t fpr);
void      afficher_cc_lis(vec_t v, fprog_t fpr, char * dejavu, int nbcc);
void      afficher_cycle_lis(vec_t v, fprog_t fpr, char ** cycles, int nbcycles);
void      liberer_lis(vec_t v);

#endif