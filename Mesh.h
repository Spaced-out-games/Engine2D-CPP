#pragma once
#include <GL/glew.h>
#include <iostream>
#include "eBuffer.h"
#include "vBuffer.h"
#include "vArray.h"

#include "debug_utils.h"
/**
To use Mesh:
1. Set up SDL and OpenGL context.
2. Create a shader either from a file or from source code. //
3. Prepare your vertex data, which can involve loading from a file or creating in memory.
4. Pass the vertex data and shader to the Mesh3D constructor. The geometry will be set up at this point.
To draw:
5. Use the shader associated with this Mesh3D by calling <see cref="glUseProgram"/> with the shader's program ID.
6. Set any required uniforms.
7. Call draw() to render the mesh.
*/
#ifndef Mesh_H
#define Mesh_H

/**
 * Mesh handles 2D mesh rendering. Assumes each vertex has 2 components (for positions).
 */

#pragma once
#include <GL/glew.h>
#include <iostream>
#include "vArray.h"  // Make sure to include the VertexArray class
#include "vBuffer.h"
#include "eBuffer.h"

 /**
  * Mesh handles 2D mesh rendering. Assumes each vertex has 2 components (for positions).
  */
template <GLenum DrawingType, GLenum PrimitiveType>
class Mesh {
public:
    vArray VAO;  // Use VertexArray class to manage VAO
    vBuffer<GLfloat, 2, DrawingType> VBO; // Vertex size is fixed to 2 (x, y)
    eBuffer<GLuint, DrawingType, PrimitiveType> EBO;

    Mesh() {}

    Mesh(const GLfloat* vertices, size_t vertex_count, const GLuint* indices, size_t index_count)
        : VBO(), EBO() {
        init(vertices, vertex_count, indices, index_count);
        

    }

    ~Mesh() {
        // VertexArray destructor will handle VAO cleanup
    }

    void init(const GLfloat* vertices, size_t vertex_count, const GLuint* indices, size_t index_count) {

        #ifdef _DEBUG
        std::cout << "4. Pass the vertex data and shader to the Mesh constructor. The geometry will be set up at this point\n";
        #endif

        VAO.init();
        VAO.bind();

        VBO.init(vertices, vertex_count);
        VBO.bind();

        EBO.init(indices, index_count);
        EBO.bind();

        printStatus();
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0); // Assuming 2 components per vertex
        glEnableVertexAttribArray(0);




    }

    void printStatus()
    {
        std::cout << "vao: " << VAO.getID() << '\n';
        std::cout << "vbo: " << VBO.getID() << '\n';
        std::cout << "ebo: " << EBO.getID() << '\n';

    }

    void draw() {
        printStatus();
        #ifdef _DEBUG
        std::cout << "7. Drawing Mesh...\n";
        printStatus();
        #endif


        



        glDrawElements(PrimitiveType, static_cast<GLsizei>(EBO.index_count), EBO.getIndexType(), 0);


        printStatus();
    }
};


#endif
