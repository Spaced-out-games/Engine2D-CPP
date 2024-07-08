#ifndef ENT_MANAGER_H
#define ENT_MANAGER_H

#include <any>
#include <vector>
#include <typeindex>
#include <stdexcept>
#include <memory>
#include "ent_array.h"

class ent_manager
{
public:
    template<typename T>
    void add_array()
    {
        // Create and store a new ent_array<T> instance
        arrays_.emplace_back(ent_array<T>());

        // Automatically flag this ent_array for cleanup
        auto_cleanup.set(arrays_.size(), 1);
    }

    template<typename T>
    ent_array<T>& get_array()
    {
        for (auto& arr : arrays_)
        {
            if (auto* typed_arr = std::any_cast<ent_array<T>>(&arr))
            {
                return *typed_arr;
            }
        }
        throw std::out_of_range("No ent_array found for the specified type");
    }

    template<typename T>
    const ent_array<T>& get_array() const
    {
        for (const auto& arr : arrays_)
        {
            if (const auto* typed_arr = std::any_cast<const ent_array<T>>(&arr))
            {
                return *typed_arr;
            }
        }
        throw std::out_of_range("No ent_array found for the specified type");
    }

    template<typename T>
    T getEntity(ent_handle handle)
    {
        // Access the correct ent_array using the type_ID from the handle
        ent_array<T> arr = get_array<T>();

        // Get the entity from the correct block and index within the array
        auto a = arr.get(handle);
        return a;
    }  
    template <typename T>
    const T& getEntity(int index)
    {
        const ent_array<T>& arr = get_array<T>();
        return arr.get(index);
    }

private:
    std::vector<std::any> arrays_;
    bitset_512 auto_cleanup;
};

#endif // ENT_ARRAY_MANAGER_H