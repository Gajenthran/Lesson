#ifndef GRID_H_
#define GRID_H_

#include <cstddef>

class Grid {
public:
  Grid();

  void initGrid();
  void putToken(int column, std::byte value);
  void removeToken(int column, std::byte);
  std::byte getToken(int column, int rank);
  bool isFull();
  bool isColumnFull(int column);
  bool check(int column, int talign);
  int getNbToken();
  void printGrid();
private:
  std::byte grid_[W][H];
  int rank_[W] = {0};
  int nbToken_ = 0;
};

#endif


