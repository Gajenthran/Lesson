#ifndef SOLVER_H_
#define SOLVER_H_

#include "grid.hpp"
#include <unordered_map>

class Solver {
public:
  Solver();

  struct GameNodes {
    Grid g;
    int bestMove;
    int bestScore;
    int turn;
    int alpha;
    int beta;
    GameNodes * children[W];
    std::unordered_map<uint64_t, int> tt;
  };

  class GameTree {
  public:
    int nbNodes_;
    int alpha_;
    int beta_;
    int bestMove_;  
  };


  GameNodes * createGameNodes(Grid g, int alp, int bet, int t, std::unordered_map<uint64_t, int> ttable) {
    GameNodes * gt = new GameNodes[1];
    gt->g = g;
    gt->bestMove = -1;
    gt->turn = t;
    gt->alpha = alp;
    gt->beta = bet;
    gt->tt = ttable;
    for(int i = 0; i < W; i++)
      gt->children[i] = nullptr;
    return gt;
  }

  GameNodes * createGameNodes(Grid g, int alp, int bet, int t) {
    GameNodes * gt = new GameNodes[1];
    gt->g = g;
    gt->bestMove = -1;
    gt->turn = t;
    gt->alpha = alp;
    gt->beta = bet;
    for(int i = 0; i < W; i++)
      gt->children[i] = nullptr;
    return gt;
  }

  auto gameTree(int nbNodes, int alpha, int beta) {
    return GameTree { nbNodes, alpha, beta, -1 };
  }

  int gtMax(Grid &g, GameTree &node);
  int gtMin(Grid &g, GameTree &node);
  int gtBestMove(Grid &g, int max);
  
  int negamax(GameNodes * gn);
  int gnNegamax(GameNodes * gn);
  int gnMin(GameNodes * gn);
  int gnMax(GameNodes * gn);
  int gnEvaluate(GameNodes * gn);
  int gnBestMove(Grid &g, int turn);
  int gnBM(Grid &g, int turn);
  int computerBM(Grid &g, int turn);
  void gnChildMove(GameNodes * gn, int col, int turn);
  int bbEvaluate(Grid &g, int turn);
  int neg(GameNodes * gn);
  int neg(GameNodes * gn, int alph, int bet);

  int evaluate(Grid &g);
  int evaluate(Grid &g, int turn);
  int maxChild(Grid &g, int col, int alpha, int beta);
  int maxFirst(Grid &g, int alpha, int beta);
  int minFirst(Grid &g, int alpha, int beta);
  int max(Grid &g, int alpha, int beta);
  int min(Grid &g, int alpha, int beta);
  int maxNodes(Grid &g, int alpha, int beta);
  int getBestChild(std::vector<int>& v);
  int computerMove(Grid &g, int thread);

private:
  int nbNodes_ = 0;
  std::unordered_map<uint64_t, int> tt_;
	int colOrder_[W];
  int MIN_SCORE = -(W*H)/2 + 3;
};

#endif
