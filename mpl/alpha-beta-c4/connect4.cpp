#include <iostream>
#include <cstddef>
#include <vector>
#include "grid.hpp"
#include "solver.hpp"

/**
 * operator
 * 3_d, 2_d, 1_d
 * flags
 * std::pair inutile? bestMove
 * copy Grid
 * make clean
 * int to byte
 */


std::byte operator "" _d(unsigned long long i){
  return std::byte(i);
}


int main() {
  Grid g;
  Solver s;
  g.initGrid();

  std::vector<int> nodes;
  int computerMove;
 do {
    // Player 1 Move (Hum)
    /* char* col = new char[50];
    std::cin >> col;
    g.putToken(std::atoi(col), 1_d); */

    // Player 1 Move (IA)
    nodes.clear();
    for(int i = 0; i < W; i++) {
      if(g.isColumnFull(i)) { 
        nodes.push_back(-1000);
        continue;
      }
      nodes.push_back(s.getScoreFromCol(g, i, 1_d, 0_d));
    }
    s.printNodeScore(nodes);
    computerMove = s.getBestMoveFromNode(nodes);
    g.putToken(computerMove, 1_d);

    // Draw the grid
    g.printGrid();
    std::cout << "\n\n";
    if(g.check(computerMove, 1_d, 4)) break;


    // Player 2 Move (IA)
    // int computerMove = s.computerMove(g, 0_d, 1_d);
    // g.putToken(computerMove, 0_d);
    nodes.clear();
    for(int i = 0; i < W; i++) {
      if(g.isColumnFull(i)) { 
        nodes.push_back(-1000);
        continue;
      }
      nodes.push_back(s.getScoreFromCol(g, i, 0_d, 1_d));
    }
    computerMove = s.getBestMoveFromNode(nodes);
    g.putToken(computerMove, 0_d);

    // Draw the grid
    g.printGrid();
    std::cout << "\n\n";
    if(g.check(computerMove, 0_d, 4)) break;
    
  } while(!g.isFull());

  return 0;
}
