/*!\file adp.c
 *
 * \brief Fichier principale qui permet d'analyser un programme.
 *
 * Le programme analyse un programme et va permettre de former
 * plusieurs graphes (Lis, Mco) où les noeuds seront les fonctions 
 * déclarées et les successeurs correspondront aux appels de fonctions.
 *
 * \author PANCHALINGAMOORTHY Gajenthran, gajenthran.panchalingamoorthy@etud.univ-paris8.fr
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "mco.h"
#include "list.h"
#include "match.h"

#define MAX 1024
#define NF 4

char *   creer_prog(int *n, int argc, char *argv[]);
char *   lire_makefile(char * fichier, int *nmake);
char *   lire_fichiers(char ** fichiers, int nbf, int *nbcar);
char **  analyser_makefile(char * makefile, int nmake, int *nbf);
void     analyser_prog(char * prog, int n);
fprog_t  analyser_fcts(char *prog, int n);
void     analyser_callf(char *prog, int n, fprog_t * fpr);
void     afficher_fcts(fprog_t fpr);

/**
 * \brief Vérifier si la chaîne est présente dans la liste de chaînes.
 * Cela permettra de vérifier si il existe un Makefile dans la liste
 * des argv.
 *
 * \param chaines  une liste de chaînes de caractères
 * \param nchaines la taille de nchaines
 * \param chaine   une chaîne de caractères 
 * \param nchaine  la taille de chaine

 * \return -1 si chaines ne contient pas chaine ou l'indice de la chaîne  
 */
static int contenir_chaine(char *chaines[], int nchaines, char *chaine, int nchaine) {
  int i;
  for(i = 0; i < nchaines; i++) {
    if(!strncmp(chaines[i], chaine, nchaine)) {
      return i;
    }
  }
  return -1;
}

/**
 * \brief Créer le programme qui va contenir soit tous les fichiers sources
 * .c du Makefile, soit l'ensemble des fichiers .c présents dans argv. 
 * Cela donne une alternative à notre projet, qui peut à la fois gérer
 * un Makefile mais également un ensemble de fichiers .c.
 *
 * \param n    taille du programme à analyser (nombre de caractère)
 * \param argc le nombre d'argument du main 
 * \param argv contenu des arguments du main (soit des fichiers.c, soit un Makefile) 

 * \return le contenu du programme dans un tableau de type de char 
 */
char * creer_prog(int *n, int argc, char *argv[]) {
  *n = MAX;
  char * prog = NULL;
  int mkf = -1;

  // Si mkf >= 0, alors lire le Makefile
  if((mkf = contenir_chaine(argv, argc, "Makefile", strlen("Makefile"))) >= 0) {
    int nmake = MAX, nbf = 0;
    char * makefile = lire_makefile(argv[mkf], &nmake);
    char ** fichiers = analyser_makefile(makefile, nmake, &nbf);
    prog = lire_fichiers(fichiers, nbf, n);
  }

  // Si mkf < 0, alors il n'y a pas de Makefile
  else
    prog = lire_fichiers(argv, argc, n);
  return prog;
}

/** \brief Lire les fichiers d'un programme et plaçant son contenu 
 * dans un tableau de char afin de pouvoir l'analyser plus tard.
 *
 * \param fichiers l'ensemble des fichiers d'un programme
 * \param nbf      le nombre de fonctions
 * \param nbcar    la taille du programme (nombre de caractères)
 * 
 * \return le contenu du programme dans un tableau de type de char 
 */
char * lire_fichiers(char ** fichiers, int nbf, int *nbcar) {
  char * prog = (char *)malloc(MAX * sizeof(*prog));
  assert(prog);
  memset(prog, 0, MAX);
  int f, c, i = 0;

  for(f = 0; f < nbf; f++) {
    FILE * fp = fopen(fichiers[f], "r");
    while((c = fgetc(fp)) != EOF) {
      prog[i] = c;
      i++;
      if(i == (*nbcar-1)) {
        *nbcar *= 2;
        prog = (char *)realloc(prog, *nbcar * sizeof(*prog));
        assert(prog);
      }
    }
    fclose(fp);
  }

  prog[i++] = '\0';
  *nbcar = i;
  return prog;
}

