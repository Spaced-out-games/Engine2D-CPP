#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <initializer_list>

class Geometry {
public:
    // Create a Geometry from an array of XYZ formatted floats
    Geometry(const std::vector<float>& vertices);

    // Create a Geometry from an initializer list of XYZ formatted floats
    Geometry(std::initializer_list<GLfloat> vertex_data);

    // destructor
    ~Geometry();


    // Gets the handle to the uploaded Geometry in device VRAM
    GLuint getVao() const { return vao; }

    // gets Vertex count
    size_t getVertexCount() const { return vertexCount / 3; } // Assuming 3D vertices

    // draws the Geometry
    void draw() const;

    // Sets this Geometry as the active VAO
    void bind() const;
private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo; // not implemented

    unsigned int vertexCount;

    // Uploads Geometry to the GPU via an initializer list. Both are called on initiation

    void upload(std::vector<GLfloat> vertices);
};



Geometry::Geometry(std::initializer_list<GLfloat> vertex_data) {
    vao = 0;
    vbo = 0;
    if (vertexCount % 3 != 0) {
        throw std::invalid_argument("Vertex data must be a multiple of 3");
    }

    // Generate and bind VAO and VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    upload(vertex_data);
}

Geometry::~Geometry()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}


void Geometry::upload(std::vector<GLfloat> vertices)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Geometry::draw() const {
    bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Geometry::bind() const {
    glBindVertexArray(vao);
}





#endif // !Geometry_H
