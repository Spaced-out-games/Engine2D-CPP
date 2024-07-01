#ifndef ENT_BLOCK_H
#define ENT_BLOCK_H

#include "bitset_512.h" // Include the bitset_512 header file
#include "ent_handle.h" // Include the ent_handle header file
#include <stdexcept>    // For std::out_of_range

template<typename T>
class ent_block
{
public:
    ent_block() : entity_count(0) {}

    // Insert an entity and return the ent_handle for it
    ent_handle insert(const T& entity)
    {
        bitset_size_t index = states.find_first_empty_slot();
        if (index != -1)
        {
            entities[index] = entity;
            states.set(index, true); // Mark as valid
            ++entity_count;
            return ent_handle(index); // Return a handle for the inserted entity
        }
        throw std::overflow_error("No available slot to insert entity");
    }

    // Retrieve an entity using an ent_handle
    T get(const ent_handle& handle) const
    {
        bitset_size_t index = handle.ent_index;
        if (index < BITSET_SIZE_BITS && states.get(index))
        {
            return entities[index];
        }
        throw std::out_of_range("Invalid handle or entity not set");
    }

    // Check if an entity is valid using an ent_handle
    bool is_valid(const ent_handle& handle) const
    {
        bitset_size_t index = handle.ent_index;
        return index < BITSET_SIZE_BITS&& states.get(index);
    }

    // Remove an entity using an ent_handle
    void remove(const ent_handle& handle)
    {
        bitset_size_t index = handle.ent_index;
        if (index < BITSET_SIZE_BITS && states.get(index))
        {
            states.set(index, false); // Mark as invalid
            entities[index] = T{};    // Optionally clear the entity
            --entity_count;
        }
    }

    // Reset all entities to invalid
    void reset()
    {
        states.zeros(); // Mark all as invalid
        entity_count = 0;
    }

    // Get the current number of entities
    bitset_size_t size() const
    {
        return entity_count;
    }

    // Get the capacity of the block
    bitset_size_t capacity() const
    {
        return BITSET_SIZE_BITS;
    }

    // Check if the block is full
    bool isFull() const
    {
        return entity_count == BITSET_SIZE_BITS;
    }

private:
    T entities[BITSET_SIZE_BITS]; // Array to store entities
    bitset_512 states;             // Bitset to track validity of each entity
    bitset_size_t entity_count;    // Counter for the number of valid entities
};

#endif // ENT_BLOCK_H