#ifndef GRID_H_
#define GRID_H_

#include <cstddef>


inline std::byte operator "" _b(unsigned long long i){
  return std::byte(i);
} 

class Grid {
public:
  Grid();

  void initGrid();
  void putToken(int column, std::byte value);
  std::byte getToken(int column);
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


