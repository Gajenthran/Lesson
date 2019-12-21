// filtrage.cpp

#include <iostream>
#include <cmath>
#include "td2-gsimage.hpp"


void filtre(GSImage& image) {
  float filtered_row[image.w()];
  for(int r = 0; r < image.h(); ++r) {
    float* row = image.pixels() + r * image.w();
    for(int c = 1; c < image.w() - 1; ++c) {
      filtered_row[c] = (row[c + 1] - row[c - 1]) / 2 + 128;
    }
    std::copy(filtered_row, filtered_row + image.w(), row);
  }
}

bool sobelFiltre(GSImage& image) {
  float const gx[3][3] = {
    {-1.0, 0.0, 1.0},
    {-2.0, 0.0, 2.0},
    {-1.0, 0.0, 1.0}  
  };

  float const gy[3][3] = {
    {-1.0, -2.0, -1.0},
    {0.0, 0.0, 0.0},
    {1.0, 2.0, 1.0}
  };

  float sx, sy, s;
  for(int r = 0; r < image.h(); ++r) {
    float* row = image.pixels() + r * image.w();
    for(int c = 0; c < image.w() - 1; ++c) {
      sx = sy = 0.0f;
      for(int i = -1; i <= 1; i++) {
        sx += row[c - image.w() + i] * gx[0][i+1];
        sx += row[c + i] * gx[1][i+1];
        sx += row[c + image.w() + i] * gx[2][i+1];

        sy += row[c - image.w() + i] * gy[0][i+1];
        sy += row[c + i] * gy[1][i+1];
        sy += row[c + image.w() + i] * gy[2][i+1];
      }

      s = sqrt(sx * sx + sy * sy);
      row[c] = s > 1.0f ? 0.0f : s < 0.0f ? 1.0f : s; 
    }
  }
  return true;
}

void on_error(char const* executable) {
  std::cout << "Usage: " << executable << " <source_file.png> <target_file.png>\n";
  exit(1);
}

int main(int argc, char const* argv[]) {
  if(argc != 3)
    on_error(argv[0]);

  GSImage image(argv[1]);
  sobelFiltre(image);
  image.save_png(argv[2]);

  return 0;
}