#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <cstdint>
#include "shared_types.h"

/*
    Basic entity class. It's got no real behavior, just provides an interface for extended types to keep track of their ref counts, as well as an interface for the engine to check if the Entity is being referenced
    so that it can be moved to the appropriate slot by the engine, ensuring stability for objects that are referenced.

*/

class Entity : public std::enable_shared_from_this<Entity>
{
public:
    Entity() = default;

    // Method to get a shared pointer to itself
    std::shared_ptr<Entity> getSharedPtr()
    {
        return shared_from_this();
    }

    // Method to get the reference count
    long getReferenceCount() const
    {
        return shared_ptr.use_count();
    }



private:

    std::shared_ptr<Entity> shared_ptr; // Holds the shared pointer to itself
};

#endif