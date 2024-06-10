#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
class Geometry {
public:
    Geometry(const std::vector<float>& vertices);
    Geometry(std::initializer_list<GLfloat> vertex_data);
    ~Geometry();
    void upload();
    GLuint getVao() const { return vao; }
    size_t getVertexCount() const { return vertices.size() / 3; } // Assuming 3D vertices
    void draw() const;
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
    std::cout << "drawn";
    bind(); // Bind the VAO before drawing
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // Draw the geometry
}
void Geometry::bind() const {
    glBindVertexArray(vao); // Set the active VAO
}

#endif