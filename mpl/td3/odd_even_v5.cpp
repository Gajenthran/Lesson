/*
 * \file odd_even_v5.cpp
 * \brief Utilisation de mutex (comme .v4) avec std::lock_guard pour protéger 
 * les données
 */

#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
 
std::vector<int> numbers;
std::mutex numbers_mutex;

void save_number(int num) {
    std::lock_guard<std::mutex> lock(numbers_mutex);
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
    std::thread t1(generate_even, 10);
    std::thread t2(generate_odd, 10);
    t1.join();
    t2.join();
 
    for(auto n : numbers) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
}