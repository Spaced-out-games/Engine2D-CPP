#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

class Mesh {
public:
    Mesh();
    Mesh(const float* vertices, size_t vertexSize, const unsigned int* indices, size_t indexSize);
    ~Mesh();

    // Draw a single instance
    void draw(const glm::mat4& transform) const;

    // Draw multiple instances
    void draw(const glm::mat4* transforms, int num_transforms) const;

    void update_vertices(const float* vertices, size_t vertexSize);

    void update_indices(const unsigned int* indices, size_t indexSize);



    static void markInitialized() { OGL_initialized = true; }
private:
    GLuint VAO, VBO, EBO, instanceVBO;
    static bool OGL_initialized;

    size_t vertex_array_size;
    size_t index_array_size;
};
// Initialize the static member variable
bool Mesh::OGL_initialized = false;

// Default constructor
Mesh::Mesh()
    : VAO(0), VBO(0), EBO(0), instanceVBO(0), vertex_array_size(0), index_array_size(0) {
    // Check if OpenGL has been initialized
    if (!OGL_initialized) {
        std::cerr << "OpenGL has not been initialized before creating a Mesh object." << std::endl;
    }
}

// Constructor with vertices and indices
Mesh::Mesh(const float* vertices, size_t vertexSize, const unsigned int* indices, size_t indexSize)
    : VAO(0), VBO(0), EBO(0), instanceVBO(0), vertex_array_size(vertexSize), index_array_size(indexSize) {
    if (!OGL_initialized) {
        std::cerr << "OpenGL has not been initialized before creating a Mesh object." << std::endl;
        return;
    }

    // Create and bind the VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create and bind the VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

    // Create and bind the EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);

    // Assuming a simple vertex format with position and possibly other attributes.
    // Change the stride and attribute pointers as needed.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO
    glBindVertexArray(0);
}

// Destructor
Mesh::~Mesh() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
    if (instanceVBO) glDeleteBuffers(1, &instanceVBO);
}

// Draw a single instance
void Mesh::draw(const glm::mat4& transform) const {
    glBindVertexArray(VAO);

    // Upload the transformation matrix (assuming a uniform is already bound in the shader)
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    return static_cast<GLuint>(currentProgram);

    GLint transformLoc = glGetUniformLocation(currentProgram, "model"); // Replace 'shaderProgram' with your actual shader program
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);

    // Draw the mesh
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_array_size / sizeof(unsigned int)), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

// Draw multiple instances
void Mesh::draw(const glm::mat4* transforms, int num_transforms) const {
    glBindVertexArray(VAO);

    // Assuming instanced rendering with instanceVBO for transforms
    if (instanceVBO == 0) {
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, num_transforms * sizeof(glm::mat4), transforms, GL_STATIC_DRAW);

        for (int i = 0; i < 4; i++) {
            glEnableVertexAttribArray(1 + i);
            glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
            glVertexAttribDivisor(1 + i, 1); // One per instance
        }
    }
    else {
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, num_transforms * sizeof(glm::mat4), transforms);
    }

    // Draw the mesh with instancing
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(index_array_size / sizeof(unsigned int)), GL_UNSIGNED_INT, 0, num_transforms);

    glBindVertexArray(0);
}

// Update vertices
void Mesh::update_vertices(const float* vertices, size_t vertexSize) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
    vertex_array_size = vertexSize;
}

// Update indices
void Mesh::update_indices(const unsigned int* indices, size_t indexSize) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);
    index_array_size = indexSize;
}