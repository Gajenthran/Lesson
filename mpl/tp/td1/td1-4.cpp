#include <thread>
#include <iostream>

/*
 * Affiche l'id du thread actuel 
 */
void foo() {
  std::cout << "Thread " << std::this_thread::get_id() << "\n";
}

/*
 * Affiche l'id du thread actuel (foo), créer un nouveau thread qui 
 * exécute la fonction foo, et attend la fin du thread "th" avant de 
 * retourner la valeur du "main".
 */
int main() {
  foo(); // Afficher l'id du thread
  std::thread th(foo); // Créer un nouveau thread qui va appeler foo
  th.join();  // Attendre la fin du thread 
  return 0;
}