/** \brief Lire le fichier Makefile, le placer dans une chaîne 
 * et retourner la chaîne.
 *
 * \param fichier le fichier Makefile
 * \param nmake   la taille du Makefile (nombre de caractères)
 * 
 * \return un tableau de char contenant le fichier Makefile
 */
char * lire_makefile(char * fichier, int *nmake) {
  int c, i = 0;
  char * makefile = (char *)malloc(MAX * sizeof(*makefile));
  FILE * fp = fopen(fichier, "r");
  while((c = fgetc(fp)) != EOF) {
    makefile[i] = c;
    i++;
    if(i == (*nmake-1)) {
      *nmake *= 2;
      makefile = (char *)realloc(makefile, *nmake * sizeof(*makefile));
      assert(makefile);
    }
  }

  makefile[i++] = '\0';
  *nmake = i;
  fclose(fp);
  return makefile;
}

/** \brief Analyser le Makefile afin de savoir l'ensemble des fichiers .c
 * du programme.
 *
 * \param makefile le fichier Makefile
 * \param nmake    la taille du Makefile (nombre de caractères)
 * \param nbf      le nombre de fonctions sources utilisé par Makefile
 * 
 * \return le nom de l'ensemble des fichiers sources du Makefile
 */
char ** analyser_makefile(char * makefile, int nmake, int *nbf) {
  int im = 0, max = NF;
  char ** fichiers = (char **)malloc(max * sizeof(*fichiers));
  assert(fichiers);

  while(im < nmake) {
    if(match_fichier(makefile, &im, fichiers, nbf))
    if(*nbf == max-1) {
      max *= 2;
      fichiers = (char **)realloc(fichiers, max * sizeof(*fichiers));
      assert(fichiers);
    }
    im++;
  }
  return fichiers;
}

/** \brief Analyser le programme, c'est-à-dire reconnaître les fonctions
 * déclarées et les appels de fonctions qui vont nous permettre de
 * créer un graphe orienté (Mco, Lis).
 *
 * \param prog le programme
 * \param n    la taille du programme
 */
void analyser_prog(char * prog, int n) {
  clock_t start_t, end_t, total_t, times[6];
  int nbfois = 1, i;
  fprog_t fpr;
  char * cc_mco, * cc_lis; 
  int nbcc_mco, nbcc_lis;
  char ** cycles_lis, ** cycles_mco;
  int nbcycles_lis = 0, nbcycles_mco = 0;
  fpr = analyser_fcts(prog, n);
  analyser_callf(prog, n, &fpr);

  // Matrice compacte
  mco_t m;
  m = creer_mco(fpr);
  start_t = clock();
  for(i = 0; i < nbfois; i++)
    afficher_mco(m, fpr);
  end_t = clock();
  total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  times[4] = total_t;

  // Matrice compacte : Composantes connexes
  start_t = clock();
  for(i = 0; i < nbfois; i++)
    cc_mco = calculer_cc_mco(m, fpr);
  end_t = clock();
  total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  times[0] = total_t;
  nbcc_mco = compter_cc(cc_mco, fpr);
  afficher_cc_mco(m, fpr, cc_mco, nbcc_mco);

  // Matrice compacte : Cycles
  start_t = clock();
  for(i = 0; i < nbfois; i++) {
    nbcycles_mco = 0;
    cycles_mco = calculer_cycle_mco(m, fpr, cc_mco, nbcc_mco, &nbcycles_mco);  
  }
  end_t = clock();
  total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  times[1] = total_t;
  afficher_cycle_mco(m, fpr, cycles_mco, nbcycles_mco);

  // Liste de successeurs
  vec_t v;
  v = creer_lis(fpr);
  start_t = clock();
  for(i = 0; i < nbfois; i++)
    afficher_lis(v, fpr);
  end_t = clock();
  total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  times[5] = total_t;

  // Liste de successeurs : Composantes connexes
  start_t = clock();
  for(i = 0; i < nbfois; i++)
    cc_lis = calculer_cc_lis(v, fpr);
  end_t = clock();
  total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  times[2] = total_t;
  nbcc_lis = compter_cc(cc_lis, fpr);
  afficher_cc_lis(v, fpr, cc_lis, nbcc_lis);

  start_t = clock();
  for(i = 0; i < nbfois; i++) {
    nbcycles_lis = 0;
    cycles_lis = calculer_cycle_lis(v, fpr, cc_lis, nbcc_lis, &nbcycles_lis);
  }
  end_t = clock();
  total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  times[3] = total_t;
  afficher_cycle_lis(v, fpr, cycles_lis, nbcycles_lis);

  // Temps 
  printf("Temps pour le calcul de la CC   (Mco): %lu\n", times[0]);
  printf("Temps pour le calcul des cycles (Mco): %lu\n", times[1]);
  printf("Temps pour le calcul de la CC   (Lis): %lu\n", times[2]);
  printf("Temps pour le calcul des cycles (Lis): %lu\n", times[3]);
  printf("Temps pour l'affichage          (Mco): %lu\n", times[4]);
  printf("Temps pour l'affichage          (Lis): %lu\n", times[5]);

  liberer_mco(m);
  liberer_lis(v);
}

