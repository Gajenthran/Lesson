/*!\file match.c
 *
 * \brief Vérification des différents pattern (différentes expressions) dans le programme.
 *
 * \author PANCHALINGAMOORTHY Gajenthran, gajenthran.panchalingamoorthy@etud.univ-paris8.fr
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "match.h"

/**
 * \brief Vérifier les accolades. Dans certains cas, elle analysera le bloc de
 * la fonction afin de trouver les appels de fonctions.
 *
 * \param s     string (chaîne de caractères) représentant le programme
 * \param is    indice du string (chaîne de caractère) du dernier élément parcouru
 * \param etat  etat (0: recherche des fonctions déclarées, 1: recherches des appels de fonctions)
 * \param fpr   les fonctions déclarées du programme et les appels
 * \param nfcts selon la valeur de etat (0: nombre de fonctions déclarées,
 * 1: indice de la fonction analysée afin de trouver les appels de fonctions)
 *
 * \return 1 si l'expression commence par une accolade ouvrante et se termine par une accolade
 * fermante
 */
int match_acc(char *s, int *is, int etat, fprog_t * fpr, int nfcts) {
  if(s[*is] == K_obra) {
    int obra = 0;
    while(s[*is]) {
      (*is)++;
      if(etat) match_callf(s, is, fpr, nfcts);

      if(s[*is] == K_obra)
        obra++;
      else if (s[*is] == K_cbra) {
        obra--;
        if(obra < 0) {
          (*is)++; 
          return 1;
        }
      }
    }
    return 0;
  }
  return 0;
}

/**
 * \brief Vérifier les parenthèses.
 *
 * \param s    string (chaîne de caractères) représentant le programme
 * \param is   indice du string (chaîne de caractère) du dernier élément parcouru
 * \param etat etat (0: recherche des fonctions déclarées, 1: recherches des appels de fonctions)
 *
 * \return 1 si l'expression commence par une parenthèse ouvrante et se termine par une parenthèse
 * fermante
 */
int match_par(char *s, int *is) {
  if(s[*is] == K_opar) {
    int opar = 0;
    while(s[*is]) {
      (*is)++;
      if(s[*is] == K_opar)
        opar++;
      else if(s[*is] == K_cpar) {
        opar--;
        if(opar < 0) {
          (*is)++;
          return 1;
        }
      }
    }
    return 0;
  }
  return 0;
}

/**
 * \brief Vérifier le symbole passé en paramètre.
 *
 * \param s  string (chaîne de caractères) représentant le programme
 * \param is indice du string (chaîne de caractère) du dernier élément parcouru
 * \param c  caractère
 *
 * \return 1 si la chaîne contient le caractère sinon 0

 */
int match_symbole(char *s, int is, char c) {
  if(s[is] == c)
    return 1;
  return 0;
}

/**
 * \brief Vérifier les commentaires.
 *
 * \param s  string (chaîne de caractères) représentant le programme
 * \param is indice du string (chaîne de caractère) du dernier élément parcouru
 */
void match_comment(char *s, int *is) {
  if((s[*is] == K_slash && s[*is+1] == K_slash) ||
     (s[*is] == K_slash && s[*is+1] == K_star)) {
    (*is)++;
    char c = s[*is];
    while(s[*is]) {
      (*is)++;
      if(c == K_slash && 
         s[*is] == K_nbsp) {
        (*is)++;
        return;
      }
      
      else if(c == K_star && 
              s[*is] == K_slash && s[*is-1] == K_star) {
        (*is)++;
        return;
      }
    }
  }
}

/**
 * \brief Vérifier les chaînes de caractères.
 *
 * \param s  string (chaîne de caractères) représentant le programme
 * \param is indice du string (chaîne de caractère) du dernier élément parcouru
 */
void match_chaine(char *s, int *is) {
  if(s[*is] == K_quote || s[*is] == K_dquote) {
    char c = s[*is];
    while(s[*is]) {
      (*is)++;
      if(c == s[*is] && s[*is-1] != K_bslash) { 
        (*is)++;
        return;
      }
    }
  }
}

/**
 * \brief Ignorer les espaces, les étoiles et les retours à la lignes.
 *
 * \param s  string (chaîne de caractères) représentant le programme
 * \param is indice du string (chaîne de caractère) du dernier élément parcouru
 */
void match_blanc(char *s, int *is) {
  if(s[*is] == K_nbsp || 
     s[*is] == K_sp || 
     s[*is] == K_star) {
    while(s[*is]) {
      if(s[*is] == K_nbsp || 
         s[*is] == K_sp || 
         s[*is] == K_star) {
        (*is)++;
        continue;
      }
      return;
    }
  }
}

