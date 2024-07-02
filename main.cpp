#include "ent_manager.h"
#include <iostream>
#include <random>
int main() {
    // Create an ent_array of integers
    ent_array<int> array;

    // Add blocks to the array
    array.add_block();
    array.add_block();
    //array.add_block();

    // Populate blocks with integers
    for (int i = 0; i < 1000; ++i) {
        array.insert(i);
    }
    for (size_t i = 0; i < 92; i++)
    {
        array.remove(rand() % 1000);
    }

    

    std::cout << "Before compression:" << std::endl;
    //array.print_blocks();
    array.print_blocks();
    std::cout << '\n';
    //array.print_bitset(1);
    

    std::cout << "Compressing...";
    // Perform compression
    array.compress();
    std::cout << "Done!" << std::endl;



    std::cout << "After compression:" << std::endl;
    array.print_blocks();
    std::cout << '\n';
    //array.print_bitset(1);

    return 0;
}