/** \brief Analyser les appels de fonctions dans chaque fonction du 
 * programme afin de trouver les successeurs de chaque noeud.
 *
 * \param prog le programme
 * \param n    la taille du programme
 * \param fpr  les fonctions déclarées du programmes et les appels
 */
void analyser_callf(char *prog, int n, fprog_t * fpr) {
  int i, is = 0, etat = 1;
  for(i = 0; i < fpr->nfcts; i++) {
    fpr->fcts[i].succ = (fonction_t *)malloc(fpr->nfcts * sizeof(*fpr->fcts[i].succ));
    assert(fpr->fcts[i].succ);
    fpr->fcts[i].nbs = 0;
  }

  i = 0;
  while(is < n) {
    if(match_fonction(prog, &is, etat, fpr, i))
      i++;
    is++;
  }
}

/** \brief Analyser les déclarations de fonctions dans le programme
 * (représentant un noeud dans le graphe) en vérifiant si il
 * existe un pattern de la forme : 
 * [nom_fonction] '(' [arg(s)] ')' '{' [instr(s)] '}'.
 *
 * \param prog le programme
 * \param n    la taille du programme
 *
 * \return structure fprog_t qui contient les fonctions déclarées du programmes et les appels
 */
fprog_t analyser_fcts(char *prog, int n) {
  int i = 0, is = 0, etat = 0;
  fprog_t fpr;
  fpr.nfcts = NF;
  fpr.fcts = (fonction_t *)malloc(fpr.nfcts * sizeof(*fpr.fcts));
  while(is++ < n) {
    if(match_fonction(prog, &is, etat, &fpr, i)) {
      i++;
      if(fpr.nfcts == i) {
        fpr.fcts = (fonction_t *)realloc(fpr.fcts, (fpr.nfcts *= 2) * sizeof(*fpr.fcts));
      }
    }
  }
  fpr.nfcts = i;
  return fpr;
}

/** \brief Afficher l'ensemble des fonctions déclarées et ses appels présentes 
 * dans le programme.
 *
 * \param fpr les fonctions déclarées du programmes et les appels
 */
void afficher_fcts(fprog_t fpr) {
  int i, j;
  printf("\n\n-- LES FONCTIONS DÉCLARÉES (%d) --\n", fpr.nfcts);
  for(i = 0; i < fpr.nfcts; i++) {
    printf("(%d)- %s\n", i, fpr.fcts[i].nom);
    for(j = 0; j < fpr.fcts[i].nbs; j++) {
      printf("  -> %s\n", fpr.fcts[i].succ[j].nom);
    }
  }
}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    fprintf(stderr, "Usage: ./adp <file(s)>.\n");
    exit(0);
  }

  int n = 0;
  char * prog = NULL;
  prog = creer_prog(&n, argc, argv);
  analyser_prog(prog, n);
  return 0;
}