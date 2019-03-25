#include "solver.hpp"
#include <algorithm>


std::byte operator "" _b(unsigned long long i){
  return std::byte(i);
} 
int evaluate(Grid &g, std::byte cur_p, std::byte opp_p) {
  std::byte t;
  for(int i = 0; i < W; i++) {
    t = g.getToken(i, 0);
    if(t != 3_d) {
      if(!g.isColumnFull(i) && g.check(i, t, 4)) {
        if(t == cur_p)
          return 1000;
        return -1000;
      }
    }
  }
  return 0;
}

std::pair<int, int> neg_alpha_beta(Grid &g, std::byte cur_p, std::byte opp_p, int alpha, int beta, int depth) {
  int ev = evaluate(g, cur_p, opp_p);
  if(abs(ev) == 1000 || depth == 0)
    return std::pair<int, int>(ev, 0);

  int bestScore = -1000, bestMove = -1;
  for(int i = 0; i < W; i++) {
    if(!g.isColumnFull(i)) {
      g.putToken(i, cur_p);
      int score = -(neg_alpha_beta(g, opp_p, cur_p, -beta, -alpha, depth - 1).first);
      g.removeToken(i, opp_p);
      if(score > bestScore) { bestScore = score; bestMove = i; }
      if(score > alpha) alpha = score; 
      if(alpha >= beta) { bestScore = score; bestMove = i; break; }
    }
  }
  return std::pair<int, int>(bestScore, bestMove);
}