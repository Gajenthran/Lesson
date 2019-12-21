//indexer_v1.cpp

#include "indexer_v1.hpp"

#include <cassert>
#include <fstream>
#include <vector>
#include <ctime>
#include <iostream>
#include <chrono>


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
void index_file(Indexer& indexer, std::string const& filename) {
    std::ifstream file(filename, std::ios_base::in);
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


int main(int argc, char const* argv[]) {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    assert(argc == 2);
    std::string path = argv[1];
    auto file_list = files(path + "ls");

    assert(file_list.size() == 598);

    Indexer indexer;
    for(auto const& file : file_list) {
        index_file(indexer, path + file);
    }

    assert(indexer.size() == 437438);
    
    end = std::chrono::system_clock::now();
    int elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>
                                                         (end-start).count();
    std::cout << "Elapsed time: " << elapsed_seconds << "s\n";

    return 0;
}