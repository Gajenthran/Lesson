/*
 * \file odd_even_v3.cpp
 * \brief Modification de la fonction save_number pour bloquer un thread et lancer l'autre.
 * Et utilisation de std::atomic pour les accès conccurents
 */
#include <iostream>
#include <vector>
#include <array>
#include <thread>
 
std::vector<int> numbers;
std::array<std::atomic<bool>, 2> wants_to_enter;
std::atomic<int> turn;

void save_number(int num, int id) {
    // mutual exclusion with only 2 threads using Dekker's algorithm

    // acquiring the lock
    wants_to_enter[id] = true;
    while(wants_to_enter[1 - id]) {
        if(turn != id) {
            wants_to_enter[id] = false;
            while(turn != id) {
                // busy wait
            }
            wants_to_enter[id] = true;
        }
    }

    // critical section
    numbers.push_back(num);

    // releasing the lock
    turn = 1 - id;
    wants_to_enter[id] = false;
}

void generate_even(int max) {
    for(int i = 0; i < max; i += 2) {
        save_number(i, 0);
    }
}
 
void generate_odd(int max) {
    for(int i = 1; i < max; i += 2) {
        save_number(i, 1);
    }
}
 
int main() {
    wants_to_enter[0] = false;
    wants_to_enter[1] = false;
    turn = 0;

    std::thread t1(generate_even, 10);
    std::thread t2(generate_odd, 10);
    t1.join();
    t2.join();
 
    for(auto n : numbers) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
}