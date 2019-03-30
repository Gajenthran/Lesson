#include <iostream>
#include <vector>
#include <cstddef>
#include <future>
#include <thread>
#include "solver.hpp"

Solver::Solver() {

}

std::byte operator "" _c(unsigned long long i){
  return std::byte(i);
} 

// MAX : IA : 0_c : X :: 1000
// MIN : J1 : 1_c : O :: -1000
int Solver::aiMove(Grid &g, bool maxMove) {
  bestMove_ = -1;
  int best = maxMove ?  max_first(g, -1000, 1000) : min_first(g, -1000, 1000);
  // std::cout << "best : " << best << "\n";
  return best;
}

int Solver::evaluate(Grid &g) {
  std::byte t;
  for(int i = 0; i < W; i++) {
    t = g.getToken(i, 0);
    if(t != 3_c) {
      if(g.check(i, 4)) {
        if(t == 0_c) {
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

int Solver::max_first(Grid &g, int alpha, int beta) {
  int maxScore = -1000, maxMove = -1;
  for(int c = 0; c < W; c++) {
    nbNodes_ = 0;
    if(!g.isColumnFull(c)) {
      Grid g2 = g;
      g2.putToken(c, 0_c);
      // g2.printGrid();
      int score = min(g2, alpha, beta);
      std::cout << c << " node : " << nbNodes_ << "\n";
      std::cout << c << " score : " << score << "\n\n";
      if(score > maxScore) { maxScore = score; maxMove = c; alpha = score; }
      if(alpha >= beta) return maxMove;
    }
  }
  return maxMove;
}

int Solver::min_first(Grid &g, int alpha, int beta) {
  int minScore = 1000, minMove = -1;
  for(int c = 0; c < W; c++) {
    nbNodes_ = 0;
    if(!g.isColumnFull(c)) {
      Grid g2 = g;
      g2.putToken(c, 1_c);
      // g2.printGrid();
      int score = max(g2, alpha, beta);
      // std::cout << c << " node : " << nbNodes_ << "\n";
      // std::cout << c << " score : " << score << "\n\n";
      if(score < minScore) { minScore = score; minMove = c; beta = score; }
      if(alpha >= beta) return minMove;
    }
  }
  return minMove;
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
    if(!g.isColumnFull(c)) {
      Grid g2 = g;
      g2.putToken(c, 0_c);
      int score = min(g2, alpha, beta);

      if(bestMove_ == -1 || score > maxScore) { maxScore = score; bestMove_ = c; alpha = score; }
      // if(alpha >= beta) return maxScore;
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
    if(!g.isColumnFull(c)) {
      Grid g2 = g;
      g2.putToken(c, 1_c);
      int score = max(g2, alpha, beta);

      if(bestMove_ == -1 || score < minScore) { minScore = score; bestMove_ = c; beta = score; }
      // if(alpha >= beta) return minScore;
    }
  }
  return minScore;
}

/* int Solver::getBestMove(Grid &g) {
  std::vector<int> nodes;
  int n;
  for(int i = 0; i < W; i++) {
    n = getScoreFromCol(g, i);
    nodes.push_back(n);
  }

  printNodeScore(nodes);
  return getBestMoveFromNode(nodes, false);
}

int Solver::getScoreFromCol(Grid &g, int col) {
  // Grid g2 = g;
  // g2.putToken(col, 0_c);
  int n = max(g, -1, 1); // -negamax(g2, opp_p, cur_p, -1000, 1000);
  return n;
}

int Solver::getBestMoveFromNode(std::vector<int>& v, int m) {
  int score = v[0], move = 0;
  for(int i = 1; i < v.size(); i++) {
    if(m) {
      if(v[i] > score) {
        score = v[i];
        move = i;
      }
    }

    else {
      if(v[i] < score) {
        score = v[i];
        move = i;
      }
    }
  }
  return move;
}

void Solver::printNodeScore(std::vector<int>& v) {
  for(int i = 0; i < v.size(); i++) {
    std::cout << i << ": " << v[i] << "\n";
  }
  std::cout << "\n";
} */

/* 
int Solver::negamax(Grid &g, std::byte cur_p, std::byte opp_p, int alpha, int beta) {
  int ev = evaluate(g, cur_p, opp_p);
  if(abs(ev) >= 1000)
    return ev;

  if(g.isFull())
    return 0;
  
  int bestScore = -1000;
  for(int i = 0; i < W; i++) {
    if(!g.isColumnFull(i)) {
      Grid g2 = g;
      g2.putToken(i, cur_p);
      int score = -negamax(g2, opp_p, cur_p, -beta, -alpha);

      if(score > bestScore) bestScore = score;
      if(score > alpha) alpha = score; 
      if(alpha >= beta) { bestScore = score; break; }
    }
  }
  return bestScore;
}


int Solver::computerMove(Grid &g, std::byte cur_p, std::byte opp_p) {
  std::vector<std::future<int>> future_results;
  for(int i = 0; i < W; i++) 
    future_results.push_back(std::async(&Solver::getScoreFromCol, this, std::ref(g), i, cur_p, opp_p));

  std::vector <int> results;
  std::transform(begin(future_results), end(future_results), back_inserter(results), [](auto& x) { return x.get(); });
  printNodeScore(results);
  return getBestMoveFromNode(results);
} */
