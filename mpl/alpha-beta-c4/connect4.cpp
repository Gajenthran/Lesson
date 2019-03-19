#include <iostream>
#include "grid.hpp"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cstddef>

std::byte operator "" _d(unsigned long long i){
  return std::byte(i);
} 

int main() {
  Grid g(W, H);
  g.initGrid();
  g.putToken(0, 1_d);

  g.putToken(0, 1_d);
  g.putToken(1, 1_d);


  if(g.isFull())
    std::cout << "isFull" << "\n";
  g.printGrid();
  /* std::srand(std::time(nullptr));
  std::byte j = 0_d;
  int i = 1;
  int r = std::rand() % W;
  while(i) {
    r = std::rand() % W;
    j = std::byte(((int(j) + 1) % 2));
    std::cout << "r vaut : " << r << "\n\n\n";
    i++;
    if(g.putToken(r, j))
      break;
  }

  std::cout << "nb coups : " <<i << "\n\n\n";
  g.printGrid(); */
  return 0;
}
