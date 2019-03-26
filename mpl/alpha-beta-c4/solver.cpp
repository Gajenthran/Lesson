#include <algorithm>
#include <iostream>
#include <vector>
#include <cstddef>
#include <future>
#include <thread>
#include "solver.hpp"

Solver::Solver() {
}

void Solver::printNodeScore(std::vector<int>& v) {
  for(int i = 0; i < v.size(); i++) {
    std::cout << i << ": " << v[i] << "\n";
  }
  std::cout << "\n";
}

int Solver::evaluate(Grid &g, std::byte cur_p, std::byte opp_p) {
  std::byte t;
  for(int i = 0; i < W; i++) {
    t = g.getToken(i, 0);
    if(int(t) != 3) {
      if(!g.isColumnFull(i) && g.check(i, t, 4)) {
        if(t == cur_p) {
          return 1000;
        }
        else {
          return -1000;
        }
      }
    }
  }
  return 0;
}

int Solver::negamax(Grid &g, int col, std::byte cur_p, std::byte opp_p, int alpha, int beta) {
  int ev = evaluate(g, cur_p, opp_p);
  if(abs(ev) >= 1000)
    return ev;

  if(g.isFull())
    return 0;

  int bestScore = -1000;
  for(int i = 0; i < W; i++) {
    if(!g.isColumnFull(i)) {
      g.putToken(i, cur_p);
      int score = -negamax(g, i, opp_p, cur_p, -beta, -alpha);
      g.removeToken(i, cur_p);
      if(score > bestScore) bestScore = score;
      if(score > alpha) alpha = score; 
      if(alpha >= beta) { bestScore = score; break; }
    }
  }
  return bestScore;
}

int Solver::getScoreFromCol(Grid &g, int col, std::byte cur_p, std::byte opp_p) {
  Grid g2 = g;
  int n = -10;
  g2.putToken(col, cur_p);
  n = -negamax(g2, col, opp_p, cur_p, -1000, 1000);
  return n;
}


int Solver::getBestMove(Grid &g, std::byte cur_p, std::byte opp_p) {
  std::vector<int> nodes;
  int n;
  for(int i = 0; i < W; i++) {
    n = getScoreFromCol(g, i, cur_p, opp_p);
    nodes.push_back(n);
  }
  return getBestMoveFromNode(nodes);
}


int Solver::getBestMoveFromNode(std::vector<int>& v) {
  int score = v[0], move = 0;
  for(int i = 1; i < v.size(); i++) {
    if(v[i] > score) {
      score = v[i];
      move = i;
    }
  }
  return move;
}


int Solver::computerMove(Grid &g, std::byte cur_p, std::byte opp_p) {
  std::vector<std::future<int>> future_results;
  for(int i = 0; i < W; i++) 
    future_results.push_back(std::async(&Solver::getScoreFromCol, this, std::ref(g), i, cur_p, opp_p));
  
  /* for(int i = 0; i < W; i++) 
    future_results[i].wait(); */
  
  std::vector <int> results;
  std::cout << "Node:\n";
  std::transform(begin(future_results), end(future_results), back_inserter(results), [](auto& x) { return x.get(); });
  return getBestMoveFromNode(results);
}
