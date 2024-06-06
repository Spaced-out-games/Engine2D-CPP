#ifndef SHADER_H
#define SHADER_H
/*

#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <string>

class Shader
{
public:
	Shader(const std::string& source);
	~Shader();
	void use() const;
	void compile(GLenum Shader_type, const std::string& source);
	void link();

private:
	GLuint program_index;
};

Shader::Shader(const std::string& source)
{
}

Shader::~Shader()
{
}

void Shader::compile(GLenum shaderType, const std::string& source)
{
	GLuint shader = glCreateShader(shaderType);
	const char* sourcePtr = source.c_str();
	glShaderSource(shader, 1, &sourcePtr, nullptr);
	glCompileShader(shader);
	// Check compilation status and handle errors
	// (omitted for brevity)
	glAttachShader(program_index, shader);
	glDeleteShader(shader); // Shader can be deleted after attaching
}

void Shader::link()
{
	program_index = glCreateProgram();
	glLinkProgram(program_index);
}

void Shader::use() const
{
	glUseProgram(program_index);
}



*/
#endif
