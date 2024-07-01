#include "ent_manager.h"
#include <iostream>

int main()
{
    ent_manager manager;

    // Add ent_array for different types
    manager.add_array<int>();
    manager.add_array<float>();

    // Get and use ent_array<int>
    auto& int_array = manager.get_array<int>();
    ent_handle int_handle = int_array.insert(42);
    std::cout << "Inserted int: " << int_array.get(int_handle) << std::endl;

    // Get and use ent_array<float>
    auto& float_array = manager.get_array<float>();
    ent_handle float_handle = float_array.insert(3.14f);
    std::cout << "Inserted float: " << float_array.get(float_handle) << std::endl;

    return 0;
}