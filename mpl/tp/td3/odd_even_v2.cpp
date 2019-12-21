/*
 * \file odd_even_v2.cpp
 * \brief Deux cas possibles :
 * - Meilleur cas : un thread est lancé, le deuxième attend la fin du première pour s'exécuter
 * - Pire cas : les deux threads se lance en même temps donc on revient au problème initial
 */
#include <iostream>
#include <vector>
#include <thread>
 
std::vector<int> numbers;
bool lock = false;

void save_number(int num) {
    while(lock) {}
    lock = true;
    numbers.push_back(num);
    lock = false;
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