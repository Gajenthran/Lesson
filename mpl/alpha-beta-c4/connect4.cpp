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

  // std::cout << g.getKey() << "\n"; exit(0);
  // g.bbPutToken(0, 0);
  // if(g.bbIsColumnFull(2, 0)) std::cout << "full bbIsColumFull\n"; 
  // g.printGrid(); 
  // int move = s.computerMove(g, false);
  // int move = s.gnBestMove(g, true);
  // int move = s.gtBestMove(g, 1);
  // int move = s.computerMove(g, true);
  // std::cout << "move: " << move << "\n";

  // g.putToken(move, 1_b);

  int move = s.gnBestMove(g, 0);
  std::cout << move << "\n";
  g.bbPutToken(move, 1);
  //if(g.bbCheck(1)) std::cout <<"ggnaé\n";

  /* int move;
   do {
    // Player 1 Move (IA)
    // move = std::rand()/((RAND_MAX + 1u)/W);
    move = s.gnBestMove(g, 0);
    // move = s.computerMove(g, false);
    // move = s.computerMove(g, 0);
    g.bbPutToken(move, 0);
    g.putToken(move, 0_b);

    // Draw the grid
    g.printGrid();
    std::cout << "\n\n";
    if(g.bbCheck(0)) break;

    // Player 2 Move (IA)
    // move = s.computerMove(g, true);
    move = s.gnBestMove(g, 1);
    g.bbPutToken(move, 1); 
    g.putToken(move, 1_b);

    // Draw the grid
    g.printGrid();
    std::cout << "\n\n";
    if(g.bbCheck(1)) break;
  
  } while(!g.isFull()); */
  // if(g.bbCheckNextMove(2)) std::cout << "0 is full\n"; 
  // if(g.bbIsColumnFull(0)) std::cout << "1 is full\n";
  // if(g.bbCheck(1)) std::cout << "1 ttis full\n";
  g.printGrid();
  return 0;
}
