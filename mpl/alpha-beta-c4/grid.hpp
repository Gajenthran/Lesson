#ifndef GRID_H_
#define GRID_H_


#include <cstddef>

class Grid {
public:
  Grid(int width, int height);

  void initGrid();
  bool putToken(int column, std::byte value);
  bool isFull();
  bool check(int column, int rank, std::byte value);
  void printGrid();
private:
  int w_;
  int h_;
  std::byte grid_[W][H];
};

#endif
/* 
initGame()
reduce()
playout()
move(npl)
eval() */


