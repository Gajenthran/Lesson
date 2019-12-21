/*
 * \file odd_even_v1.cpp
 * \brief Problème de mémoire. Allocation de la mémoire par les deux threads 
 * et donc problème de mémoire avec push_back : data races 
 * (cf. http://www.cplusplus.com/reference/vector/vector/push_back/)
 */ 
#include <iostream>
#include <vector>
#include <thread>

std::vector<int> numbers;

void save_number(int num) {
    numbers.push_back(num);
}

void generate_even(int max) {
    for(int i = 0; i < max; i += 2) {
        save_number(i);
    }
}
 
void generate_odd(int max) {
    for(int i = 1; i < max; i += 2) {
        save_number(i);
    }
}
 
int main() {
    std::thread t1(generate_even, 100);
    std::thread t2(generate_odd, 100);
    t1.join();
    t2.join();
 
    for(auto n : numbers) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
}