#include "grid.hpp"
#include <iostream>
#include <cstddef>

std::byte operator "" _b(unsigned long long i){
  return std::byte(i);
} 

Grid::Grid(int width, int height) {
  w_ = width;
  h_ = height;
}

void Grid::initGrid() {
  for(int i = 0; i < w_; ++i) {
    for(int j = 0; j < h_; ++j) {
      grid_[i][j] = 3_b;
    }
  }
}

void Grid::printGrid() {
  for(int i = w_ - 1; i >= 0; --i) {
    for(int j = 0; j < h_; ++j) {
      std::cout << int(grid_[i][j]) << " ";
    }
    std::cout << "\n";
  }
}

bool Grid::isFull() {
  for(int i = 0; i < W; i++) {
      if(grid_[i][H-1] == 3_b)
        return false;
  }
  return true;
}

bool Grid::putToken(int column, std::byte value) {
  int posW = 0;
  while(grid_[posW][column] != 3_b && posW < W)
    ++posW;

  grid_[posW][column] = value;

  if(check(column, posW, value))
    return true;

  return false;
}

bool Grid::check(int column, int rank, std::byte value) {
  // Horizontal droite
  int inc = 1;
  int align = 1;
  while(inc + column < w_ && align < 4) {
    if(grid_[rank][column + inc] != value)
      break;
    align++;
    inc++;
  }
  // Horizontal gauche 
  inc = 1;
  while(column - inc >= 0 && align < 4) {
    if(grid_[rank][column - inc] != value)
      break;
    align++;
    inc++;
  }

  if(align >= 4)
    return true;

  // Vertical haut 
  inc = 1;
  align = 1;
  while(rank + inc < h_ && align < 4) {
    if(grid_[rank + inc][column] != value)
      break;
    align++;
    inc++;
  }

  // Vertical bas
  inc = 1;
  while(rank - inc >= 0 && align < 4) {
    if(grid_[rank - inc][column] != value)
      break;
    align++;
    inc++;
  }

  if(align >= 4) 
    return true;

 /* // Diagonale haut droite
  inc = 1;
  while(rank + inc < w_ && column + inc < h_ && align < 4) {
    if(grid_[rank + inc][column + inc] != value)
      break;
    align++;
    inc++;
  }

  // Diagonale bas gauche 
  inc = 1;
  while(rank - inc >= 0 && column - inc >= 0 && align < 4) {
    if(grid_[rank - inc][column - inc] != value)
      break;
    align++;
    inc++;
  }

  if(align >= 4)
    return true;


  // Diagonale haut gauche
  inc = 1;
  while(column - inc >= 0 && rank + inc < h_ && align < 4) {
    if(grid_[rank + inc][column - inc] != value)
      break;
    align++;
    inc++;
  }

  // Diagonale bas droite
  inc = 1;
  while(column + inc < w_ && rank - inc >= 0 && align < 4) {
    if(grid_[rank - inc][column + inc] != value)
      break;
    align++;
    inc++;
  }

  if(align >= 4)
    return true; */
  return false;
} 
