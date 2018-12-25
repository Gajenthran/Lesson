/*!\file list.c
 *
 * \brief Graphe : liste de successeurs
 *
 * \author PANCHALINGAMOORTHY Gajenthran, gajenthran.panchalingamoorthy@etud.univ-paris8.fr
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "match.h"
#include "list.h"

/**
 * \brief Ajouter un successeur à la liste de successeurs.
 *
 * \param l    liste de successeurs
 * \param adrn adresse du noeud successeur
 */
static void ajouter_succ(lis_t ** l, noeud_t * adrn) {
  while(*l)
    l = &((*l)->succ);

  lis_t * s = (lis_t *)malloc(sizeof(*s));
  assert(s);
  s->an = (noeud_t *)malloc(sizeof(*s->an));
  assert(s->an);
  s->an = adrn;
  s->succ = NULL; 

  s->succ = *l;
  (*l) = s;
}

/**
 * \brief Creer le graphe : liste des successeurs en s'aidant
 * de la structure fprog_t qui contient l'ensemble des fonctions.
 *
 * \param fpr les fonctions déclarées du programmes et les appels
 * 
 * \return structure vec_t qui correspond à l'ensemble des vecteurs 
 * contenant les noeuds du graphe
 */
vec_t creer_lis(fprog_t fpr) {
  vec_t v;
  v.n = (noeud_t *)malloc(fpr.nfcts * sizeof(*v.n));
  assert(v.n);
  v.n->l = NULL;
  v.nbn = fpr.nfcts;
  int i, j, succ;

  for(i = 0; i < v.nbn; i++) {
    v.n[i].in = i;
    v.n[i].l = (lis_t *)malloc(sizeof(*v.n[i].l));
    v.n[i].l = NULL;
  }

  for(i = 0; i < fpr.nfcts; i++) { 
    for(j = 0; j < fpr.fcts[i].nbs; j++) {
      if((succ = cmp_chaines(fpr, fpr.fcts[i].succ[j].nom)) >= 0) {
        ajouter_succ(&v.n[i].l, &v.n[succ]);
      }
    }
  }
  return v; 
}

/**
 * \brief Ajouter l'ensemble des indices du noeuds qui forment un cycle
 * dans un vecteur.
 *
 * \param v         vecteur de noeuds (le graphe)
 * \param pile      pile stockant les indices des noeuds au fur et à mesure du parcours du graphe
 * \param n         indice du noeud qui a déja été visité (formant ainsi un cycle)
 * \param cycles    vecteur contenant l'ensemble des cycles du programme
 * \param nbcyc     nombre de cycles du programme
 * \param taillecyc la taille allouée pour le vecteur cycles
 */
static void ajouter_cycle(vec_t v, char * pile, int n, char ** cycles, int *nbcyc, int *taillecyc) {
  int i, j = 0;
  if(*nbcyc == (*taillecyc-1)) {
    *taillecyc *= 2;
    cycles = (char **)realloc(cycles, *taillecyc * sizeof(*cycles));
    assert(cycles);
  }
  cycles[(*nbcyc)] = (char *)malloc(v.nbn * sizeof(*cycles[(*nbcyc)]));
  assert(cycles[(*nbcyc)]);
  memset(cycles[(*nbcyc)], -1, v.nbn);
  for(i = n; i < v.nbn; i++) {
    if(pile[i] == -1)
      break;
    cycles[*nbcyc][j++] = pile[i];
  }
  (*nbcyc)++;
}

/**
 * \brief Trouver les différents noeuds qui forment un cycle en parcourant 
 * le graphe (Lis) en profondeur.
 *
 * \param v         vecteur de noeuds (le graphe)
 * \param in        indice du noeud
 * \param pile      pile stockant les indices des noeuds au fur et à mesure du parcours du graphe
 * \param ip        indice du dernier élément de la pile
 * \param cycles    vecteur contenant l'ensemble des cycles du programme
 * \param nbcyc     nombre de cycles du programme
 * \param taillecyc la taille allouée pour le vecteur cycles
 */
static void dfs_cycle_lis(vec_t v, int in, char * pile, int *ip, char ** cycles, int *nbcyc, int *taillecyc) {
  int n;
  if((n = deja_visite(pile, v.nbn, in)) >= 0) {
    ajouter_cycle(v, pile, n, cycles, nbcyc, taillecyc);
    return;
  }
  pile[(*ip)++] = in;
  lis_t * l = v.n[in].l;
  while(l) {
    dfs_cycle_lis(v, l->an->in, pile, ip, cycles, nbcyc, taillecyc);
    l = l->succ;
  }
  pile[--(*ip)] = -1;
}

/**
 * \brief Calculer les cycles de la liste des successeurs en commençant
 * par le noeud "main".
 *
 * \param v      vecteur de noeuds (le graphe)
 * \param fpr    fonctions déclarées du programmes et les appels
 * \param dejavu composantes connexes
 * \param nbcc   nombre de composantes connexes
 * \param nbcyc  nombre de cycles
 *
 * \return vecteur de l'ensemble des cycles du programme.
 */
