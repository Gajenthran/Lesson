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
  int lastMove() { return lastToken_; };
  void printGrid();

  void bbPutToken(uint64_t column);
  bool bbIsColumnFull(uint64_t column);
  bool bbCheck(uint64_t column);
  bool checkAlignment(int position);
  
private:
  std::byte grid_[W][H];
  int rank_[W] = {0};
  int nbToken_ = 0;
  int lastToken_;

  uint64_t p1 = {0};
  uint64_t p2 = {0};

  uint64_t pgrid_ = {0}; 
  uint64_t mask_ = {0};

};

#endif


