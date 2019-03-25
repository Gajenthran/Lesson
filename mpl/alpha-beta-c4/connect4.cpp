#include <iostream>
#include <cstddef>
#include <vector>
#include "grid.hpp"
#include "solver.hpp"

/**
 * operator
 * 3_d, 2_d, 1_d, cell name
 */


std::byte operator "" _d(unsigned long long i){
  return std::byte(i);
}

int main() {
  Grid g;
  Solver s(5);
  g.initGrid();

//  int computerMove = s.getBestMove(g, 0_d, 1_d);
//  std::cout << computerMove << "\n\n";
//  g.putToken(computerMove, 0_d);
//  g.printGrid();
 
 do {
    // Player 1 Move
    char* col = new char[50];
    std::cin >> col;
    g.putToken(std::atoi(col), 1_d);

    // Player 2 Move 
    int computerMove = s.getBestMove(g, 0_d, 1_d);
    g.putToken(computerMove, 0_d);

    // Draw the grid
    g.printGrid();
    std::cout << "\n\n";
    if(g.check(computerMove, 0_d, 4)) break;
    
  } while(!g.isFull());
  
  return 0;
}
