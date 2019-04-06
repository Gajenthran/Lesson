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

  bool bbCheckNextMove(int column, int turn);
  void bbPutToken(int column);
  void bbPutToken(int column, int turn);
  bool bbIsColumnFull(int column);
  bool bbIsColumnFull(int column, int turn);
  bool bbCheckNextMove(int column);
  bool bbIsFull();
  bool bbCheck();
  bool bbCheck(int turn);
  bool checkAlignment(uint64_t position);
  uint64_t getKey();
  uint64_t getKey(int turn);
  
private:
  std::byte grid_[W][H];
  int rank_[W] = {0};
  int nbToken_ = 0;
  int lastToken_;

  uint64_t tokens_[2] = {0};
  int height_[W] = {0}; // to uchar
  int turn_ = 1;

};

#endif
