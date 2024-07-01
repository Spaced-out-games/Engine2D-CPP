#ifndef ENT_ARRAY_H
#define ENT_ARRAY_H

#include "ent_block.h"
#include "ent_handle.h"
#include <vector>
#include <stdexcept> // For std::out_of_range and std::overflow_error

template<typename T>
class ent_array
{
public:
    // Insert an entity into the first available block or create a new block if necessary
    ent_handle insert(const T& entity)
    {
        // Iterate through existing blocks to find one that is not full
        for (size_t i = 0; i < blocks.size(); ++i)
        {
            if (!blocks[i].isFull())
            {
                return blocks[i].insert(entity);
            }
        }

        // If all blocks are full, create a new block and insert the entity
        blocks.emplace_back();
        return blocks.back().insert(entity);
    }

    // Retrieve an entity using an ent_handle
    T get(const ent_handle& handle) const
    {
        // Find the block index and the index within the block from the handle
        if (handle.block_ID < blocks.size())
        {
            return blocks[handle.block_ID].get(handle);
        }
        throw std::out_of_range("Invalid block index in handle");
    }

    // Check if an entity is valid using an ent_handle
    bool is_valid(const ent_handle& handle) const
    {
        // Ensure the block index is valid
        if (handle.block_index < blocks.size())
        {
            return blocks[handle.block_index].is_valid(handle);
        }
        return false; // Invalid block index
    }

    // Remove an entity using an ent_handle
    void remove(const ent_handle& handle)
    {
        // Ensure the block index is valid
        if (handle.block_index < blocks.size())
        {
            blocks[handle.block_index].remove(handle);
        }
        else
        {
            throw std::out_of_range("Invalid block index in handle");
        }
    }

    // Get the total number of entities across all blocks
    bitset_size_t size() const
    {
        bitset_size_t total_size = 0;
        for (const auto& block : blocks)
        {
            total_size += block.size();
        }
        return total_size;
    }

    // Get the total capacity across all blocks
    bitset_size_t capacity() const
    {
        return blocks.size() * BITSET_SIZE_BITS;
    }

    // Reset all blocks and clear the array
    void reset()
    {
        blocks.clear();
    }

private:
    std::vector<ent_block<T>> blocks; // Vector of blocks
};

#endif // ENT_ARRAY_H