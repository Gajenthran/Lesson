#include "solver.hpp"
#include <algorithm>
#include <iostream>
#include <cstddef>

Solver::Solver(int maxDepth) {
	depth_ = maxDepth;
}

int Solver::evaluate(Grid &g, std::byte cur_p, std::byte opp_p) {
  std::byte t;
  for(int i = 0; i < W; i++) {
    t = g.getToken(i, 0);
    if(int(t) != 3) {
      if(!g.isColumnFull(i) && g.check(i, t, 4)) {
        if(t == cur_p)
          return 1000;
        return -1000;
      }
    }
  }
  return 0;
}

std::pair<int, int> Solver::negamax(Grid &g, std::byte cur_p, std::byte opp_p, int alpha, int beta, int depth) {
  int ev = evaluate(g, cur_p, opp_p);
  if(abs(ev) == 1000 || depth == 0)
    return std::pair<int, int>(ev, 0);

  int bestScore = -1000, bestMove = -1;
  for(int i = 0; i < W; i++) {
    if(!g.isColumnFull(i)) {
      g.putToken(i, cur_p);
      int score = -(negamax(g, opp_p, cur_p, -beta, -alpha, depth - 1).first);
      g.removeToken(i, opp_p);
      if(score > bestScore) { bestScore = score; bestMove = i; }
      if(score > alpha) alpha = score; 
      if(alpha >= beta) { bestScore = score; bestMove = i; break; }
    }
  }
  return std::pair<int, int>(bestScore, bestMove);
}

int Solver::getBestMove(Grid &g, std::byte cur_p, std::byte opp_p) {
	std::pair<int, int> n = negamax(g, cur_p, opp_p, -1000, 1000, depth_);
    std::cout << n.first << "\n";
    std::cout << n.second << "\n";
    if(n.second == -1) {
      std::cout << "Erreur position.\n";
      exit(0);
    }
    return n.second;
}

/* void printVec(std::vector<int>& v) {
  for(int i = 0; i < v.size(); i++) {
    std::cout << i << ": " << v[i] << "\n";
  }
  std::cout << "\n";
}

void eachNode(Grid &g, std::byte cur_p, std::byte opp_p, int alpha, int beta, int depth) {
  std::vector<int> best;
  for(int i = 0; i < W; i++) {
    g.putToken(i, cur_p);
    best.push_back((neg_alpha_beta(g, opp_p, cur_p, alpha, beta, depth).first));
    g.removeToken(i, cur_p);
  }
  printVec(best);
} */
