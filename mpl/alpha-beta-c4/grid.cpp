#include "grid.hpp"
#include <iostream>
#include <cstddef>

std::byte operator "" _b(unsigned long long i){
  return std::byte(i);
} 

Grid::Grid() {
  countToken_ = 0;
}

void Grid::initGrid() {
  for(int i = 0; i < W; ++i) {
    for(int j = 0; j < H; ++j) {
      grid_[i][j] = 3_b;
    }
  }
}

void Grid::printGrid() {
  for(int i = W - 1; i >= 0; --i) {
    for(int j = 0; j < H; ++j) {
      std::cout << int(grid_[i][j]) << " ";
    }
    std::cout << "\n";
  }
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

bool Grid::putToken(int column, std::byte value) {
  if(rank_[column] < W) {
    grid_[rank_[column]][column] = value;
    rank_[column]++;
    countToken_++;
  }
  if(check(column, value))
    return true;

  return false;
}

bool Grid::check(int column, std::byte value) {
  // Horizontal droite
  int inc = 1;
  int align = 1;
  while(inc + column < W && align < 4) {
    if(grid_[rank_[column]][column + inc] != value)
      break;
    align++;
    inc++;
  }
  // Horizontal gauche 
  inc = 1;
  while(column - inc >= 0 && align < 4) {
    if(grid_[rank_[column]][column - inc] != value)
      break;
    align++;
    inc++;
  }

  if(align >= 4)
    return true;



  // Vertical haut 
  inc = 1;
  align = 1;
  while(rank_[column] + inc < H && align < 4) {
    if(grid_[rank_[column] + inc][column] != value)
      break;
    align++;
    inc++;
  }

  // Vertical bas
  inc = 1;
  while(rank_[column] - inc >= 0 && align < 4) {
    if(grid_[rank_[column] - inc][column] != value)
      break;
    align++;
    inc++;
  }

  if(align >= 4) 
    return true;



 // Diagonale haut droite
  inc = 1;
  align = 1;
  while(rank_[column] + inc < W && column + inc < H && align < 4) {
    if(grid_[rank_[column] + inc][column + inc] != value)
      break;
    align++;
    inc++;
  }

  // Diagonale bas gauche 
  inc = 1;
  while(rank_[column] - inc >= 0 && column - inc >= 0 && align < 4) {
    if(grid_[rank_[column] - inc][column - inc] != value)
      break;
    align++;
    inc++;
  }

  if(align >= 4)
    return true;



  // Diagonale haut gauche
  inc = 1;
  align = 1;
  while(column - inc >= 0 && rank_[column] + inc < H && align < 4) {
    if(grid_[rank_[column] + inc][column - inc] != value)
      break;
    align++;
    inc++;
  }

  // Diagonale bas droite
  inc = 1;
  while(column + inc < W && rank_[column] - inc >= 0 && align < 4) {
    if(grid_[rank_[column] - inc][column + inc] != value)
      break;
    align++;
    inc++;
  }

  if(align >= 4)
    return true;

  return false;
} 
