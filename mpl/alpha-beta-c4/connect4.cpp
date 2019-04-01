#include <iostream>
#include <cstddef>
#include <vector>
#include "grid.hpp"
#include "solver.hpp"
#include <ctime>
#include <cstdlib>

/**
 * operator
 * 3_b, 2_b, 1_b
 * bitmap
 * transposition table
 * alpha beta
 */

int main() {
  Grid g;
  Solver s;
  g.initGrid();

  std::srand(std::time(nullptr));
  /* g.putToken(3, 0_b);

  g.putToken(2, 1_b);
  g.putToken(2, 0_b);

  g.putToken(1, 0_b);
  g.putToken(1, 1_b);
  g.putToken(1, 0_b);

  g.putToken(0, 0_b);
  g.putToken(0, 1_b);
  g.putToken(0, 0_b); */

  /* g.putToken(3, 1_b);
  g.putToken(1, 1_b);
  g.putToken(2, 1_b);
  g.putToken(4, 0_b); */

  int move = s.minFirst(g, -1000, 1000);
  g.putToken(move, 1_b);

  g.printGrid();

  /* int move;
  do {
    // Player 1 Move (IA)
    // int r = std::rand()/((RAND_MAX + 1u)/W);
    move = s.computerMove(g, false);
    g.putToken(move, 1_b);

    // Draw the grid
    g.printGrid();
    std::cout << "\n\n";
    if(g.check(move, 4)) exit(0);

    // Player 2 Move (IA)
    move = s.computerMove(g, true);
    g.putToken(move, 0_b); 

    // Draw the grid
    g.printGrid();
    std::cout << "\n\n";
    if(g.check(move, 4)) exit(0);
    
  } while(!g.isFull()); */

  g.printGrid();
  return 0;
} 
