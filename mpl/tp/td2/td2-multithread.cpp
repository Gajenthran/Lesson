/*
 * Calcul d'un tableau de N éléments à l'aide de plusieurs thread.
 * Code repris du cours de "Machine parallèles" (de  Jean-Noel Vittaut (http://www.ai.univ-paris8.fr/~jnv/))
 */
#include <vector>
#include <thread>
#include <future>
#include <iostream>
#include <random>
#include <benchmark/benchmark.h>

auto max_range = [](auto begin, auto end) {
  auto m = *begin;
  for(auto it = begin; it != end; ++it) {
    if(*it > m) {
      m = *it;
    }
  }
  return m;
};

std::vector<int> random_vector(int n) {
  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<int> dis;
  std::vector<int> vec;
  vec.reserve(n);
  for(int i = 0; i < n; ++i) {
    auto num = dis(gen);
    vec.push_back(num);
  }
  return vec;
}

int x = 0;

void max_single_thread(benchmark::State& state) {
  std::vector<int> vec = random_vector(state.range(0));

  for(auto _ : state) {
    x += max_range(begin(vec), end(vec));
  }
}

void max_multithread(benchmark::State& state) {
  std::vector <int> vec = random_vector(1 << 24);
  auto nb_threads = state.range(0);
  for(auto _ : state) {
    std::vector<std::future<int>> future_results;
    auto it = begin(vec);
    for(int i = 0; i < nb_threads - 1; ++i) {
      future_results.push_back(std::async(std::launch::async, max_range, it, it + vec.size() / nb_threads));
      it = it + vec.size() / nb_threads;
    }
    future_results.push_back(std::async(std::launch::async, max_range, it, end(vec)));
    std::vector<int> results;
    std::transform(begin(future_results), end(future_results), back_inserter(results), [](auto& x) { return x.get(); });
    x += max_range(begin(results), end(results));
  }
}


BENCHMARK(max_single_thread)->Arg(1 << 24);
BENCHMARK(max_multithread)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Arg(7)->Arg(8);

BENCHMARK_MAIN();