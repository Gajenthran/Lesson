// sort-multithread.cpp

#include <vector>
#include <thread>
#include <future>
#include <iostream>
#include <random>
#include <benchmark/benchmark.h>


auto qsort_vector = [](auto begin, auto end) {
  std::sort(begin, end);
};

std::vector<int> random_vector(int n) {
  std::random_device rd;  
  std::mt19937 gen(rd()); 
  std::uniform_int_distribution<int> dis;
  std::vector<int> vec;
  vec.reserve(n);
  for(int i = 0; i < n; ++i) {
    auto num = dis(gen);
    vec.push_back(num);
  }
  return vec;
}

void sort_multithread(benchmark::State& state) {
  std::vector<int> vec = random_vector(1 << 24);
  auto nb_threads = state.range(0);
  for(auto _ : state) {
    std::vector<std::future<int>> future_results;
    auto it = begin(vec);
    for(int i = 0; i < nb_threads - 1; ++i) {
      std::async(std::launch::async, qsort_vector, it, it + vec.size() / nb_threads);
      it = it + vec.size() / nb_threads;
    }
    std::async(std::launch::async, qsort_vector, it, end(vec));
  }
}

BENCHMARK(sort_multithread)->Arg(2);

BENCHMARK_MAIN();