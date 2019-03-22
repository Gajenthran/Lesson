#ifndef GRID_H_
#define GRID_H_

#include <cstddef>

class Grid {
public:
  Grid();

  void initGrid();
  int  getCountToken() { return countToken_; };
  bool putToken(int column, std::byte value);
  bool isFull();
  bool isColumnFull(int column);
  bool check(int column, std::byte value);
  void printGrid();
private:
  std::byte grid_[W][H];
  int rank_[W] = {0};
  int countToken_;
};

#endif


