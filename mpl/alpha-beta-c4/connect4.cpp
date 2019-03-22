#include <iostream>
#include "grid.hpp"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cstddef>
#include <cstdio>

/**
 * operator
 * clone tab
 * segment fault when playing + possibleMove
 * 3_d, 2_d, 1_d, cell name
 */

std::byte operator "" _d(unsigned long long i){
  return std::byte(i);
} 


int negamax(Grid &g, std::byte player, int * pos) {
  if(g.isFull())
    return 0;

  for(int i = 0; i < W; i++) {
    if(!g.isColumnFull(i) && g.check(i, player)) {
      *pos = i;
      return W * H+1 - g.getCountToken()/2;
    }
  }

  int best = -W * H;
  for(int i = 0; i < W; i++) {
    if(!g.isColumnFull(i)) {
      Grid g2 = g;
      g2.putToken(i, player);
      int score = -negamax(g2, std::byte(((int(player) + 1) % 2)), pos);
      if(score > best) {
        *pos = i;
        best = score;
      }
    }
  }
  return best;
}

int negamax_alpha_beta(Grid &g, std::byte player, int alpha, int beta, int * pos) {
  if(g.isFull())
    return 0;

  for(int i = 0; i < H; i++) {
    if(!g.isColumnFull(i) && g.check(i, player)) {
      *pos = i;
      return (W * H + 1 - g.getCountToken())/2;
    }
  }

  int max = (W * H - 1 - g.getCountToken())/2;
  if(beta > max) {
    beta = max;
    if(alpha >= beta)
      return beta;
  }

  for(int i = 0; i < H; i++) {
    if(!g.isColumnFull(i)) {
      Grid g2 = g;
      g2.putToken(i, player);
      int score = -negamax_alpha_beta(g2, std::byte(((int(player) + 1) % 2)), -beta, -alpha, pos);
      if(score >= beta) {
        *pos = i;
        return score;
      }
      if(score >= alpha)
        alpha = score; 
    }
  }
  return alpha;
}

int main() {
  Grid g;
  g.initGrid();

/*  Grid g2 = g;
  negamax_alpha_beta(g2, 0_d, -1, -1, &pos);
  g.putToken(pos, 0_d);
  g.printGrid(); */

  while(!g.isFull()) {
    int pos = 0;
    char* col = new char[50];
    std::cin >> col;
    g.putToken(std::atoi(col), 1_d);

    Grid g2 = g;
    negamax_alpha_beta(g2, 0_d, -1, -1, &pos);
    std::cout << pos << "\n";
    g.putToken(pos, 0_d);
    g.printGrid();
    
    std::cout << "\n\n";
  }

  if(g.isFull())
    std::cout << "Full\n";

  return 0;
}
