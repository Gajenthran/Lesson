#include <thread>
#include <iostream>
#include <vector>
#include <future>
#include <random>
#include <benchmark/benchmark.h>

// Nombre de thread créé
#define NTHREAD 2

/*
 * Initialisation du tableau de taille NTAB.
 */
auto initVec(int n) {
  std::random_device rd;  
  std::mt19937 gen(rd()); 
  std::uniform_int_distribution<int> dis;
  std::vector<int> vec;
  vec.reserve(n);
  for(int i = 0; i < n; ++i) {
      auto num = i; 
      vec.push_back(num);
  }
  return vec;
}

/*
 * Recherche de la valeur max du tableau où chaque élément
 * représente le max obtenu par les threads.
 */
int maxVec(std::vector<int> vec, int debut, int fin) {
  int max = vec[debut];
  for(int i = debut; i < fin; i++) {
    if(max < vec[i])
      max = vec[i];
  }
  return max;
}

/*
 * Recherche de la valeur max en comparant les différentes valeurs
 * obtenues grâces aux threads
 */
int maxFuture(std::future<int> * max) {
  int m = max[0].get(), tmp;
  for(int i = 1; i < NTHREAD; i++) {
    tmp = max[i].get();
    if(m < tmp)
      m = tmp;
  }
  return m;
}

void runThreads(benchmark::State& state) {
  auto vec = initVec(state.range(0));
  std::cout << vec.size() << "\n" ;
  std::future<int> max[vec.size()];

  int part = vec.size()/NTHREAD, begin = 0, end = part, i = 0;
  for(auto _ : state) {
    while(i != NTHREAD) { 
      max[i] = std::async(std::launch::async, maxVec, vec, begin, end);
      end = (++i == NTHREAD-1) ? vec.size() : end + part;
      begin += part;
    }
    
    std::cout << "Max du vecteur: " << maxFuture(max) << "\n";
  }

  // std::cout << "Max du vecteur: " << maxFuture(max) << "\n";
}

BENCHMARK(runThreads)->RangeMultiplier(8)->Range(8, 8 << 21);
BENCHMARK_MAIN();

