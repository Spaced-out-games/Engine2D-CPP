#pragma once
#include <GL/glew.h>
#include <iostream>

#include "debug_utils.h"


/**
 * Represents a buffer of vertices in VRAM.
 */
template <typename T, size_t VertexSize, GLenum DrawingType>
class vBuffer {
public:
    GLuint VBO;

    vBuffer() : VBO(0) {}

    void init(const T* vertices, size_t vertex_count) {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * VertexSize * sizeof(T), vertices, DrawingType);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }

    void unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ~vBuffer() {
        if (VBO != 0) {
            glDeleteBuffers(1, &VBO);
        }
    }
    bool isBound() const {
        GLint currentBuffer = 0;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentBuffer);
        return (static_cast<GLuint>(currentBuffer) == VBO);
    }
    GLuint getID() const {
        return VBO;
    }
};
