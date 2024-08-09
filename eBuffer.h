#pragma once
#include <GL/glew.h>
#include <iostream>


#include "debug_utils.h"

template <typename IndexType, GLenum DrawingType, GLenum PrimitiveType>
class eBuffer {
public:
    GLuint EBO;
    size_t index_count;

    eBuffer() : EBO(0) {}

    void init(const IndexType* indices, size_t count) {
        index_count = count;
        glGenBuffers(1, &EBO);
        CHECK_GL_ERROR();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(IndexType), indices, DrawingType);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    }

    GLenum getIndexType() const {
        return sizeof(IndexType) == 4 ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
    }

    void bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }

    void unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ~eBuffer() {
        if (EBO != 0) {
            glDeleteBuffers(1, &EBO);
        }
    }
    bool isBound() const {
        GLint currentBuffer = 0;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentBuffer);
        return (static_cast<GLuint>(currentBuffer) == EBO);
    }
    GLuint getID() const {
        return EBO;
    }
    size_t size() const
    {
        return index_count;
    }
};

