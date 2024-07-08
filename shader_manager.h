#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shader.h"

// Shader manager class that manages multiple shader programs
class shader_manager
{
public:
    // Load a shader program from vertex and fragment shader files
    static int load_shader(const std::string& fragmentPath, const std::string& vertexPath);

    // Load a shader program from a single file with embedded vertex and fragment shaders
    static int load_shader(const std::string& shaderPath);

    // Set the current shader program to use by its ID
    static void use(int shaderID);

    // Get the Shader object by ID
    static Shader* get_shader(int shaderID);

    // Set uniform matrix for the currently active shader
    static void setUniform(const std::string& name, const glm::mat4& mat);

    // Set uniform float for the currently active shader
    static void setUniform(const std::string& name, float value);

    // Retrieve the singleton instance of shader_manager
    static shader_manager& getInstance();
    //static void make_instance() { master_shader_manager = shader_manager(); }
    ~shader_manager() = default;

private:
    shader_manager() = default;
    

    std::vector<Shader> shaders;
    int currentShaderID = -1;

    static shader_manager master_shader_manager;
};
// Static member initialization
shader_manager shader_manager::master_shader_manager = shader_manager();

int shader_manager::load_shader(const std::string& fragmentPath, const std::string& vertexPath) {
    Shader newShader(vertexPath, fragmentPath);
    int id = static_cast<int>(shader_manager::getInstance().shaders.size());
    shader_manager::getInstance().shaders.push_back(newShader);
    return id;
}

int shader_manager::load_shader(const std::string& shaderPath) {
    Shader newShader(shaderPath);
    int id = static_cast<int>(shader_manager::getInstance().shaders.size());
    shader_manager::getInstance().shaders.push_back(newShader);
    return id;
}

void shader_manager::use(int shaderID) {
    if (shaderID >= 0 && shaderID < shader_manager::getInstance().shaders.size()) {
        shader_manager::getInstance().shaders[shaderID].use();
        shader_manager::getInstance().currentShaderID = shaderID;
    }
    else {
        std::cerr << "Invalid shader ID: " << shaderID << std::endl;
    }
}

Shader* shader_manager::get_shader(int shaderID) {
    if (shaderID >= 0 && shaderID < shader_manager::getInstance().shaders.size()) {
        return &shader_manager::getInstance().shaders[shaderID];
    }
    else {
        std::cerr << "Invalid shader ID: " << shaderID << std::endl;
        return nullptr;
    }
}

void shader_manager::setUniform(const std::string& name, const glm::mat4& mat) {
    Shader* shader = shader_manager::get_shader(shader_manager::getInstance().currentShaderID);
    if (shader) {
        Shader::setUniform(shader->getProgramID(), name, mat);
    }
}

void shader_manager::setUniform(const std::string& name, float value) {
    Shader* shader = shader_manager::get_shader(shader_manager::getInstance().currentShaderID);
    if (shader) {
        Shader::setUniform(shader->getProgramID(), name, value);
    }
}

shader_manager& shader_manager::getInstance() {
    return master_shader_manager;
}
#endif // SHADER_MANAGER_H