/**
 * \brief Ignorer les espaces, les étoiles, les retours à la ligne, 
 * les chaînes de caractères (entre guillemets) et les commentaires.
 *
 * \param s  string (chaîne de caractères) représentant le programme
 * \param is indice du string (chaîne de caractère) du dernier élément parcouru
 */
void match_ignore(char *s, int *is) {
  match_blanc(s, is);
  match_chaine(s, is);
  match_comment(s, is);
}

/**
 * \brief Vérifier si l'expression est constituée de 
 * lettres de l'alphabet et/ou de nombres (en comptant les tirets).
 *
 * \param s  string (chaîne de caractères) représentant le programme
 * \param is indice du string (chaîne de caractère) du dernier élément parcouru
 *
 * \return 1 si l'expression est bien un alphanumérique (composé possible d'un tiret) sinon 0
 */
int match_alphanum(char *s, int *is) {
  if((s[*is] >= K_A && s[*is] <= K_z) ||
     (s[*is] >= K_0 && s[*is] <= K_9) ||
      s[*is] == K_dash) {
    while(s[*is]) {
      (*is)++;
      if((s[*is] >= K_A && s[*is] <= K_z) ||
         (s[*is] >= K_0 && s[*is] <= K_9) ||
          s[*is] == K_dash)
        continue;
      return 1;
    }
    return 1;
  }
  return 0;
}

/**
 * \brief Vérifier la structure d'une fonction.
 *
 * \param s     string (chaîne de caractères) représentant le programme
 * \param is    indice du string (chaîne de caractère) du dernier élément parcouru
 * \param etat  etat (0: recherche des fonctions déclarées, 1: recherches des appels de fonctions)
 * \param fpr   les fonctions déclarées du programme et les appels
 * \param nfcts selon la valeur de etat (0: nombre de fonctions déclarées,
 * 1: indice de la fonction analysée afin de trouver les appels de fonctions)
 *
 * \return la valeur de retour de la fonction match_args si l'expression suivante est un nom de 
 * fonction, sinon 0
 */
int match_fonction(char *s, int *is, int etat, fprog_t * fpr, int nfcts) {
  match_ignore(s, is);
  int d = *is-1;
  if(match_alphanum(s, is))
    return match_args(s, is, d, etat, fpr, nfcts);
  return 0;
}

/**
 * \brief Vérifier la structure des arguments d'une fonction.
 *
 * \param s     string (chaîne de caractères) représentant le programme
 * \param is    indice du string (chaîne de caractère) du dernier élément parcouru
 * \param d     début de la chaîne de caractère de la fonction 
 * \param etat  etat (0: recherche des fonctions déclarées, 1: recherches des appels de fonctions)
 * \param fpr   les fonctions déclarées du programme et les appels
 * \param nfcts selon la valeur de etat (0: nombre de fonctions déclarées,
 * 1: indice de la fonction analysée afin de trouver les appels de fonctions)
 *
 * \return la valeur de retour de la fonction match_bloc si l'expression suivante ressemble à la structure 
 * des arguments d'une fonction sinon 0
 */
int match_args(char *s, int *is, int d, int etat, fprog_t * fpr, int nfcts) {
  int f = *is;
  match_ignore(s, is);
  if(match_par(s, is))
    return match_bloc(s, is, d, f, etat, fpr, nfcts);
  return 0;
}

/**
 * \brief Vérifier la structure d'un bloc d'une fonction afin de vérifier si il existe
 * des appels de fonctions dans le bloc.
 *
 * \param s     string (chaîne de caractères) représentant le programme
 * \param is    indice du string (chaîne de caractère) du dernier élément parcouru
 * \param d     début de la chaîne de caractère de la fonction 
 * \param f     fin de la chaîne de caractère de la fonction 
 * \param etat  etat (0: recherche des fonctions déclarées, 1: recherches des appels de fonctions)
 * \param fpr   les fonctions déclarées du programme et les appels
 * \param nfcts selon la valeur de etat (0: nombre de fonctions déclarées,
 * 1: indice de la fonction analysée afin de trouver les appels de fonctions) 
 *
 * \return 1 si l'expression suivante est un un bloc de fonction sinon 0
 */
int match_bloc(char *s, int *is, int d, int f, int etat, fprog_t * fpr, int nfcts) {
  match_ignore(s, is);

  if(match_acc(s, is, etat, fpr, nfcts)) {
    if(!etat)
      fpr->fcts[nfcts].nom = strndup(&s[d], f-d);
    return 1;
  }

  return 0;
}

