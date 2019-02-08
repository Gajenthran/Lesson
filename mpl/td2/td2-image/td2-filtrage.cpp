// filtrage.cpp

#include <iostream>
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

void on_error(char const* executable) {
  std::cout << "Usage: " << executable << " <source_file.png> <target_file.png>\n";
  exit(1);
}

int main(int argc, char const* argv[]) {
  if(argc != 3)
    on_error(argv[0]);

  GSImage image(argv[1]);
  filtre(image);
  image.save_png(argv[2]);

  return 0;
}