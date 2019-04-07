#include <iostream>
#include <vector>
#include <cstddef>
#include <future>
#include <thread>
#include <unordered_map>
#include "solver.hpp"

Solver::Solver() {
  for(int i = 0; i < W; i++) {
      colOrder_[i] = W/2 + (1 - 2 * (i % 2)) * (i + 1)/2;
  }
}

/* int Solver::neg(GameNodes * gn) {
  if(gn->g.bbIsFull())
    return 0;

  int ev = bbEvaluate(gn->g, gn->turn);
  if(ev != 0)
    return ((W * H + 1 - gn->g.getNbToken())/2) * (gn->turn == 1 ? 1 : -1);

  int bestScore = -1000;
  for(int c = 0; c < W; c++) {
    if(!gn->g.bbIsColumnFull(c, gn->turn)) {
      Grid g2 = gn->g;
      g2.bbPutToken(c, gn->turn);
      gn->children[c] = createGameNodes(g2, -gn->beta, -gn->alpha, !gn->turn, gn->tt);
      int score = -neg(gn->children[c]); 

      if(score > bestScore)
        bestScore = score;
    }
  }
  return bestScore;
} */


int Solver::computerBM(Grid &g, int turn) {
  std::unordered_map<uint64_t, int> ttable;
  GameNodes * gn = createGameNodes(g, -1000, 1000, turn, ttable);

  // std::vector<std::future<int>> future_results;
  for(int c = 0; c < W; c++) {
    if(!g.bbIsColumnFull(c, 1))
      std::async(&Solver::gnChildMove, this, std::ref(gn), c, turn);
  }

  // std::vector<int> results;
  // std::transform(begin(future_results), end(future_results), back_inserter(results), [](auto& x) { return x.get(); });

  gn->bestMove = -1; gn->bestScore = -1000;
  for(int i = 0; i < W; i++) {
    if(gn->children[i] != nullptr) {
      if(gn->bestScore < gn->children[i]->bestScore) {
        gn->bestMove = i;
        gn->bestScore = gn->children[i]->bestScore;
      }
    }
  }
  std::cout << "bestScore   : " << gn->bestScore << "\n";
  std::cout << "bestMove    : " << gn->bestMove << "\n";
  return gn->bestMove;
}

int Solver::neg(GameNodes * gn) {
  nbNodes_++;
  if(gn->g.bbIsFull())
    return 0;

  for(int c = 0; c < W; c++) {
    if(!gn->g.bbIsColumnFull(c, gn->turn) && gn->g.bbCheckNextMove(c, gn->turn)) {
      return (W * H + 1 - gn->g.getNbToken())/2;
    }
  }

  int bestScore = (W * H - 1 - gn->g.getNbToken())/2;
  auto key = tt_.find(gn->g.getKey(gn->turn));
  if(key != tt_.end()) {
    std::cout << "entre\n";
    bestScore = key->second + MIN_SCORE - 1;// key->second; //- (W * H)/2 - 1; 
  }

  if(gn->beta > bestScore) {
    gn->beta = bestScore;
    if(gn->alpha >= gn->beta) return gn->beta;
  }

  for(int c = 0; c < W; c++) {
    if(!gn->g.bbIsColumnFull(colOrder_[c], gn->turn)) {
      Grid g2 = gn->g;
      g2.bbPutToken(colOrder_[c], gn->turn);
      gn->children[colOrder_[c]] = createGameNodes(g2, -gn->beta, -gn->alpha, !gn->turn);
      int score = -neg(gn->children[colOrder_[c]]);

      if(score >= gn->beta) { return score; }
      if(score > gn->alpha) { gn->alpha = score; }
    }
  }

  tt_[gn->g.getKey(gn->turn)] = gn->alpha - MIN_SCORE + 1; //  - (W * H)/2 + 1;
  return gn->alpha;
}

