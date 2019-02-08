/*!\file mco.h
 *
 * \brief Graphe : matrice compacte. 
 * L'ensemble des structures représentant le graphe Mco
 *
 * \author PANCHALINGAMOORTHY Gajenthran, gajenthran.panchalingamoorthy@etud.univ-paris8.fr
 */
#ifndef MCO_H_
#define MCO_H_
#include "match.h"

typedef struct mco mco_t;
typedef struct cell cell_t;

/** \brief Structure représentant les cellules de la matrice compacte */
struct cell {
  int n;              /*!< Indice du noeud */
  int s;              /*!< Indice du successeur */
};

/** \brief Structure représentant la matrice compacte */
struct mco {
  struct cell * c;    /*!< L'ensemble des cellules de la mco de taille nba */
  int nba;            /*!< Nombre d'arêtes */
  int nbn;            /*!< Nombre de noeuds */
};


mco_t     creer_mco(fprog_t fpr);
char *    calculer_cc_mco(mco_t m, fprog_t fpr);
int       compter_cc(char * dejavu, fprog_t fpr);
char **  calculer_cycle_mco(mco_t m, fprog_t fpr, char * dejavu, int nbcc, int *nbcyc);
void 	  afficher_cycle_mco(mco_t m, fprog_t fpr, char ** cycles, int nbcycles);
void      afficher_cc_mco(mco_t m, fprog_t fpr, char * dejavu, int nbcc);
void      afficher_mco(mco_t m, fprog_t fpr);
void      liberer_mco(mco_t m);

#endif