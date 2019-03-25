#ifndef SOLVER_H_
#define SOLVER_H_

#include "grid.hpp"
#include <algorithm>

class Solver {
public:
  Solver(int maxDepth);
  int evaluate(Grid &g, std::byte cur_p, std::byte opp_p, int depth);
  std::pair<int, int> negamax(Grid &g, int col, std::byte cur_p, std::byte opp_p, int alpha, int beta, int depth);
  int getBestMove(Grid &g, std::byte cur_p, std::byte opp_p);
  int getMoveFromCol(Grid &g, int col, std::byte cur_p, std::byte opp_p);
  int getBestMoveFromNode(std::vector<std::pair<int, int>>& v);

private:
  int depth_;
};

#endif
