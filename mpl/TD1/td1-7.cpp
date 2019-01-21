#include <thread>
#include <iostream>

// Nombre de thread créé
#define NTHREAD 2
// Taille du tableau
#define NTAB 100

// La valeur max du tableau créé 
int max = -1;

/*
 * Initialisation du tableau de taille NTAB.
 */
void init(int *t, int n) {
  for(int i = 0; i < n; i++)
    t[i] = i;
}

/*
 * Recherche de la valeur max du tableau dans
 * l'intervalle donné.
 */
void maxTab(int *t, int debut, int fin) {
  int m = t[debut];   
  for(int i = debut; i < fin; i++) {
    if(m < t[i])
      m = t[i];
  }

  if(max < m)
    max = m;
}

int main() {
  // Création du tableau et des threads
  int t[NTAB], n = NTAB;
  init(t, n);
  std:: thread th[NTHREAD];

  // Chaque thread traite une partie du tableau
  th[0] = std:: thread(maxTab, t, 0, NTAB/2);
  th[1] = std:: thread(maxTab, t, NTAB/2, NTAB);
  
  // Attendre la fin de chaque thread
  for(int i = 0; i < NTHREAD; i++) {
    th[i].join();
  }
  
  // Affiche la valeur max obtenue grâce aux threads
  std::cout << "Max du tableau: " << max << "\n";
  return 0;
}
