#ifndef SOLVER_H_
#define SOLVER_H_

#include "grid.hpp"

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
  };

  class GameTree {
  public:
    int nbNodes_;
    int alpha_;
    int beta_;
    int bestMove_;  
  };


  GameNodes * createGameNodes(Grid g, int turn) {
    GameNodes * gt = new GameNodes[1];
    gt->g = g;
    gt->bestMove = -1;
    gt->turn = turn;
    gt->alpha = -1000;
    gt->beta = 1000;
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

  int gnMin(GameNodes * gn);
  int gnMax(GameNodes * gn);
  int gnEvaluate(GameNodes * gn);
  int gnBestMove(Grid &g);

  int evaluate(Grid &g);
  int maxChild(Grid &g, int col, int alpha, int beta);
  int maxFirst(Grid &g, int alpha, int beta);
  int minFirst(Grid &g, int alpha, int beta);
  int max(Grid &g, int alpha, int beta);
  int min(Grid &g, int alpha, int beta);
  int maxNodes(Grid &g, int alpha, int beta);
  int getBestChild(std::vector<int>& v);
  int computerMove(Grid &g, int thread);

private:
  int nbNodes_;
	int colOrder_[W];
};

#endif