char ** calculer_cycle_lis(vec_t v, fprog_t fpr, char * dejavu, int nbcc, int *nbcyc) {
  int i, j, ip = 0, imain = -1, taillecyc = NCYCLES;
  char * pile = (char *)malloc(v.nbn * sizeof(*pile));
  assert(pile);
  memset(pile, -1, v.nbn);

  char ** cycles = (char **)malloc(taillecyc * sizeof(*cycles));
  assert(cycles);

  for(i = 0; i < fpr.nfcts; i++) {
    if(!strcmp(fpr.fcts[i].nom, "main")) {
      imain = i;
      break;
    }
  }

  // Si il existe un main dans les fonctions du programme
  if(imain >= 0) {
    dfs_cycle_lis(v, imain, pile, &ip, cycles, nbcyc, &taillecyc);
    // Parcours des autres composantes connexes
    for(i = 2; i <= nbcc; i++) {
      for(j = 0; j < v.nbn; j++) {
        if(dejavu[j] == i) {
          dfs_cycle_lis(v, j, pile, &ip, cycles, nbcyc, &taillecyc);
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
 * \brief Parcourir les différents noeuds qui sont connexes en parcourant 
 * le graphe en profondeur. A la fin du parcours, tous les noeuds qui ont été 
 * visités forment une composante connexe.
 *
 * \param v      vecteur de noeuds (le graphe)
 * \param in     indice du noeud
 * \param dejavu vecteur contenant les composantes connexes
 * \param nbcc   nombre de composantes connexes
 */
static void chercher_succ_lis(vec_t v, int in, char * dejavu, int nbcc) {
  dejavu[in] = nbcc;
  lis_t * l = v.n[in].l;
  while(l) {
    if(!dejavu[l->an->in])
      chercher_succ_lis(v, l->an->in, dejavu, nbcc);
    l = l->succ;
  }
}

/**
 * \brief Calculer les composantes connexes de la liste des successeurs en commençant
 * par le noeud "main".
 *
 * \param v   vecteur de noeuds (le graphe)
 * \param fpr fonctions déclarées du programme et les appels
 *
 * \return vecteur de l'ensemble des composantes connexes du programme
 */
char * calculer_cc_lis(vec_t v, fprog_t fpr) {
  char * dejavu = (char *)malloc(v.nbn * sizeof(*dejavu));
  assert(dejavu);
  memset(dejavu, 0, v.nbn);
  int i, nbcc = 0, imain = -1;

  for(i = 0; i < fpr.nfcts; i++) {
    if(!strcmp(fpr.fcts[i].nom, "main")) {
      imain = i;
      break;
    }
  }

  // Si il existe un main dans les fonctions du programme
  if(imain >= 0) {
    chercher_succ_lis(v, imain, dejavu, ++nbcc);
   	for(i = 0; i < v.nbn; i++)
      if(!dejavu[i])
        chercher_succ_lis(v, i, dejavu, ++nbcc);
  }
  return dejavu;
}

/**
 * \brief Afficher les composantes connexes de la liste des successeurs.
 *
 * \param v      vecteur de noeuds (le graphe)
 * \param fpr    fonctions déclarées du programme et les appels
 * \param dejavu vecteur contenant les composantes connexes
 * \param nbcc   nombre de composantes connexes
 */
void afficher_cc_lis(vec_t v, fprog_t fpr, char * dejavu, int nbcc) {
  int i, j;
  printf("\n\n-- COMPOSANTES CONNEXES (LIS): %d -- \n", nbcc);
  for(i = 1; i <= nbcc; i++) {
    printf("%d)   ", i);
    for(j = 0; j < v.nbn; j++) {
      if(dejavu[j] == i)
        printf("%s - ", fpr.fcts[v.n[j].in].nom);
    }
    printf("\n\n");
  }
}

/**
 * \brief Afficher les cycles de la liste de successeurs.
 *
 * \param v      vecteur de noeuds (le graphe)
 * \param fpr    fonctions déclarées du programme et les appels
 * \param cycles vecteur contenant l'ensemble des cycles du programme
 * \param nbcyc  nombre de cycles du programme
 */
void afficher_cycle_lis(vec_t v, fprog_t fpr, char ** cycles, int nbcyc) {
  int i, j;
  printf("\n\n-- AFFICHAGE DES CYCLES (LIS): %d -- \n", nbcyc);
  for(i = 0; i < nbcyc; i++) {
    printf("%d)   ", i+1); 
    for(j = 0; j < v.nbn; j++) {
      if(cycles[i][j] != -1) {
        printf("%s - ", fpr.fcts[(int)cycles[i][j]].nom);
      }
    }
    printf("\n\n");
  }
}

/**
 * \brief Afficher les noeuds du graphe (Lis) où chaque valeur
 * d'un noeud correspond à l'indice des fonctions de fpr.
 *
 * \param v   vecteur de noeuds (le graphe)
 * \param fpr fonctions déclarées du programme et les appels
 */
void afficher_lis(vec_t v, fprog_t fpr) {
  int i;
  lis_t * l;
  printf("\n\n-- AFFICHAGE DE LA LISTE DE SUCC -- \n");
  for(i = 0; i < v.nbn; i++) {
    printf("- %s\n", fpr.fcts[v.n[i].in].nom);
    l = v.n[i].l;
    while(l) {
      printf(" -> %s\n", fpr.fcts[l->an->in].nom);
      l = l->succ;
    }
  }
}

/**
 * \brief Libérer la structure représentant la liste de successeurs.
 *
 * \param v vecteur de noeuds (le graphe)
 */
void liberer_lis(vec_t v) {
  int i;
  lis_t * tmp;
  for(i = 0; i < v.nbn; i++) {
    while(v.n[i].l) {
      tmp = v.n[i].l;
      v.n[i].l = v.n[i].l->succ;
      free(tmp);
    }
  }
}
