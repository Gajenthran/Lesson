#include <random>
#include <algorithm>
#include <vector>
#include <iostream>

#define N 10000

auto init_tab = [](auto begin, auto end) {
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, 100);
  for(auto it = begin; it != end; ++it) {
		*it = distribution(generator);
  }
};

auto print_tab = [](auto begin, auto end) {
	for(auto it = begin; it != end; ++it)
		std::cout << *it << "\n";
};

/* auto merge_sort = [](auto begin, auto end) {
	std::vector<int> v0(N);
	std::sort(begin, begin + N/2);
	std::sort(begin + N/2, end);
	std::merge(begin, begin + N/2, begin + N/2, end, begin(v0));

	return v0;
}; */

int main(int argc, char* argv[]) {
	std::vector<int> v(N);
	std::vector<int> v0(N);
	init_tab(begin(v), end(v));

	// merge sort
	/* std::sort(begin(v), begin(v) + N/2);
	std::sort(begin(v) + N/2, end(v));
	std::merge(begin(v), begin(v) + N/2, begin(v)+ N/2, end(v), begin(v0)); */

	// heap sort
  std::make_heap (begin(v),end(v));
	std::sort_heap(begin(v), end(v));
}