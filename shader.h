#ifndef SHADER_H
#define SHADER_H


#pragma once

#include <string>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
class Shader
{
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void use() const;

		GLuint getProgram() const { return program; }
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

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
    }

    return shader;
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

#endif
