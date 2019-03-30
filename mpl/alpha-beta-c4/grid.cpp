#include <iostream>
#include <cstddef>
#include "grid.hpp"


std::byte operator "" _b(unsigned long long i){
  return std::byte(i);
} 

Grid::Grid() {
}

void Grid::initGrid() {
  for(int i = 0; i < W; ++i) {
    for(int j = 0; j < H; ++j) {
      grid_[i][j] = 3_b;
    }
  }
}

void Grid::printGrid() {
  for(int i = H-1; i >= 0; --i) {
    for(int j = 0; j < W; ++j) {
      // std::cout << int(grid_[j][i]) << " ";
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
  if(rank_[column] < H) {
    grid_[column][rank_[column]] = value;
    rank_[column]++;
    nbToken_++;
  }
}

std::byte Grid::getToken(int column, int rank) {
  return grid_[column][std::max(rank_[column] - 1, 0)];
}


void Grid::removeToken(int column, std::byte) {
  rank_[column]--;
  grid_[column][rank_[column]] = 3_b;
  nbToken_--;
}

int Grid::getNbToken() {
  int nb = 0;
  for(int i = 0; i < W; i++) {
    for(int j = 0; j < H; j++) {
      if(grid_[i][j] != 3_b)
        nb++;
    }
  }
  return nb;
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