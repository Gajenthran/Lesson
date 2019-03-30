#include <iostream>
#include <cstddef>
#include <vector>
#include "grid.hpp"
#include "solver.hpp"
#include <ctime>
#include <cstdlib>

/**
 * operator
 * 3_d, 2_d, 1_d
 * flags
 * int to byte
 * alpha beta
 */

std::byte operator "" _d(unsigned long long i){
  return std::byte(i);
}


int main() {
  Grid g;
  Solver s;
  g.initGrid();

  std::srand(std::time(nullptr));
  /* g.putToken(3, 0_d);

  g.putToken(2, 1_d);
  g.putToken(2, 0_d);

  g.putToken(1, 0_d);
  g.putToken(1, 1_d);
  g.putToken(1, 0_d);

  g.putToken(0, 0_d);
  g.putToken(0, 1_d);
  g.putToken(0, 0_d); */

  /* g.putToken(1, 1_d);
  g.putToken(1, 1_d);
  g.putToken(1, 1_d); */

  g.printGrid();

  /* int move = s.aiMove(g, true);
  g.putToken(move, 0_d); */

  std::vector<int> nodes;
  int computerMove = 0;
  do {
    // Player 1 Move (IA)
    // int r = std::rand()/((RAND_MAX + 1u)/W);
    int r = s.aiMove(g, false);
    g.putToken(r, 1_d);

    // Draw the grid
    g.printGrid();
    std::cout << "\n\n";
    if(g.check(r, 4)) break;

    // Player 2 Move (IA)
    int move = s.aiMove(g, true);
    g.putToken(move, 0_d); 

    // Draw the grid
    g.printGrid();
    std::cout << "\n\n";
    if(g.check(computerMove, 4)) break;
    
  } while(!g.isFull());

  g.printGrid();
  return 0;
}
