#include <iostream>
#include <cstddef>
#include "grid.hpp"

Grid::Grid() {
}

void Grid::initGrid() {
  for(int i = 0; i < W; ++i) {
    for(int j = 0; j < H; ++j) {
      grid_[i][j] = 3_b;
    }
  }
  for (int i = 0; i < W; i++)
    height_[i] = (H + 1) * i;
}

void Grid::printGrid() {
  for(int i = H-1; i >= 0; --i) {
    for(int j = 0; j < W; ++j) {
      switch(int(grid_[j][i])) {
        case 0:
          std::cout << " X ";
          break;
        case 1:
          std::cout << " O ";
          break;
        default:
          std::cout << " . ";
          break;
      }
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

bool Grid::isFull() {
  for(int i = 0; i < W; i++) {
    if(!isColumnFull(i))
      return false;
  }
  return true;
}

bool Grid::isColumnFull(int column) {
  if(rank_[column] < H)
    return false;
  return true;
}

void Grid::putToken(int column, std::byte value) {
  // assert
  if(rank_[column] < H) {
    grid_[column][rank_[column]] = value;
    rank_[column]++;
    // nbToken_++;
    lastToken_ = column;
  }
}

std::byte Grid::getToken(int column) {
  return grid_[column][std::max(rank_[column] - 1, 0)];
}

int Grid::getNbToken() {
  return nbToken_;
}

bool Grid::check(int column, int talign) {
  int rank = std::max(rank_[column] - 1, 0);
  std::byte value = grid_[column][rank];
  if(value == 3_b) return false;
  
  // Horizontal droite
  int inc = 1;
  int align = 1;
  while(inc + column < W) {
    if(grid_[column + inc][rank] != value)
      break;
    align++;
    inc++;
  }

  // Horizontal gauche 
  inc = 1;
  while(column - inc >= 0) {
    if(grid_[column - inc][rank] != value)
      break;
    align++;
    inc++;
  }

  if(align >= talign)
    return true;


  // Vertical haut 
  inc = 1;
  align = 1;
  while(rank + inc < H) {
    if(grid_[column][rank + inc] != value)
      break;
    align++;
    inc++;
  }

  // Vertical bas
  inc = 1;
  while(rank - inc >= 0) {
    if(grid_[column][rank - inc] != value)
      break;
    align++;
    inc++;
  }

  if(align >= talign) 
    return true;


 // Diagonale haut droite
  inc = 1;
  align = 1;
  while(rank + inc < W && column + inc < H) {
    if(grid_[column + inc][rank + inc] != value)
      break;
    align++;
    inc++;
  }

  // Diagonale bas gauche 
  inc = 1;
  while(rank - inc >= 0 && column - inc >= 0) {
    if(grid_[column - inc][rank - inc] != value)
      break;
    align++;
    inc++;
  }

  if(align >= talign)
    return true;



  // Diagonale haut gauche
  inc = 1;
  align = 1;
  while(column - inc >= 0 && rank + inc < H) {
    if(grid_[column - inc][rank + inc] != value)
      break;
    align++;
    inc++;
  }

  // Diagonale bas droite
  inc = 1;
  while(column + inc < W && rank - inc >= 0) {
    if(grid_[column + inc][rank - inc] != value)
      break;
    align++;
    inc++;
  }

  if(align >= talign)
    return true;

  return false;
}


bool Grid::bbIsFull() {
  return nbToken_ == W * H;
}

/* void Grid::bbPutToken(int column) {
  std::cout << "toto\n";
  tokens_[turn_ & 1] ^= 1ULL << height_[column]++;
  nbToken_++;
  turn_++;
} */

void Grid::bbPutToken(int column, int turn) {
  tokens_[turn] ^= 1ULL << height_[column]++;
  nbToken_++;
}


/*bool Grid::bbIsColumnFull(int column) {
  std::cout << "toto\n";
  uint64_t top = ((1ULL << (H + 1) * W) - 1ULL) / ((1ULL << (H + 1)) - 1ULL) << H;
  uint64_t t = tokens_[turn_ & 1] | (1ULL << height_[column]);
  return (t & top) != 0;
} */


bool Grid::bbIsColumnFull(int column, int turn) {
  uint64_t top = ((1ULL << (H + 1) * W) - 1ULL) / ((1ULL << (H + 1)) - 1ULL) << H;
  uint64_t t0 = tokens_[turn] | (1ULL << height_[column]);
  uint64_t t1 = tokens_[!turn] | (1ULL << height_[column]);
  return ((t0 & top) != 0) || ((t1 & top) != 0);
}

/* bool Grid::bbCheckNextMove(int column) {
  std::cout << "toto\n";
  uint64_t t = tokens_[turn_ & 1];
  t ^= 1ULL << height_[column]++;
  return checkAlignment(t);
} */

bool Grid::bbCheckNextMove(int column, int turn) {
  uint64_t t = tokens_[turn];
  t ^= 1ULL << height_[column];
  return checkAlignment(t);
}

/* bool Grid::bbCheck() {
  std::cout << "toto\n";
  uint64_t t = tokens_[turn_];
  return checkAlignment(t);
} */

bool Grid::bbCheck(int turn) {
  uint64_t t = tokens_[turn];
  return checkAlignment(t);
}


bool Grid::checkAlignment(uint64_t position) {
  // horizontale 
  uint64_t m = position & (position >> (H+1));
  if(m & (m >> (2*(H+1)))) {
    // std::cout << "horizontal work\n";
    return true;
  }

  // verticale
  m = position & (position >> 1);
  if(m & (m >> 2)) {
    // std::cout << "vertical work\n";
    return true;
  }

  // diagonale (\)
  m = position & (position >> H);
  if(m & (m >> (2*H))) {
    // std::cout << "diagonal1 work\n";
    return true;
  }

  // diagonale (/)
  m = position & (position >> (H+2));
  if(m & (m >> (2*(H+2)))) {
    // std::cout << "diagonal2 work\n";
    return true;
  }

  return false;
}

uint64_t Grid::getKey() { 
  uint64_t mask = tokens_[0] | tokens_[1];
  return tokens_[turn_ & 1] + mask;
}

uint64_t Grid::getKey(int turn) { 
  uint64_t mask = tokens_[0] | tokens_[1];
  return tokens_[turn] + mask;
}