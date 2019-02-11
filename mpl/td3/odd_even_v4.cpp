/*
 * \file odd_even_v4.cpp
 * \brief Utilisation de std::mutex pour bloquer et protéger les données entre 
 * plusieurs variables (à l'aide de la méthode lock et unlock). 
 */
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
 
std::vector<int> numbers;
std::mutex numbers_mutex;

void save_number(int num) {
    // acquiring the lock
    numbers_mutex.lock();
    // critical section
    numbers.push_back(num);
    // releasing the lock
    numbers_mutex.unlock();
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