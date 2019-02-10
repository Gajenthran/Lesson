#include <vector>
#include <thread>
#include <future>
#include <iostream>
#include <random>
#include <cmath>
#include "td2-gsimage.hpp"
#include <benchmark/benchmark.h>

char const* img_src  = "img/maison.jpg";
char const* img_dest = "img/moutain_filter.jpg";

bool filtre(GSImage::View& view) {
  float filtered_row[view.w];
  for(int r = view.y; r < view.h; ++r) {
    float* row = view.getPixel(r-view.y, 0); // (view.image.pixels() + view.y) + r * view.w;
    for(int c = view.x; c < view.w; ++c) {
        filtered_row[c] = (row[c + 1] - row[c - 1]) / 2 + 128;
    }
    std::copy(filtered_row, filtered_row + view.w, row);
  }
  return true;
}

bool sobelFiltre(GSImage::View& view) {
  int const gx[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
  };

  int const gy[3][3] = {
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1}
  };

  float sx, sy, s;
  for(int r = 0; r < view.h; ++r) {
    float* row = view.operator[](r);
    for(int c = 0; c < view.w; ++c) {
      sx = sy = 0.0f;
      for(int i = -1; i <= 1; i++) {
        sx += row[c - view.image.w() + i] * (float)gx[0][i+1];
        sx += row[c + i] * gx[1][i+1];
        sx += row[c + view.image.w() + i] * (float)gx[2][i+1];

        sy += row[c - view.image.w() + i] * (float)gy[0][i+1];
        sy += row[c + i] * gy[1][i+1];
        sy += row[c + view.image.w() + i] * (float)gy[2][i+1];
      }

      s = sqrt(sx * sx + sy * sy);// std::abs(sx) + std::abs(sy);
      row[c] = s > 0.8f ? 0.0f : s < 0.0f ? 1.0f : s; 
      // std::cout << s << "\n";
    }
  }
  return true;
}

void on_error(char const* executable) {
  std::cout << "Usage: " << executable << " <source_file.png> <target_file.png>\n";
  exit(1);
}

bool x = false;

void filtre_single_thread(benchmark::State& state) {
  GSImage image(img_src);
  GSImage::View view = image.view(0, 0, image.w(), image.h()); // image.w(), image.h());
  for(auto _ : state) {
    // x = filtre(view);
    // x = sobelFiltre(view);
    break;
  }
  image.save_png(img_dest);
}

void filtre_multithread(benchmark::State& state) {
  auto nb_threads = state.range(0);
  GSImage image(img_src);
  for(auto _ : state) {
    auto it = 0;  
    for(int i = 0; i < nb_threads - 1; ++i) {
      GSImage::View view = image.view(0, it, image.w(), it + image.h() / nb_threads);
      std::async(std::launch::async, [&] { return filtre(view); } );
      it = it + view.image.h() / nb_threads;
    }
    
    GSImage::View view = image.view(0, it, image.w(), image.h());
    std::async(std::launch::async, [&] { return filtre(view); } );
    break;
  }
  image.save_png(img_dest);
}

BENCHMARK(filtre_single_thread)->Arg(2);
// BENCHMARK(filtre_multithread)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Arg(7)->Arg(8);
BENCHMARK_MAIN();