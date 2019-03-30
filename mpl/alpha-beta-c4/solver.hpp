#ifndef SOLVER_H_
#define SOLVER_H_

#include "grid.hpp"

class Solver {
public:
  Solver();
  void printNodeScore(std::vector<int>& v);
  int negamax(Grid &g, std::byte cur_p, std::byte opp_p, int alpha, int beta);
  int neg(Grid &g, std::byte cur_p, int alpha, int beta);
  int getBestMove(Grid &g);
  int getScoreFromCol(Grid &g, int col);
  int getBestMoveFromNode(std::vector<int>& v, int m);
  int computerMove(Grid &g, std::byte cur_p, std::byte opp_p);
  int evaluate(Grid &g);
  int max_first(Grid &g, int alpha, int beta);
  int min_first(Grid &g, int alpha, int beta);
  int max(Grid &g, int alpha, int beta);
  int min(Grid &g, int alpha, int beta);
  int aiMove(Grid &g, bool maxMove);
private:
	int bestMove_;
	int nbNodes_;

};

#endif
