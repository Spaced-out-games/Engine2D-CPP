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
    T get(int index) const
    {
        // Determine block ID and index within block
        if (index < 0 || index >= size())
        {
            throw std::out_of_range("Index out of range");
        }

        int block_ID = index / 512;
        int index_in_block = index % 512;

        if (block_ID < blocks.size())
        {
            return blocks[block_ID].get(index_in_block);
        }
        throw std::out_of_range("Invalid block index");
    }

    // Check if an entity is valid using an ent_handle
    bool is_valid(const ent_handle& handle) const
    {
        // Ensure the block index is valid
        if (handle.block_ID < blocks.size())
        {
            return blocks[handle.block_ID].is_valid(handle);
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
        //blocks[0].remove(2);
        if (blocks.empty()) return;

        // Initialize block indices
        size_t leftmost_block_index = 0;
        size_t rightmost_block_index = blocks.size() - 1;
        int left_index = -1;
        int right_index = -1;

        // Find the leftmost block that isn't full
        while (leftmost_block_index < blocks.size() && blocks[leftmost_block_index].isFull()) {
            leftmost_block_index++;
        }

        // If we found no non-full blocks, there's nothing to compress
        //if (leftmost_block_index >= blocks.size()) return;

        // Find the rightmost block that has contents
        while (rightmost_block_index > leftmost_block_index && blocks[rightmost_block_index].isEmpty()) {
            rightmost_block_index--;
        }

        // If we found no non-empty blocks, there's nothing to compress
        //if (rightmost_block_index <= leftmost_block_index) return;


        int debug_itercnt = 0;

        int debug_hitch = 0;
        // Process blocks to compress them
        while (!(leftmost_block_index == rightmost_block_index && right_index == left_index + 1))
        {



            // Get the index of the first empty slot in the leftmost block
            left_index = blocks[leftmost_block_index].getFirstEmpty();

            // Get the index of the last occupied slot in the rightmost block
            right_index = blocks[rightmost_block_index].getLastFull();

            // Move inward if necessary
            while (left_index == -1 && leftmost_block_index < rightmost_block_index)
            {
                leftmost_block_index++;
                if (leftmost_block_index >= blocks.size()) break;
                left_index = blocks[leftmost_block_index].getFirstEmpty();
            }

            while (right_index == -1 && rightmost_block_index > leftmost_block_index)
            {
                rightmost_block_index--;
                if (rightmost_block_index == static_cast<size_t>(-1)) break;
                right_index = blocks[rightmost_block_index].getLastFull();
            }

            // Ensure valid indices before proceeding
            if (left_index != -1 && right_index != -1)
            {
                // Move entity from the rightmost block to the leftmost block
                blocks[leftmost_block_index].insert(blocks[rightmost_block_index].get_entity(right_index));

                // Update the bitsets
                blocks[rightmost_block_index].get_bitset().set(right_index, 0);  // Mark slot as empty
                blocks[leftmost_block_index].get_bitset().set(left_index, 1);   // Mark slot as occupied
            }
            else
            {
                // If no valid slots are found, break the loop
                break;
            }

            //if (leftmost_block_index == rightmost_block_index && right_index == left_index + 1) { break; }
        }
    }

    void add_block() {
        blocks.emplace_back();
    }

    //template <typename T>
    void print_blocks() {
        for (size_t i = 0; i < blocks.size(); ++i) {
            std::cout << "\nBlock " << i << ": \n";
            for (int j = 0; j < 512; ++j) {
                if (blocks[i].get_bitset().get(j)) {
                    std::cout << blocks[i].get_entity(j) << "\t";
                }
                else
                {
                    std::cout << "(X) \t";
                }
            }
        }
    }
    void print_block(int blockID) {

        std::cout << "\nBlock " << blockID << ": \n";
        for (int j = 0; j < 512; ++j) {
            if (blocks[blockID].get_bitset().get(j)) {
                std::cout << blocks[blockID].get_entity(j) << "\t";
            }
            else
            {
                std::cout << "(X) \t";
            }
        }
    }

    T get_entity(bitset_size_t block_ID, bitset_size_t ent_index)
    {
        blocks
    }

    
    void print_bitset(int index)
    {
        auto& target_block = blocks[index];

        std::cout << target_block.get_bitset();
    }

private:
    std::vector<ent_block<T>> blocks; // Vector of blocks
};

#endif // ENT_ARRAY_H