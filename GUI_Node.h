#pragma once

#ifndef MESH3D_H
#include "Mesh2D.h"
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm> // For std::min and std::max
#include <array>
//#include "engineCore.h"
//class engineCore;
// Have GUI_Nodes query a slot within an hb_manager upon construction. This allows all the hitboxes to be in parallel, which is good for performance


class GUINode
{
public:
    bool colliding;
    GUINode* parent;
    GUINode** children; // Fixed-size array for children
    inline static Mesh2D* rectangle = nullptr;
    size_t child_count;
    size_t occupied_slots;
    glm::vec3 translation;
    glm::vec3 scale;
    glm::vec3 color;
    float rotation;
    #ifdef _DEBUG
    bool debug_print = true;
    #endif

    static void init()
    {
        GLfloat vertices[] = {
            // Positions         
            0.5f,  0.5f,
            0.5f, -0.5f,
           -0.5f, -0.5f,
           -0.5f,  0.5f,
        };

        GLuint indices[] = {
            0, 1, 3,
            1, 2, 3
        };
        #ifdef _DEBUG
        std::cout << "3. Prepare your vertex data, which can involve loading from a file or creating in memory.\n";
        #endif
        rectangle = new Mesh2D(vertices, 8, indices, 6);

    }

    GUINode()
        : parent(nullptr), children(nullptr), child_count(0), occupied_slots(0),
        translation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), color(1.0f, 1.0f, 1.0f), swap_color(0.2, 0.2, 0.2), colliding (false)
    {
        rotation = 0.0f;
        children = new GUINode * [child_count];
        for (size_t i = 0; i < child_count; ++i)
        {
            children[i] = nullptr;
        }
    }

    GUINode(glm::vec3 initial_translation, glm::vec3 initial_scale, glm::vec3 initial_color, size_t max_children)
        : parent(nullptr), child_count(max_children), occupied_slots(0), colliding(false),
        translation(initial_translation), scale(initial_scale), color(initial_color), swap_color(0.2,0.2,0.2)
    {
        rotation = 0.0f;
        children = new GUINode * [child_count];
        for (size_t i = 0; i < child_count; ++i)
        {
            children[i] = nullptr;
        }
    }

    virtual ~GUINode() = default;

    virtual void addChild(GUINode* new_child)
    {
        if (occupied_slots < child_count)
        {
            children[occupied_slots] = new_child;
            new_child->parent = this;
            ++occupied_slots;
        }

    }
    virtual void draw()
    {
        glm::mat4 transform = get_nested_transform();
        #ifdef _DEBUG
        if (debug_print)
        {
            std::cout << "6. Set any required uniforms.";
            debug_print = false;
        }
        
        #endif // DEBUG

        Shader::setUniform("transform", transform);
        Shader::setUniform("color", color);
        


        rectangle->draw();
        for (size_t i = 0; i < occupied_slots; ++i)
        {
            if (children[i] != nullptr)
            {
                children[i]->draw();
            }
        }
    }

    glm::mat4 get_nested_transform()
    {
        glm::mat4 rel_transform(1.0f);
        float theta = glm::radians(rotation);
        glm::vec3 axis(0.0f, 0.0f, 1.0f);
        rel_transform = glm::translate(rel_transform, translation);
        rel_transform = glm::rotate(rel_transform, theta, axis);
        rel_transform = glm::scale(rel_transform, scale);
        if (parent != nullptr)
        {
            return parent->get_nested_transform() * rel_transform;
        }
        else
        {
            return rel_transform;
        }
    }
    glm::vec3 scaleFromMatrix(const glm::mat4& matrix) {
        glm::vec3 scale;
        // Extract columns of the matrix
        glm::vec3 col1(matrix[0]);
        glm::vec3 col2(matrix[1]);
        glm::vec3 col3(matrix[2]);

        // Compute scale factors
        scale.x = glm::length(col1);
        scale.y = glm::length(col2);
        scale.z = glm::length(col3);

        return scale;
    }
    glm::vec4 getBounds()
    {
        // Define the corners of the rectangle in local space
        std::array<glm::vec4, 4> localCorners = {
            glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),
            glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
            glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
            glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f)
        };

        // Get the transformation matrix
        glm::mat4 matrix = get_nested_transform();



        



        // Transform the local corners to world space
        std::array<glm::vec2, 4> worldCorners;
        for (size_t i = 0; i < 4; ++i)
        {
            glm::vec4 transformedCorner = matrix * localCorners[i];
            worldCorners[i] = glm::vec2(transformedCorner.x, transformedCorner.y);
        }

        // Find the min and max x and y coordinates
        float minX = worldCorners[0].x;
        float maxX = worldCorners[0].x;
        float minY = worldCorners[0].y;
        float maxY = worldCorners[0].y;

        for (size_t i = 1; i < 4; ++i)
        {
            minX = std::min(minX, worldCorners[i].x);
            maxX = std::max(maxX, worldCorners[i].x);
            minY = std::min(minY, worldCorners[i].y);
            maxY = std::max(maxY, worldCorners[i].y);
        }

        // Return the bounds as a glm::vec4 (minX, minY, maxX, maxY)
        return glm::vec4(minX, minY, maxX, maxY);
    }
    // Fix this later
    virtual void onCollide(void* details)
    {

    }
    
    // Returns a pointer to the child node that collides with this mouse position
    GUINode* collides(const glm::vec2& mousePos)
    {
        // Check if this node itself collides with the mouse position
        if (isInBounds(mousePos))
        {
            // Iterate through each child node to find a collision
            for (size_t i = 0; i < occupied_slots; ++i)
            {
                if (children[i] != nullptr)
                {
                    // Check if the child node collides with the mouse position
                    if (children[i]->isInBounds(mousePos))
                    {
                        // If it does, return the child node
                        return children[i];
                    }
                }
            }
            return this;
        }

        // Return nullptr if no collision is found
        return nullptr;
    }
    void swapColor()
    {
        glm::vec3 tempcolor = color;
        color = swap_color;
        swap_color = tempcolor;
        colliding = false;
    }
    private:
        glm::vec3 swap_color;
        inline bool isInBounds(const glm::vec2& position)
        {
            // Get the node's bounding box
            glm::vec4 bounds = getBounds();



            // Check if the mouse position is within the bounds
            return (position.x >= bounds.x && position.x <= bounds.z &&
                position.y >= bounds.y && position.y <= bounds.w);
        }
};



//class Function_Node