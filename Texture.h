#pragma once
#include <GL/glew.h> // For OpenGL functions and types
#include <SDL.h> // For SDL functions
#include <iostream> // For error handling

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
    GLuint ID;
    GLenum type;

    Texture(GLenum textureType = GL_TEXTURE_2D) : type(textureType) {
        glGenTextures(1, &ID);
        glBindTexture(type, ID);

        // Set texture wrapping parameters
        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Generate a simple 1x1 texture with a solid color (e.g., red)
        unsigned char data[4] = { 255, 0, 0, 255 }; // RGBA for red color
        glTexImage2D(type, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(type);
    }

    void bind(GLuint textureUnit) const {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(type, ID);
    }
};

#endif