#pragma once

#ifndef MESH3D_H
#include "Mesh2D.h"
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "engineCore.h"
//class engineCore;
// Have GUI_Nodes query a slot within an hb_manager upon construction. This allows all the hitboxes to be in parallel, which is good for performance


class GUINode
{
public:
    GUINode* parent;
    GUINode** children; // Fixed-size array for children
    inline static Mesh2D* rectangle = nullptr;
    size_t child_count;
    size_t occupied_slots;
    glm::vec3 translation;
    glm::vec3 scale;
    glm::vec3 color;
    float rotation;

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
        translation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), color(1.0f, 1.0f, 1.0f)
    {
        rotation = 0.0f;
        children = new GUINode * [child_count];
        for (size_t i = 0; i < child_count; ++i)
        {
            children[i] = nullptr;
        }
    }

    GUINode(glm::vec3 initial_translation, glm::vec3 initial_scale, glm::vec3 initial_color, size_t max_children)
        : parent(nullptr), child_count(max_children), occupied_slots(0),
        translation(initial_translation), scale(initial_scale), color(initial_color)
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
        transform = glm::scale(transform, scale);
        #ifdef _DEBUG
        std::cout << "6. Set any required uniforms.";
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
        if (parent != nullptr)
        {
            return parent->get_nested_transform() * rel_transform;
        }
        else
        {
            return rel_transform;
        }
    }
};



//class Function_Node