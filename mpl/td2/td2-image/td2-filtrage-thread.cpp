#include <vector>
#include <thread>
#include <future>
#include <iostream>
#include <random>
#include <benchmark/benchmark.h>
#include "td2-gsimage.hpp"

char const* img_src  = "img/moutain.jpg";
char const* img_dest = "img/moutain_filter.jpg";

bool filtre(GSView& view) {
  uint8_t filtered_row[view.w];
  for(int r = view.y; r < view.h; ++r) {
    uint8_t* row = (view.image->pixels() + view.y) + r * view.w;
    for(int c = view.x; c < view.w - 1; ++c) {
        filtered_row[c] = (row[c + 1] - row[c - 1]) / 2 + 128;
    }
    std::copy(filtered_row, filtered_row + view.w, row);
  }
  return true;
}

GSView create_view(GSImage& image, int x, int y, int w, int h) { 
  GSView view;
  view.x = x;
  view.y = y;
  view.w = w;
  view.h = h;
  view.image = &image;
  return view;
}

void on_error(char const* executable) {
  std::cout << "Usage: " << executable << " <source_file.png> <target_file.png>\n";
  exit(1);
}

bool x = false;

void filtre_single_thread(benchmark::State& state) {
  GSImage image(img_src);
  GSView view = create_view(image, 0, 0, image.w(), image.h());
  for(auto _ : state) {
    x = filtre(view);
    break;
  }
  image.save_png(img_dest);
}

void filtre_multithread(benchmark::State& state) {
  auto nb_threads = state.range(0);
  GSImage image(img_src);
  for(auto _ : state) {
    auto it = 0;
    std::vector<std::future<int>> future_results;
    for(int i = 0; i < nb_threads - 1; ++i) {
      GSView view_img = create_view(image, 0, it, image.w(), it + image.h() / nb_threads);
      std::async(std::launch::async, [&] { return filtre (view_img); } );
      it = it + image.h() / nb_threads;
    }
    GSView view_img = create_view(image, 0, it, image.w(), image.h());
    std::async(std::launch::async, [&] { return filtre (view_img); } );
    break;
  }
  image.save_png(img_dest);
} 

//BENCHMARK(filtre_single_thread)->Arg(2);
BENCHMARK(filtre_multithread)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Arg(7)->Arg(8);

BENCHMARK_MAIN();