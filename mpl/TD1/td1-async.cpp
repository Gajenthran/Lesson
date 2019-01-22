#include <thread>
#include <iostream>
#include <vector>
#include <future>

// Nombre de thread créé
#define NTHREAD 5
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
 * Recherche de la valeur max du tableau où chaque élément
 * représente le max obtenu par les threads.
 */
int maxTab(int *t, int debut, int fin) {
  int max = t[debut];
  for(int i = debut; i < fin; i++) {
    if(max < t[i])
      max = t[i];
  }
  return max;
}

/*
 * Recherche de la valeur max en comparant les différentes valeurs
 * obtenues grâces aux threads
 */
int maxTab_Future(std::future<int> * max) {
  int m = max[0].get(), tmp;
  for(int i = 1; i < NTHREAD; i++) {
    tmp = max[i].get();
    if(m < tmp)
      m = tmp;
  }
  return m;
}

int main() {
  // Création du tableau et des threads
  int t[NTAB], n = NTAB, i = 0;
  std::future<int> max[NTHREAD];
  init(t, n);

  int part = NTAB/NTHREAD, begin = 0, end = part;
  while(i != NTHREAD) { 
    max[i] = std::async(std::launch::async, maxTab, t, begin, end);
    end = (++i == NTHREAD-1) ? NTAB : end + part;
    begin += part;
  }

  // Affiche la valeur max obtenue grâce aux threads
  std::cout << "Max du tableau: " << maxTab_Future(max) << "\n";
  return 0;
}