/**
 * \brief Vérifier la structure des appels de fonctions.
 *
 * \param s     string (chaîne de caractères) représentant le programme
 * \param is    indice du string (chaîne de caractère) du dernier élément parcouru
 * \param d     début de la chaîne de caractère de la fonction 
 * \param f     fin de la chaîne de caractère de la fonction 
 * \param fpr   les fonctions déclarées du programme et les appels
 * \param nfcts selon la valeur de etat (0: nombre de fonctions déclarées,
 * 1: indice de la fonction analysée afin de trouver les appels de fonctions)
 *
 * \return 1 si l'expression suivante est une fonction (appelée) sinon 0
 */
int match_callf(char *s, int *is, fprog_t * fpr, int nfcts) {
  match_ignore(s, is);
  int d = *is, f;
  if(!match_alphanum(s, is))
    return 0;
  int nbs = fpr->fcts[nfcts].nbs;
  f = *is;

  match_ignore(s, is);
  if(match_symbole(s, *is, '(')) {
    if((cmp_nchaines(*fpr, &s[d], f-d) >= 0) &&
       (!cmp_callf(*fpr, &s[d], f-d, nfcts))) {
      fpr->fcts[nfcts].succ[nbs].nom = strndup(&s[d], f-d);
      fpr->fcts[nfcts].nbs++;
    }
    return 1;
  }

  (*is)--;
  return 0;
}

/**
 * \brief Vérifier si il s'agit d'un fichier .c.
 *
 * \param s        string (chaîne de caractères) représentant le programme
 * \param is       indice du string (chaîne de caractère) du dernier élément parcouru
 * \param fichiers l'ensemble des fichiers .c 
 * \param nbf      nombre de fonctions
 *
 * \return 1 si l'expression suivante est un fichier.c sinon 0
 */
int match_fichier(char *s, int *is, char ** fichiers, int *nbf) {
	int d = *is;
	if(match_alphanum(s, is) &&
		 match_symbole(s, *is, '.') &&
		 (*is)++ &&
		 match_symbole(s, *is, 'c')) {
		(*is)++;
		fichiers[*nbf] = strndup(&s[d], (*is)-d);
		(*nbf)++;
		return 1;
	}
	return 0;
}

/**
 * \brief Vérifier la présence d'une fonction dans l'ensemble des fonctions.
 *
 * \param fpr les fonctions déclarées du programme et les appels
 * \param fct indice du string (chaîne de caractère) du dernier élément parcouru
 *
 * \return l'indice si la fonction est présente dans le vecteur de fonction sinon -1
 */
int cmp_chaines(fprog_t fpr, char * fct) {
  int i;
  for(i = 0; i < fpr.nfcts; i++) {
    if(strcmp(fpr.fcts[i].nom, fct) == 0)
      return i;
  }
  return -1;
}

/**
 * \brief Vérifier la présence des n premiers caractères d'une fonction dans
 * l'ensemble des fonctions.
 *
 * \param fpr les fonctions déclarées du programme et les appels
 * \param fct fonction
 * \param len la taille de la fonction fct
 *
 * \return l'indice si la fonction est présente dans le vecteur de fonction sinon -1
 */
int cmp_nchaines(fprog_t fpr, char * fct, int len) {
  int i;
  for(i = 0; i < fpr.nfcts; i++) {
    if((len == strlen(fpr.fcts[i].nom)) && 
       (!strncmp(fpr.fcts[i].nom, fct, len)))
      return i;
  }
  return -1;
}

/**
 * \brief Vérifier si la fonction fct est un des successeurs de la fonction
 * d'indice n.
 *
 * \param fpr les fonctions déclarées du programme et les appels
 * \param fct fonction
 * \param len taille de la fonction fct
 * \param n l'indice de fpr
 *
 * \return 1 si la fonction est présente dans le vecteur de fonctions sinon 0
 */
int cmp_callf(fprog_t fpr, char * fct, int len, int n) {
  int isuc;
  for(isuc = 0; isuc < fpr.fcts[n].nbs; isuc++) {
    if((len == strlen(fpr.fcts[n].succ[isuc].nom)) &&
       (!strncmp(fpr.fcts[n].succ[isuc].nom, fct, len)))
      return 1;
  }
  return 0;
}

/**
 * \brief Vérifier si l'indice du noeud/de la fonction est dans la pile.
 *
 * \param pile pile contenant les indices des noeuds parcourus
 * \param n    taille de la pile
 * \param in   indice du noeud
 *
 * \return l'indice si la pile contient l'indice du noeud sinon -1
 */
int deja_visite(char * pile, int n, int in) {
  int i;
  for(i = 0; i < n; i++) {
    if(pile[i] == in)
      return i;
  }

  return -1;
}