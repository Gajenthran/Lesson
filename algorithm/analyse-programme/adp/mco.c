/*!\file mco.c
 *
 * \brief Graphe : matrice compacte
 *
 * \author PANCHALINGAMOORTHY Gajenthran, gajenthran.panchalingamoorthy@etud.univ-paris8.fr
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "mco.h"
#include "match.h"

/**
 * \brief Créer le graphe : matrice compacte en s'aidant
 * de la structure fprog_t qui contient l'ensemble des fonctions.
 *
 * \param fpr les fonctions déclarées du programme et les appels
 * 
 * \return structure mco_t qui correspond à la matrice compacte créée.
 */
mco_t creer_mco(fprog_t fpr) {
  int i, j, succ, nel = 0;
  mco_t m;

  m.nba = fpr.nfcts * fpr.nfcts;
  m.nbn = fpr.nfcts;
  m.c = (cell_t *)malloc(m.nba * sizeof(*m.c));
  assert(m.c);

  for(i = 0; i < fpr.nfcts; i++) {
    for(j = 0; j < fpr.fcts[i].nbs; j++) {
      if((succ = cmp_chaines(fpr, fpr.fcts[i].succ[j].nom)) >= 0) {
        m.c[nel].n = i;
        m.c[nel].s = succ;
        nel++;
      }
    }
  }

  m.c = (cell_t *)realloc(m.c, nel * sizeof(*m.c));
  assert(m.c);
  m.nba = nel;
  return m;
}

/**
 * \brief Vérifier si le noeud passé en paramètre possède 
 * également un ou des successeurs.
 *
 * \param m     matrice compacte (le graphe)
 * \param noeud noeud
 * 
 * \return -1 si il n'a pas de successeur sinon l'indice du noeud dans
 * la matrice
 */
static int trouver_noeud_mco(mco_t m, int noeud) {
  int i;
  for(i = 0; i < m.nba; i++)
    if(noeud == m.c[i].n)
      return i;

  return -1;
}

/**
 * \brief Parcourir les différentes noeuds qui sont connexes en parcourant 
 * le graphe en profondeur. A la fin du parcours, tous les noeuds qui ont été 
 * visités forme une composante connexe.
 *
 * \param m      matrice compacte (le graphe)
 * \param in     indice du noeud
 * \param dejavu vecteur contenant les composantes connexes
 * \param nbcc   nombre de composantes connexes
 */ 
static void chercher_succ_mco(mco_t m, int in, char * dejavu, int nbcc) {
  dejavu[m.c[in].n] = nbcc;
  do {
    int succ = trouver_noeud_mco(m, m.c[in].s);
    if(!dejavu[m.c[in].s]) {
      if(succ >= 0)
        chercher_succ_mco(m, succ, dejavu, nbcc);
      else
        dejavu[m.c[in].s] = nbcc;        
    }
    in++;
  } while(in < m.nba && m.c[in].n == m.c[in-1].n); 
}

/**
 * \brief Calculer les composantes connexes de la liste des successeurs en commençant
 * par le noeud "main".
 *
 * \param m   matrice compacte (le graphe)
 * \param fpr fonctions déclarées du programme et les appels
 *
 * \return vecteur de l'ensemble des composantes connexes du programme
 */
char * calculer_cc_mco(mco_t m, fprog_t fpr) {
  char * dejavu = (char *)malloc(m.nba * sizeof(*dejavu));
  assert(dejavu);
  memset(dejavu, 0, m.nbn);
  int i, succ, nbcc = 0, imain = -1;

  for(i = 0; i < m.nba; i++) {
    if(!strcmp(fpr.fcts[m.c[i].n].nom, "main")) {
      imain = i;
      break;
    }
  }

  // Si il existe un main dans les fonctions du programme
  if(imain >= 0) {
    chercher_succ_mco(m, imain, dejavu, ++nbcc);
    for(i = 0; i < m.nbn; i++) {
      if(!dejavu[i]) {
        if((succ = trouver_noeud_mco(m, i)) >= 0)
          chercher_succ_mco(m, succ, dejavu, ++nbcc); 
        else
          dejavu[i] = ++nbcc;
      }
    }
  }
  return dejavu;
}


