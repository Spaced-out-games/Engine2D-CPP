#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>         // Core GLM functionality
#include <glm/vec3.hpp>        // glm::vec3
#include <glm/mat4x4.hpp>      // glm::mat4

// This go around, I want a Shader to be compiled by a shaderManager rather than an instance of a basic_shader, both from source directly, and from a file
// shaderManager is supposed to functionmore or less like a singleton with a public static interface to get a basic_shader
// This means that Shaders should be more or less structs with constructors and destructors, as well as interfaces to set uniforms.
// Linking them in the GPU is handled by shaderManager when it goes out of scope, but shaderManager will also provide an interface to delete directly

// So in totality:


class basic_shader
{
public:

    basic_shader(GLuint program_ID, GLuint fragment_ID, GLuint vertex_ID) : mShader_ID(program_ID) {}

    ~basic_shader() { glDeleteProgram(mShader_ID); }

    void use() const { glUseProgram(mShader_ID); }

    void setUniform(const std::string& name, int value) {
        GLint location = glGetUniformLocation(mShader_ID, name.c_str());
        if (location != -1) {
            glUniform1i(location, value);
        }
    }

    void setUniform(const std::string& name, float value) {
        GLint location = glGetUniformLocation(mShader_ID, name.c_str());
        if (location != -1) {
            glUniform1f(location, value);
        }
    }

    void setUniform(const std::string& name, const glm::vec3& value) {
        GLint location = glGetUniformLocation(mShader_ID, name.c_str());
        if (location != -1) {
            glUniform3fv(location, 1, &value[0]);
        }
    }

    void setUniform(const std::string& name, const glm::mat4& value) {
        GLint location = glGetUniformLocation(mShader_ID, name.c_str());
        if (location != -1) {
            glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
        }
    }


private:
    GLuint mShader_ID;
   // GLuint mFragment_ID;
    GLuint mVertex_ID;

};







/*
class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use() const;

    GLuint getProgram() const { return program; }

    //  void setUniform(const std::string& name, int value);

    //  void setUniform(const std::string& name, float value);

     // void setUniform(const std::string& name, const glm::vec3& value);

     // void setUniform(const std::string& name, const glm::mat4& value);
private:
    GLuint program;

    GLuint compile(const std::string& sourcePath, GLenum type);

    void link(GLuint vertex_shader, GLuint fragment_shader);
};

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    GLuint vertexShader = compile(vertexPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = compile(fragmentPath, GL_FRAGMENT_SHADER);
    link(vertexShader, fragmentShader);
}





Shader::~Shader() {
    glDeleteProgram(program);
}

void Shader::use() const {
    glUseProgram(program);
}

GLuint Shader::compile(const std::string& sourcePath, GLenum type) {
    std::ifstream shaderFile(sourcePath);
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    std::string shaderCode = shaderStream.str();
    const char* shaderSource = shaderCode.c_str();

    GLuint basic_shader = glCreateShader(type);
    glShaderSource(basic_shader, 1, &shaderSource, nullptr);
    glCompileShader(basic_shader);

    int success;
    glGetShaderiv(basic_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(basic_shader, 512, nullptr, infoLog);
        std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
    }

    return basic_shader;
}

void Shader::link(GLuint vertexShader, GLuint fragmentShader) {
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Error: Program linking failed\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
*/
#endif