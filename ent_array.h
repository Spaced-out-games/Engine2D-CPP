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

    // Fallback option
    void remove(int index)
    {
        int block_ID = index / 512;
        index = index % 512;
        blocks[block_ID].remove(index);
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



    void compress()
    {
        // The block index of the left pointer
        size_t leftmost_block_index = 0;

        // The left pointer block index
        size_t left_index = 0;

        // The block index of the right pointer
        size_t rightmost_block_index = size() - 1;

        // The right pointer block index
        size_t right_index = 0;

        // Find the leftmost block that isn't full
        while (!blocks[leftmost_block_index].isFull() && leftmost_block_index < blocks.size())
        {
            leftmost_block_index++;
        }

        // If we got to the end from the left and there wasnt any empty slots, abort
        if (leftmost_block_index == blocks.size() && !blocks[leftmost_block_index -1].isFull()) { return; }

        // Find the rightmost block that has contents
        while (blocks[rightmost_block_index].isEmpty() && rightmost_block_index >= 0)
        {
            rightmost_block_index--;
        }
        // If we got to the beginning of the array and it has no contents, abort
        if (rightmost_block_index == -1 && blocks[0].isEmpty()) { return; }



        // Now that we have the correct starting blocks, find the correct indices
        while (leftmost_block_index < rightmost_block_index)
        {
            // Get the index of the first empty element in the leftmost block
            left_index = blocks[leftmost_block_index].getFirstEmpty();

            // Get the index of the last occupied element of the rightmost block
            right_index = blocks[rightmost_block_index].getLastFull();

            // Move either block inward if necessary

            // There are no empty spaces on the left
            while (left_index == -1)
            {
                // So move inward from the left
                leftmost_block_index++;
                left_index = blocks[leftmost_block_index].getFirstEmpty();
            }

            // There are no occupied spaces on the right
            while (right_index == -1)
            {
                rightmost_block_index--;
                right_index = blocks[rightmost_block_index].getLastFull();
            }

            // Now that we have verified we have valid block indices, move from the right to the left

            // Insert a copy of the rightmost to the leftmost slot
            blocks[leftmost_block_index].insert(blocks[rightmost_block_index].get_entity(right_index));

            // Update the state of the rightmost
            blocks[rightmost_block_index].get_bitset().set(right_index, 0);

            // And update the state of the leftmost
            blocks[leftmost_block_index].get_bitset().set(left_index, 1);


        }


    }

    void add_block() {
        blocks.emplace_back();
    }

    //template <typename T>
    void print_blocks() {
        for (size_t i = 0; i < blocks.size(); ++i) {
            std::cout << "Block " << i << ": ";
            for (int j = 0; j < 512; ++j) {
                if (blocks[i].get_bitset().get(j)) {
                    std::cout << blocks[i].get_entity(j) << " ";
                }
                else
                {
                    std::cout << "(EMPTY SLOT)";
                }
            }
            std::cout << std::endl;
        }
    }
private:
    std::vector<ent_block<T>> blocks; // Vector of blocks
};

#endif // ENT_ARRAY_H