/**
 * \brief Compter le nombre de composantes connexes en retrouvant la valeur
 * maximale dans le tableau dejavu.
 *
 * \param m   matrice compacte (le graphe)
 * \param fpr fonctions déclarées du programme et les appels
 *
 * \return le nombre de composantes connexes du graphe
 */
int compter_cc(char * dejavu, fprog_t fpr) {
  int i, max = 0;
  for(i = 0; i < fpr.nfcts; i++) {
    if(max < dejavu[i])
      max = dejavu[i];
  }
  return max;
}

/**
 * \brief Ajouter l'ensemble des indices d'un noeud qui forment un cycle
 * dans un vecteur.
 *
 * \param m         matrice compacte (le graphe)
 * \param pile      pile stockant les indices des noeuds au fur et à mesure du parcours du graphe
 * \param n         indice du noeud qui a déja été visité (formant ainsi un cycle)
 * \param cycles    vecteur contenant l'ensemble des cycles du programme
 * \param nbcyc     nombre de cycles du programme
 * \param taillecyc la taille allouée pour le vecteur cycles
 */
static void ajouter_cycle_mco(mco_t m, char * pile, int n, char ** cycles, int *nbcyc, int *taillecyc) {
  int i, j = 0;
  if(*nbcyc == (*taillecyc-1)) {
    *taillecyc *= 2;
    cycles = (char **)realloc(cycles, *taillecyc * sizeof(*cycles));
    assert(cycles);
  }

  cycles[(*nbcyc)] = (char *)malloc(m.nbn * sizeof(*cycles[(*nbcyc)]));
  assert(cycles[(*nbcyc)]);
  memset(cycles[(*nbcyc)], -1, m.nbn);

  for(i = n; i < m.nbn; i++) {
    if(pile[i] == -1)
      break;
    cycles[*nbcyc][j++] = pile[i];
  }
  (*nbcyc)++;
}

/**
 * \brief Trouver les différentes noeuds qui forment un cycles en parcourant 
 * le graphe (Mco) en profondeur.
 *
 * \param m         matrice compacte (le graphe)
 * \param in        indice du noeud
 * \param pile      pile stockant les indices des noeuds au fur et à mesure du parcours du graphe
 * \param ip        indice du dernier élément de la pile
 * \param cycles    vecteur contenant l'ensemble des cycles du programme
 * \param nbcyc     nombre de cycles du programme
 * \param taillecyc la taille allouée pour le vecteur cycles
 */
static void dfs_cycle_mco(mco_t m, int in, char * pile, int *ip, char ** cycles, int *nbcyc, int *taillecyc) {
  int n;
  if((n = deja_visite(pile, m.nbn, m.c[in].n)) >= 0) {
    ajouter_cycle_mco(m, pile, n, cycles, nbcyc, taillecyc);
    return;
  }
  pile[(*ip)++] = m.c[in].n;
  do {
    int succ = trouver_noeud_mco(m, m.c[in].s);
    if(succ >= 0)
      dfs_cycle_mco(m, succ, pile, ip, cycles, nbcyc, taillecyc);    
    in++;
  } while(in < m.nba && m.c[in].n == m.c[in-1].n);
  pile[--(*ip)] = -1;
}

/**
 * \brief Calculer les cycles de la matrice compacte en commençant
 * par le noeud "main".
 *
 * \param m      matrice compacte (le graphe)
 * \param fpr    fonctions déclarées du programme et les appels
 * \param dejavu composantes connexes
 * \param nbcc   nombre de composantes connexes
 * \param nbcyc  nombre de cycles
 *
 * \return vecteur de l'ensemble des cycles du programme.
 */
