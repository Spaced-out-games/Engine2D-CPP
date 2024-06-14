#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
// Contains an array of vertices. Simply initialize and call draw()
class Geometry {
public:
    // Create a Geometry from an array of XYZ formatted floats
    Geometry(const std::vector<float>& vertices);

    // Create a Geometry from an initializer list of XYZ formatted floats
    Geometry(std::initializer_list<GLfloat> vertex_data);
    
    // destructor
    ~Geometry();

    // Uploads geometry to the GPU
    void upload();

    // Gets the handle to the uploaded geometry in device VRAM
    GLuint getVao() const { return vao; }

    // gets Vertex count
    size_t getVertexCount() const { return vertices.size() / 3; } // Assuming 3D vertices
    
    // draws the geometry
    void draw() const;

    // Sets this geometry as the active VAO
    void bind() const;
private:
    GLuint vao;
    GLuint vbo;
    std::vector<float> vertices;
};
Geometry::Geometry(std::initializer_list<GLfloat> vertices) : vertices(vertices) {
    if (vertices.size() % 3 != 0) {
        throw std::invalid_argument("Vertex data must be a multiple of 3");
    }

    // Generate and bind VAO and VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    upload();
}

Geometry::Geometry(const std::vector<float>& vertices)
    : vertices(vertices), vao(0), vbo(0) {}

Geometry::~Geometry() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Geometry::upload() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Geometry::draw() const {
    //std::cout << "drawn";
    bind(); // Bind the VAO before drawing
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // Draw the geometry
}
void Geometry::bind() const {
    glBindVertexArray(vao); // Set the active VAO
}

#endif