int Solver::gnBM(Grid &g, int turn) {
  std::unordered_map<uint64_t, int> ttable;
  GameNodes * gn = createGameNodes(g, -1000, 1000, 1, ttable);
  for(int c = 0; c < W; c++) {
    if(!g.bbIsColumnFull(c, 1))
      gnChildMove(gn, c, 0);
  }

  gn->bestMove = -1; gn->bestScore = -1000;
  for(int i = 0; i < W; i++) {
    if(gn->children[i] != nullptr) {
      if(gn->bestScore < gn->children[i]->bestScore) {
        gn->bestMove = i;
        gn->bestScore = gn->children[i]->bestScore;
      }
    }
  }
  std::cout << "bestScore   : " << gn->bestScore << "\n";
  std::cout << "bestMove    : " << gn->bestMove << "\n";
  return gn->bestMove;
}

void Solver::gnChildMove(GameNodes * gn, int col, int turn) {
  std::unordered_map<uint64_t, int> ttable;
  if(gn->g.bbCheckNextMove(col, turn)) {
    gn->children[col] = createGameNodes(gn->g, -1000, 1000, 0, ttable);
    gn->children[col]->bestScore = (W * H + 1 - gn->g.getNbToken())/2 * (turn ? 1 : -1);
  } else {
    Grid g2 = gn->g;
    g2.bbPutToken(col, turn);
    gn->children[col] = createGameNodes(g2, -1000, 1000, !turn, ttable);
    gn->children[col]->bestScore = neg(gn->children[col]) * (turn ? -1 : 1);
  }
  std::cout << gn->children[col]->bestScore << "\n";
}

int Solver::gnBestMove(Grid &g, int turn) {
  std::unordered_map<uint64_t, int> ttable;
  GameNodes * gn = createGameNodes(g, -1000, 1000, turn);
  int score;
  for(int i = 0; i < W; i++) {
    tt_.clear();
    if(!g.bbIsColumnFull(i, turn)) {
      nbNodes_ = 0;
        Grid g2 = g;
        g2.bbPutToken(i, turn);
        gn->children[i] = createGameNodes(g2, -1000, 1000, !turn, ttable);
        score = -neg(gn->children[i]);

      std::cout << i << " " << score << "\n";
      std::cout << "nbNodes_ : " << nbNodes_ << "\n\n";
      gn->children[i]->bestScore = score;
    }
  }

  gn->bestMove = -1; gn->bestScore = -1000;
  for(int i = 0; i < W; i++) {
    if(gn->children[i] != nullptr) {
      if(gn->bestScore < gn->children[i]->bestScore) {
        gn->bestMove = i;
        gn->bestScore = gn->children[i]->bestScore;
      }
    }
  }
  std::cout << "bestScore   : " << gn->bestScore << "\n";
  std::cout << "bestMove    : " << gn->bestMove << "\n";
  return gn->bestMove;
}


int Solver::evaluate(Grid &g, int turn) {
  if(turn) {
    if(g.bbCheck(turn))
      return 1000 - g.getNbToken();
  } else {
    if(g.bbCheck(turn))
      return -1000 + g.getNbToken();
  }
  return 0;
}


int Solver::negamax(GameNodes * gn) {
  if(gn->g.bbIsFull())
    return 0;

  int ev = evaluate(gn->g, gn->turn);
  if(ev != 0)
    return ev;

  int bestScore, score;
  auto key = gn->tt.find(gn->g.getKey(gn->turn));
  if(key != gn->tt.end()) {
    std::cout << "toto\n";
    bestScore = key->second;
  } else
    bestScore = 1000 * (gn->turn ? -1 : 1);

  for(int c = 0; c < W; c++) {
    if(!gn->g.bbIsColumnFull(c, !gn->turn)) {
        Grid g2 = gn->g;
        g2.bbPutToken(c, gn->turn);
        gn->children[c] = createGameNodes(g2, gn->alpha, gn->beta, !gn->turn, gn->tt);
        score = negamax(gn->children[c]);

      if(!gn->turn) {
        if(bestScore > score) {
          bestScore = gn->bestScore = gn->beta = score;
          gn->bestMove = c; // gn->children[c]->bestMove; //c;
        }
        if(score <= gn->alpha) return score;
      } else {
        if(bestScore < score) {
          bestScore = gn->bestScore = gn->alpha = score;
          gn->bestMove = c; // gn->children[c]->bestMove; //c;
        }
        if(score >= gn->beta) return score;
      }
    }    
  }
  gn->tt[gn->g.getKey(gn->turn)] = bestScore;
  return bestScore;
} 

