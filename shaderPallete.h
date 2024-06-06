#ifndef SHADER_PALLETE_H
#define SHADER_PALLETE_H
#pragma once
//#include "shader.h"
#include <GL/glew.h>
#include <SDL.h>
#include <vector>

class ShaderPallete
{
public:
	ShaderPallete(size_t initialCapacity = 64);
	~ShaderPallete();
	size_t add_program(const std::string& vertex_source, const std::string& fragment_source);
	void use_shader(size_t index);

private:
	std::vector<GLuint> shader_IDs;

	GLuint compile(GLenum shaderType, const std::string& source);
};

ShaderPallete::ShaderPallete(size_t initialCapacity)
{
	shader_IDs.reserve(initialCapacity);
}

ShaderPallete::~ShaderPallete()
{
}
size_t ShaderPallete::add_program( const std::string& vertex_source, const std::string& fragment_source)
{
	GLuint program = glCreateProgram();

	if (!vertex_source.empty())
	{
		GLuint vtx_shader = compile(GL_VERTEX_SHADER, vertex_source);
		glAttachShader(program, vtx_shader);
		glDeleteShader(vtx_shader);
	}

	if (!fragment_source.empty())
	{
		GLuint frag_shader = compile(GL_VERTEX_SHADER, fragment_source);
		glAttachShader(program, frag_shader);
		glDeleteShader(frag_shader);
	}

	glLinkProgram(program);
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		// Handle linking error (log it, throw an exception, etc.)
	}

	// Store the program's ID and return its index
	shader_IDs.push_back(program);
	return shader_IDs.size() - 1;

}

void ShaderPallete::use_shader(size_t index)
{
	glUseProgram(shader_IDs.at(index));
}

GLuint ShaderPallete::compile(GLenum shaderType, const std::string& source)
{
	GLuint shader = glCreateShader(shaderType);
	const char* sourcePtr = source.c_str();
	glShaderSource(shader, 1, &sourcePtr, nullptr);
	glCompileShader(shader);

	// Check for compilation errors
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		// Handle compilation error (log it, throw an exception, etc.)
	}

	return shader;
}

#endif