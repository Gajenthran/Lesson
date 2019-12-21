#include <thread>
#include <iostream>

// Nombre de thread créé
#define N 99

int glob = 0;
 
/*
 * Incrémentation de la variable globale "glob"
 * afin de comprendre le comportement des threads.
 */
void inc() {
  for(int i = 0; i < 1; i++) 
    glob++;
}

int main() {
  inc();

  // Création de N threads
  std:: thread th[N];

  // Lancer de 100 threads
  for(int i = 0; i < N; i++)
    th[i] = std:: thread(inc);
  
  // Attendre la fin de chaque thread
  for(int i = 0; i < N; i++)
    th[i].join();
  
  // Afficher la valeur de "glob"
  std::cout << "glob: " << glob << "\n";
  return 0;
}
