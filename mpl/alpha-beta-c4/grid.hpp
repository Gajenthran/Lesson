#ifndef GRID_H_
#define GRID_H_

#include <cstddef>
#include <vector>

class Grid {
public:
  Grid();

  void initGrid();
  int  getCountToken() { return countToken_; };
  void putToken(int column, std::byte value);
  void removeToken(int column, std::byte);
  std::byte getToken(int column, int rank);
  bool isFull();
  void nextMoves();
  int getNMSize();
  int getNextMove(int column);
  bool isColumnFull(int column);
  bool check(int column, std::byte value, int talign);
  void printGrid();
private:
  std::byte grid_[W][H];
  int rank_[W] = {0};
  std::vector<int> nextMoves_;
  int countToken_;
};

#endif


