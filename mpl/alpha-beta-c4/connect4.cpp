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
  g.bbPutToken(0, 0); 

  // std::cout << g.getKey() << "\n"; exit(0);
  // g.putToken(0, 1_b); 
  // if(g.bbIsColumnFull(2, 0)) std::cout << "full bbIsColumFull\n"; 
  // g.printGrid(); 
  // int move = s.computerMove(g, false);
  // int move = s.gnBestMove(g, true);
  // int move = s.gtBestMove(g, 1);
  // int move = s.computerMove(g, true);
  // std::cout << "move: " << move << "\n";

  // g.putToken(move, 1_b);
  // g.bbPutToken(move);

  int move = s.gnBestMove(g, true);
  std::cout << move << "\n";

  /* int move;
   do {
    // Player 1 Move (IA)
    move = std::rand()/((RAND_MAX + 1u)/W);
    // move = s.computerMove(g, false);
    // move = s.gnBestMove(g, 1);
    g.bbPutToken(move, 0);
    g.putToken(move, 0_b);

    // Draw the grid
    g.printGrid();
    std::cout << "\n\n";
    if(g.bbCheck(0)) break;

    // Player 2 Move (IA)
    // move = s.computerMove(g, true);
    move = s.gnBestMove(g, true);
    g.bbPutToken(move, 1); 
    g.putToken(move, 1_b);

    // Draw the grid
    g.printGrid();
    std::cout << "\n\n";
    if(g.bbCheck(1)) break;
  
  } while(!g.isFull()); */
  if(g.bbCheckNextMove(2)) std::cout << "0 is full\n"; 
  if(g.bbIsColumnFull(0)) std::cout << "1 is full\n";
  g.printGrid();
  return 0;
}
