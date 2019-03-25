#include <algorithm>
#include <iostream>
#include <vector>
#include <cstddef>
#include "solver.hpp"

Solver::Solver(int maxDepth) {
  depth_ = maxDepth;
}

int Solver::evaluate(Grid &g, std::byte cur_p, std::byte opp_p, int depth) {
  std::byte t;
  for(int i = 0; i < W; i++) {
    t = g.getToken(i, 0);
    if(int(t) != 3) {
      if(!g.isColumnFull(i) && g.check(i, t, 4)) {
        if(t == cur_p) {
          return 1000 * (depth + 1);
        }
        else {
          return -1000 * (depth + 1);
        }
      }
    }
  }
  return 0;
}

std::pair<int, int> Solver::negamax(Grid &g, int col, std::byte cur_p, std::byte opp_p, int alpha, int beta, int depth) {
  int ev = evaluate(g, cur_p, opp_p, depth);
  if(abs(ev) >= 1000 || depth == 0)
    return std::pair<int, int>(ev, col);

  int bestScore = -1000, bestMove = -1;
  for(int i = 0; i < W; i++) {
    if(!g.isColumnFull(i)) {
      g.putToken(i, cur_p);
      int score = -(negamax(g, i, opp_p, cur_p, -beta, -alpha, depth - 1).first);
      g.removeToken(i, cur_p);
      if(score > bestScore) { bestScore = score; bestMove = i; }
      if(score > alpha) alpha = score; 
      if(alpha >= beta) { bestScore = score; bestMove = i; break; }
    }
  }
  return std::pair<int, int>(bestScore, bestMove);
}


void printNodeScore(std::vector<std::pair<int, int>>& v) {
  for(int i = 0; i < v.size(); i++) {
    std::cout << i << ": " << v[i].first << "\n";
  }
  std::cout << "\n";
}

int Solver::getBestMoveFromNode(std::vector<std::pair<int, int>>& v) {
  int score = v[0].first, move = 0;
  for(int i = 1; i < v.size(); i++) {
    if(v[i].first > score) {
      score = v[i].first;
      move = i;
    }
  }
  return move;
}

int Solver::getBestMove(Grid &g, std::byte cur_p, std::byte opp_p) {
  std::vector<std::pair<int, int>> nodes;
  std::pair<int, int> n;
  for(int i = 0; i < W; i++) {
    g.putToken(i, cur_p);
    std::pair<int, int> n = negamax(g, i, opp_p, cur_p, -1000, 1000, depth_);
    g.removeToken(i, cur_p);
    nodes.push_back(std::pair<int, int>(-n.first, n.second));
  }

  printNodeScore(nodes);
  if(n.second == -1) {
    std::cout << "Erreur position.\n";
    exit(0);
  }
  return getBestMoveFromNode(nodes);
}


int Solver::getMoveFromCol(Grid &g, int col, std::byte cur_p, std::byte opp_p) {
  g.putToken(col, cur_p);
  std::pair<int, int> n = negamax(g, col, opp_p, cur_p, -1000, 1000, depth_);
  g.removeToken(col, cur_p);

  return n.first;
}
