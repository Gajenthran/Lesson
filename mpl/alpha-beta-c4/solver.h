#ifndef SOLVER_H_
#define SOLVER_H_

#include <algorithm>

class Solver {
public:
  Solver(int maxDepth);
  int evaluate(Grid &g, std::byte cur_p, std::byte opp_p);
  std::pair<int, int> negamax(Grid &g, std::byte cur_p, std::byte opp_p, int alpha, int beta, int depth);

private:
  int depth_;
}

#endif
