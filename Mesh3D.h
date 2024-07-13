#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Since you're going to need a shader, might as well include it.
#ifndef SHADER_H
#include "shader.h"
#endif

#ifndef MESH2D_H
#define MESH2D_H
/// <summary>
/// Represents a 3D mesh for rendering with OpenGL.
/// </summary>
/// <remarks>
/// To use Mesh3D:
/// 1. Set up SDL and OpenGL context.
/// 2. Create a shader either from a file or from source code.
/// 3. Prepare your vertex data, which can involve loading from a file or creating in memory.
/// 4. Pass the vertex data and shader to the Mesh3D constructor. The geometry will be set up at this point.
///
/// To draw:
/// 5. Use the shader associated with this Mesh3D by calling <see cref="glUseProgram"/> with the shader's program ID.
/// 6. Set any required uniforms.
/// 7. Call <see cref="Draw"/> to render the mesh.
/// </remarks>
struct Mesh3D
{
    // Vertex buffer
    GLuint VBO;

    // Buffer that stores the state of the Mesh
    GLuint VAO;

    // Index buffer
    GLuint EBO;

    // Pointer to the shader this Mesh is using
    Shader* m_shader;

    Mesh3D(GLfloat* vertices, size_t vertices_size, GLuint* indices, size_t indices_size, Shader shader) : m_shader(shader)
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~Mesh3D()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void draw()//glm::mat4& transform)
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }

};

#endif