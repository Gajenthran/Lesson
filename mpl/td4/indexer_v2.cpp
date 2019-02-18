//indexer_v2.cpp

#include "indexer_v1.hpp"

#include <cassert>
#include <fstream>
#include <vector>
#include <ctime>
#include <iostream>
#include <future>
#include <benchmark/benchmark.h>

// load a list of filenames from a file
std::vector<std::string> files(std::string const& path) {
	char buffer[256];
	std::vector<std::string> file_list;
	std::ifstream file(path, std::ios_base::in);
	while(file.good()) {
		file.getline(buffer, 256);
		file_list.push_back(buffer);
	}
	return file_list;
}

// load a file and store its words into the indexer
auto index_file = [](Indexer& indexer, auto begin, auto end) {
	std::string path = "gutenberg-books/";
	for(auto it = begin; it != end; ++it) {
		std::ifstream file(path + *it, std::ios_base::in);
		std::string buffer;
		enum { word, space } state = space;
		while(file.good()) {
			int c = std::tolower(file.get());
			if(c >= 'a' && c <= 'z') {
				if(state == space ) {
					state = word;
					buffer.clear();
				}
				buffer.push_back(c);
			} else {
				if(state == word) {
					indexer.get(buffer);
					state = space;
				}
			}
		}
	}
};

void indexer_multithread(benchmark::State& state) {
	auto nb_threads = state.range(0);
	std::string path = "gutenberg-books/";
	auto file_list = files(path + "ls");

	assert(file_list.size() == 598);

	Indexer indexer;
	for(auto _ : state) {
		auto it = begin(file_list);
		for(int i = 0; i < nb_threads - 1; ++i) {
			std::async(std::launch::async, index_file, std::ref(indexer), it, it + file_list.size() / nb_threads);
			it = it + file_list.size() / nb_threads;
		}
		std::async(std::launch::async, index_file, std::ref(indexer), it, end(file_list));
	}

	assert(indexer.size() == 437438);
}

BENCHMARK(indexer_multithread)->Arg(2)->Arg(3);
BENCHMARK_MAIN();