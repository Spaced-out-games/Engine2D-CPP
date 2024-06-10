#ifndef SHADER_LIBRARY_H
#define SHADER_LIBRARY_H
#pragma once
#include "shader.h"
#include <GL/glew.h>
#include <SDL.h>
#include <vector>


class ShaderLibrary {
public:
    void loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    Shader& getShader(const std::string& name);
    void useShader(const std::string& name);

private:
    std::unordered_map<std::string, Shader> shaders;
};
void ShaderLibrary::loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
    shaders.emplace(name, Shader(vertexPath, fragmentPath));
}

Shader& ShaderLibrary::getShader(const std::string& name) {
    return shaders.at(name);
}

void ShaderLibrary::useShader(const std::string& name) {
    shaders.at(name).use();
}
#endif