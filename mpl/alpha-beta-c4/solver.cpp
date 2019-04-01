#include <iostream>
#include <vector>
#include <cstddef>
#include <future>
#include <thread>
#include "solver.hpp"

Solver::Solver() {
  for(int i = 0; i < W; i++) {
      colOrder_[i] = W/2 + (1 - 2 * (i % 2)) * (i + 1)/2;
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
  if(!g.isColumnFull(colOrder_[c])) {
    nbNodes_ = 0;
    Grid g2 = g;
    g2.putToken(colOrder_[c], 0_b);
    score = min(g2, alpha, beta);
    std::cout << colOrder_[c] << " node : " << nbNodes_ << "\n";
    std::cout << colOrder_[c] << " score : " << score << "\n\n";
  }
  return score;
}


/* int Solver::gtMaxChild(Grid &g, GameTree &nodes) {
  int score = -1000;
  if(!g.isColumnFull(colOrder_[c])) {
    nodes.nbNodes_++;
    Grid g2 = g;
    g2.putToken(colOrder_[c], 0_b);
    score = gtMin(g2, nodes);
    std::cout << colOrder_[c] << " node : " << nbNodes_ << "\n";
    std::cout << colOrder_[c] << " score : " << score << "\n\n";
  }
  return score;
} */

int Solver::maxNodes(Grid &g, int alpha, int beta) {
  int maxScore = -1000, maxMove = -1;
  for(int c = 0; c < W; c++) {
    nbNodes_ = 0;
    int score = maxChild(g, c, alpha, beta);
    if(score > maxScore) { maxScore = score; maxMove = colOrder_[c]; alpha = score; }
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
  return colOrder_[getBestChild(results)];
}

int Solver::maxFirst(Grid &g, int alpha, int beta) {
  int maxScore = -1000, maxMove = -1;
  for(int c = 0; c < W; c++) {
    nbNodes_ = 0;
    if(!g.isColumnFull(colOrder_[c])) {
      Grid g2 = g;
      g2.putToken(colOrder_[c], 0_b);
      int score = min(g2, alpha, beta);
      std::cout << colOrder_[c] << " node : " << nbNodes_ << "\n";
      std::cout << colOrder_[c] << " score : " << score << "\n\n";
      if(score > maxScore) { maxScore = score; maxMove = colOrder_[c]; alpha = score; }
      if(alpha >= beta) return maxMove;
    }
  }
  return maxMove;
}

int Solver::gtBestMove(Grid &g, int max) {
  GameTree node = gameTree(0, -1000, 1000);
  int score;
  if(max) score = gtMax(g, node);
  else score = gtMin(g, node);
  std::cout << "bestMove:" << node.bestMove_ << "\n";
  std::cout << "Score:" << score << "\n\n";
  std::cout << "NodeCount:" << node.nbNodes_ << "\n\n";
  return node.bestMove_;
}

int Solver::gtMin(Grid &g, GameTree &node) {
  node.nbNodes_++;
  int ev = evaluate(g);
  for(int i = 0; i < W; i++) {
    if(g.check(i, 4)) {
      // node.bestMove_ = i;
      return ev;
    }
  }

  if(g.isFull())
    return 0;

  int minScore = 1000;
  for(int c = 0; c < W; c++) {
    if(!g.isColumnFull(colOrder_[c])) {
      Grid g2 = g;
      GameTree child = gameTree(node.nbNodes_, node.alpha_, node.beta_);
      g2.putToken(colOrder_[c], 1_b);
      int score = gtMax(g2, child);
      if(score < minScore) { minScore = score; node.beta_ = score; node.bestMove_ = colOrder_[c]; }
      if(node.alpha_ >= node.beta_) return minScore;
    }
  }

  // std::cout << "bestMove:" << node.bestMove_ << "\n";
  return minScore;
}

int Solver::gtMax(Grid &g, GameTree &node) {
  node.nbNodes_++;
  int ev = evaluate(g);
  for(int i = 0; i < W; i++) {
    if(g.check(i, 4)) {
      // node.bestMove_ = i;
      return ev;
    }
  }

  if(g.isFull())
    return 0;

  int maxScore = -1000;
  for(int c = 0; c < W; c++) {
    if(!g.isColumnFull(colOrder_[c])) {
      Grid g2 = g;
      GameTree child = gameTree(node.nbNodes_, node.alpha_, node.beta_);
      g2.putToken(colOrder_[c], 0_b);
      int score = gtMin(g2, child);
      // std::cout << "bestMove : " << node.bestMove_ << "\n";
      if(score > maxScore) { maxScore = score; node.alpha_ = score; node.bestMove_ = colOrder_[c]; }
      if(node.alpha_ >= node.beta_) return maxScore;
    }
  }
  return maxScore;
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
    if(!g.isColumnFull(colOrder_[c])) {
      Grid g2 = g;
      g2.putToken(colOrder_[c], 0_b);
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
    if(!g.isColumnFull(colOrder_[c])) {
      Grid g2 = g;
      g2.putToken(colOrder_[c], 1_b);
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
    if(!g.isColumnFull(colOrder_[c])) {
      Grid g2 = g;
      g2.putToken(colOrder_[c], 1_b);
      int score = max(g2, alpha, beta);
      std::cout << colOrder_[c] << " node : " << nbNodes_ << "\n";
      std::cout << colOrder_[c] << " score : " << score << "\n\n";
      if(score < minScore) { minScore = score; minMove = colOrder_[c]; beta = score; }
      if(alpha >= beta) return minMove;
    }
  }
  return minMove;
}