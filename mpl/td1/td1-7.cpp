#include <thread>
#include <iostream>

// Nombre de thread créé
#define NTHREAD 4
// Taille du tableau
#define NTAB 100

/*
 * Initialisation du tableau de taille NTAB.
 */
void init(int *t, int n) {
  for(int i = 0; i < n; i++)
    t[i] = i;
}

/*
 * Recherche de la valeur max du tableau dans
 * l'intervalle donné pour les threads
 */
void maxTab_thread(int *t, int debut, int fin, int *max) {
  int m = t[debut];   
  for(int i = debut; i < fin; i++) {
    if(m < t[i])
      m = t[i];
  }

  *max = m;
}

/*
 * Recherche de la valeur max du tableau où chaque élément
 * représente le max obtenu par les threads.
 */
int maxTab(int *t, int n) {
  int max = t[0];
  for(int i = 0; i < n; i++) {
    if(max < t[i])
      max = t[i];
  }
  return max;
}

int main() {
  // Création du tableau et des threads
  int t[NTAB], n = NTAB, max[NTHREAD], i = 0;
  std:: thread th[NTHREAD];
  init(t, n);

  int part = NTAB/NTHREAD, begin = 0, end = part;
  while(i != NTHREAD) {
    th[i] = std::thread(maxTab_thread, t, begin, end, &max[i]);
    end = (++i == NTHREAD-1) ? NTAB : end + part;
    begin += part;
  }
  
  // Attendre la fin de chaque thread
  for(int i = 0; i < NTHREAD; i++)
    th[i].join();
  
  // Affiche la valeur max obtenue grâce aux threads
  std::cout << "Max du tableau: " << maxTab(max, NTHREAD) << "\n";
  return 0;
}
