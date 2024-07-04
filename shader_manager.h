#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include "basic_shader.h"
#include <glm/glm.hpp>         // Core GLM functionality
#include <glm/vec3.hpp>        // glm::vec3
#include <glm/mat4x4.hpp>      // glm::mat4
class shader_manager
{
public:
	shader_manager();
	~shader_manager();
	static basic_shader compile(const std::string& source, GLenum shader_type);
	static basic_shader compileFromFile(const std::string& path, GLenum shader_type);

    int addShader(basic_shader new_shader);
    int addShader(const std::string& source, GLenum shader_type);
    int addShader(const std::string& path, GLenum shader_type);
    void link(GLuint vertexShader, GLuint fragmentShader);


private:
    std::vector<shader> shaders;
};
shader_manager::shader_manager() {}

shader_manager::~shader_manager() {}

basic_shader shader_manager::compile(const std::string& source, GLenum type)
{
    GLuint shader_ID = glCreateShader(type);
    glShaderSource(shader_D, 1, &source, nullptr);
    glCompileShader(shader_ID);

    int success;
    glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(basic_shader, 512, nullptr, infoLog);
        std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
    }

    return basic_shader(shader_ID);
}

basic_shader shader_manager::compileFromFile(const std::string& path, GLenum type) {
    std::ifstream shaderFile(path);
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    std::string shaderCode = shaderStream.str();
    const char* shaderSource = shaderCode.c_str();

    GLuint shader_ID = glCreateShader(type);
    glShaderSource(shader_ID, 1, &shaderSource, nullptr);
    glCompileShader(shader_ID);

    int success;
    glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader_ID, 512, nullptr, infoLog);
        std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
    }

    return basic_shader(shader_ID);
}

int shader_manager::addShader(const std::string& source, GLenum shader_type)
{
    GLuint shader_ID = glCreateShader(type);
    glShaderSource(shader_D, 1, &source, nullptr);
    glCompileShader(shader_ID);

    int success;
    glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(basic_shader, 512, nullptr, infoLog);
        std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
    }
    shaders.push_back(basic_shader(shader_ID));
    return shaders.size() - 1;

}

int shader_manager::addShader(const std::string& path, GLenum shader_type)
{
    std::ifstream shaderFile(path);
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    std::string shaderCode = shaderStream.str();
    const char* shaderSource = shaderCode.c_str();

    GLuint shader_ID = glCreateShader(type);
    glShaderSource(shader_ID, 1, &shaderSource, nullptr);
    glCompileShader(shader_ID);

    int success;
    glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader_ID, 512, nullptr, infoLog);
        std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
    }

    shaders.push_back(basic_shader(shader_ID));
    return shaders.size() - 1;
}

void shader_manager::link(GLuint vertexShader, GLuint fragmentShader)
{
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

/*
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