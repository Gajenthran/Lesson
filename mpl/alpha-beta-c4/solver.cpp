#include <iostream>
#include <vector>
#include <cstddef>
#include <future>
#include <thread>
#include "solver.hpp"

Solver::Solver() {
  for(int i = 0; i < W; i++) {
      colOder_[i] = W/2 + (1 - 2 * (i % 2)) * (i + 1)/2;
  }
}

// MAX : IA : 0_b : X ::  1000
// MIN : J1 : 1_b : O :: -1000
int Solver::evaluate(Grid &g) {
  std::byte t;
  for(int i = 0; i < W; i++) {
    t = g.getToken(i);
    if(t != 3_b) {
      if(g.check(i, 4)) {
        if(t == 0_b) {
          return 1000 - g.getNbToken();
        }
        else {
          return -1000 + g.getNbToken();
        }
      }
    }
  }
  return 0;
}

int Solver::maxChild(Grid &g, int c, int alpha, int beta) {
  int score = -1000;
  if(!g.isColumnFull(colOder_[c])) {
    nbNodes_ = 0;
    Grid g2 = g;
    g2.putToken(colOder_[c], 0_b);
    score = min(g2, alpha, beta);
    std::cout << colOder_[c] << " node : " << nbNodes_ << "\n";
    std::cout << colOder_[c] << " score : " << score << "\n\n";
  }
  return score;
}

int Solver::maxNodes(Grid &g, int alpha, int beta) {
  int maxScore = -1000, maxMove = -1;
  for(int c = 0; c < W; c++) {
    nbNodes_ = 0;
    int score = maxChild(g, c, alpha, beta);
    if(score > maxScore) { maxScore = score; maxMove = colOder_[c]; alpha = score; }
    if(alpha >= beta) return maxMove;
  }

  return maxMove;
}

int Solver::computerMove(Grid &g, int thread) {
  if(thread)
    return maxNodes(g, -1000, 1000);

  std::vector<std::future<int>> future_results;
  for(int c = 0; c < W; c++) 
    future_results.push_back(std::async(&Solver::maxChild, this, std::ref(g), c, -1000, 1000));

  std::vector<int> results;
  std::transform(begin(future_results), end(future_results), back_inserter(results), [](auto& x) { return x.get(); });
  return colOder_[getBestChild(results)];
}

int Solver::maxFirst(Grid &g, int alpha, int beta) {
  int maxScore = -1000, maxMove = -1;
  for(int c = 0; c < W; c++) {
    nbNodes_ = 0;
    if(!g.isColumnFull(colOder_[c])) {
      Grid g2 = g;
      g2.putToken(colOder_[c], 0_b);
      int score = min(g2, alpha, beta);
      std::cout << colOder_[c] << " node : " << nbNodes_ << "\n";
      std::cout << colOder_[c] << " score : " << score << "\n\n";
      if(score > maxScore) { maxScore = score; maxMove = colOder_[c]; alpha = score; }
      if(alpha >= beta) return maxMove;
    }
  }
  return maxMove;
}

int Solver::max(Grid &g, int alpha, int beta) {
  nbNodes_++;
  int ev = evaluate(g);
  for(int i = 0; i < W; i++) {
    if(g.check(i, 4)) {
      return ev;
    }
  }

  if(g.isFull())
    return 0;

  int maxScore = -1000;

  for(int c = 0; c < W; c++) {
    if(!g.isColumnFull(colOder_[c])) {
      Grid g2 = g;
      g2.putToken(colOder_[c], 0_b);
      int score = min(g2, alpha, beta);

      if(score > maxScore) { maxScore = score; alpha = score; }
      if(alpha >= beta) return maxScore;
    }
  }
  return maxScore;
}

int Solver::min(Grid &g, int alpha, int beta) {
  nbNodes_++;
  int ev = evaluate(g);
  for(int i = 0; i < W; i++) {
    if(g.check(i, 4)) {
      return ev;
    }
  }

  if(g.isFull())
    return 0;

  int minScore = 1000;

  for(int c = 0; c < W; c++) {
    if(!g.isColumnFull(colOder_[c])) {
      Grid g2 = g;
      g2.putToken(colOder_[c], 1_b);
      int score = max(g2, alpha, beta);
      if(score < minScore) { minScore = score; beta = score; }
      if(alpha >= beta) return minScore;
    }
  }
  return minScore;
}

int Solver::getBestChild(std::vector<int>& v) {
  int score = v[0], move = 0;
  for(int i = 1; i < v.size(); i++) {
    if(v[i] > score) {
      score = v[i];
      move = i;
    }
  }
  return move;
}

int Solver::minFirst(Grid &g, int alpha, int beta) {
  int minScore = 1000, minMove = -1;
  for(int c = 0; c < W; c++) {
    nbNodes_ = 0;
    if(!g.isColumnFull(colOder_[c])) {
      Grid g2 = g;
      g2.putToken(colOder_[c], 1_b);
      int score = max(g2, alpha, beta);
      std::cout << colOder_[c] << " node : " << nbNodes_ << "\n";
      std::cout << colOder_[c] << " score : " << score << "\n\n";
      if(score < minScore) { minScore = score; minMove = colOder_[c]; beta = score; }
      if(alpha >= beta) return minMove;
    }
  }
  return minMove;
}