/* // MAX : IA : 0_b : X ::  1000
// MIN : J1 : 1_b : O :: -1000
int Solver::gnEvaluate(GameNodes * gn) {
  std::byte t;
  for(int i = 0; i < W; i++) {
    t = gn->g.getToken(i);
    if(t != 3_b) {
      if(gn->g.check(i, 4)) {
        if(t == 0_b) {
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

int Solver::gnMin(GameNodes * gn) {
  int ev = evaluate(gn->g);
  for(int i = 0; i < W; i++) {
    if(gn->g.check(i, 4)) {
      return ev;
    }
  }

  if(gn->g.isFull()) {
    return 0;
  }

  int minScore = 1000;
  for(int c = 0; c < W; c++) {
    if(!gn->g.isColumnFull(c)) {
      Grid g2 = gn->g;
      g2.putToken(c, 1_b);
      gn->children[c] = createGameNodes(g2, gn->turn * -1);
      int score = gnMax(gn->children[c]);
      if(score < minScore) {
        gn->bestMove = c; // gn->children[c]->bestMove;
        gn->bestScore = score; // gn->children[c]->bestScore;
        minScore = score;
        gn->beta = score;
      }
      if(gn->alpha >= gn->beta) return minScore;
    }
  }
  return minScore;
}

int Solver::gnMax(GameNodes * gn) {
  int ev = evaluate(gn->g);
  for(int i = 0; i < W; i++) {
    if(gn->g.check(i, 4)) {
      return ev;
    }
  }

  if(gn->g.isFull()) {
    return 0;
  }

  int maxScore = -1000;
  for(int c = 0; c < W; c++) {
    if(!gn->g.isColumnFull(c)) {
      Grid g2 = gn->g;
      g2.putToken(c, 0_b);
      gn->children[c] = createGameNodes(g2, gn->turn * -1);
      int score = gnMin(gn->children[c]);
      if(score > maxScore) { 
        gn->bestMove = c;
        gn->bestScore = score;
        maxScore = score;
        gn->alpha = score;
      }
      if(gn->alpha >= gn->beta) return maxScore;
    }
  }
  return maxScore;
} */

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
  if(ev != 0)
    return ev;

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
  if(ev != 0)
    return ev;

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
  if(ev != 0)
    return ev;


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
  if(ev != 0)
    return ev;


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

int Solver::gnNegamax(GameNodes * gn) {
  if(gn->g.isFull())
    return 0;

  int ev = evaluate(gn->g);
  for(int c = 0; c < W; c++) {
    if(!gn->g.isColumnFull(c) && gn->g.check(c, 4)) {
      return 1000 - gn->g.getNbToken();
    }
  }

  int bestScore = 1000;
  for(int c = 0; c < W; c++) {
    if(!gn->g.isColumnFull(c)) {
      Grid g2 = gn->g;
      g2.putToken(c, gn->turn == 1 ? 0_b : 1_b);
      std::unordered_map<uint64_t, int> tt;
      gn->children[c] = createGameNodes(g2, -gn->beta, -gn->alpha, gn->turn * -1, tt);
      int score = -gnNegamax(gn->children[c]);

      if(score > bestScore) {
        bestScore = score;
        gn->bestMove = c; // gn->children[c]->bestMove;
        gn->bestScore = score; // gn->children[c]->bestScore;
      }
    }
  }
  return bestScore;
}