char ** calculer_cycle_mco(mco_t m, fprog_t fpr, char * dejavu, int nbcc, int *nbcyc) {
  int i, j, ip = 0, imain = -1, taillecyc = NCYCLES;
  char * pile = (char *)malloc(m.nbn * sizeof(*pile));
  assert(pile);
  memset(pile, -1, m.nbn);

  char ** cycles = (char **)malloc(taillecyc * sizeof(*cycles));
  assert(cycles);

  for(i = 0; i < m.nba; i++) {
    if(!strcmp(fpr.fcts[m.c[i].n].nom, "main")) {
      imain = i;
      break;
    }
  }

  // Si il existe un main dans les fonctions du programme
  if(imain >= 0) {
    dfs_cycle_mco(m, imain, pile, &ip, cycles, nbcyc, &taillecyc);
    // Parcours des autres composantes connexes
    for(i = 2; i <= nbcc; i++) {
      for(j = 0; j < m.nba; j++) {
        if(dejavu[m.c[j].n] == i) {
          dfs_cycle_mco(m, j, pile, &ip, cycles, nbcyc, &taillecyc);
          break;
        }
      }
    }
  }

  cycles = (char **)realloc(cycles, *nbcyc * sizeof(*cycles));
  assert(cycles);
  return cycles;
}

/**
 * \brief Afficher les composantes connexes de la matrice compacte.
 *
 * \param m      matrice compacte (le graphe)
 * \param fpr    fonctions déclarées du programme et les appels
 * \param dejavu composantes connexes
 * \param nbcc   nombre de composantes connexes
 */
void afficher_cc_mco(mco_t m, fprog_t fpr, char * dejavu, int nbcc) {
  int i, j;
  printf("\n\n-- COMPOSANTES CONNEXES (MCO): %d -- \n", nbcc);
  for(i = 1; i <= nbcc; i++) {
    printf("%d)   ", i);
    for(j = 0; j < m.nbn; j++) {
      if(dejavu[j] == i)
        printf("%s - ", fpr.fcts[j].nom);
    }
    printf("\n\n");
  }
}

/**
 * \brief Afficher les cycles de la matrice compacte.
 *
 * \param m      matrice compacte (le graphe)
 * \param fpr    fonctions déclarées du programme et les appels
 * \param cycles vecteur contenant l'ensemble des cycles du programme
 * \param nbcyc  nombre de cycles du programme
 */
void afficher_cycle_mco(mco_t m, fprog_t fpr, char ** cycles, int nbcyc) {
  int i, j;
  printf("\n\n-- AFFICHAGE DES CYCLES (MCO): %d --\n", nbcyc);
  for(i = 0; i < nbcyc; i++) {
    printf("%d)   ", i+1); 
    for(j = 0; j < m.nbn; j++) {
      if(cycles[i][j] != -1) {
        printf("%s - ", fpr.fcts[(int)cycles[i][j]].nom);
      }
    }
    printf("\n\n");
  }
}

/**
 * \brief Afficher les noeuds du graphe (Mco) où chaque valeur
 * d'un noeud correspond à l'indice des fonctions de fpr.
 *
 * \param m   matrice compacte (le graphe)
 * \param fpr fonctions déclarées du programme et les appels
 */
void afficher_mco(mco_t m, fprog_t fpr) {
  int i = 0;
  printf("\n\n-- AFFICHAGE DE LA MATRICE COMPACTE -- \n");
  while (i < m.nba) {
    printf("%s\n", fpr.fcts[m.c[i].n].nom);
    do {
      printf(" -> %s\n", fpr.fcts[m.c[i].s].nom);
      i++;
    } while(i < m.nba && m.c[i].n == m.c[i-1].n);
  }
}

/**
 * \brief Libérer la structure représentant la matrice compacte.
 *
 * \param m matrice compacte (le graphe)
 */
void liberer_mco(mco_t m) {
  if(m.c) {
    free(m.c);
    m.c = NULL;
    m.nbn = 0;
  }
}
