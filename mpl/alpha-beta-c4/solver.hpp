#ifndef SOLVER_H_
#define SOLVER_H_

#include "grid.hpp"
#include <algorithm>

class Solver {
public:
  Solver();
  int evaluate(Grid &g, std::byte cur_p, std::byte opp_p);
  void printNodeScore(std::vector<int>& v);
  int negamax(Grid &g, int col, std::byte cur_p, std::byte opp_p, int alpha, int beta);
  int getBestMove(Grid &g, std::byte cur_p, std::byte opp_p);
  int getScoreFromCol(Grid &g, int col, std::byte cur_p, std::byte opp_p);
  int getBestMoveFromNode(std::vector<int>& v);
  int computerMove(Grid &g, std::byte cur_p, std::byte opp_p);

private:
};

#endif
