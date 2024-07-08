#pragma once
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
//#define SHADER_DEBUG

class Shader {
    friend class shader_manager;
public:
    void use() const {
        glUseProgram(programID);
    }

    static void setUniform(GLuint programID, const std::string& name, const glm::mat4& mat) {
        glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    static void setUniform(GLuint programID, const std::string& name, float value) {
        glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
    }

    GLuint getProgramID() const {
        return programID;
    }

private:
    GLuint programID;

    Shader(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string vertexCode, fragmentCode;
        if (!readShaderSource(vertexPath, vertexCode) || !readShaderSource(fragmentPath, fragmentCode)) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            return;
        }
        compile(vertexCode.c_str(), fragmentCode.c_str());
    }

    Shader(const std::string& shaderPath) {
        std::string vertexCode, fragmentCode;
        readAndSplitShaderSource(shaderPath, vertexCode, fragmentCode);
        compile(vertexCode.c_str(), fragmentCode.c_str());
    }

    static bool readShaderSource(const std::string& path, std::string& code) {
        std::ifstream shaderFile(path);
        if (!shaderFile.is_open()) {
            std::cerr << "Failed to open shader file: " << path << std::endl;
            return false;
        }
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        code = shaderStream.str();
        return true;
    }

    static void readAndSplitShaderSource(const std::string& shaderPath, std::string& vertexCode, std::string& fragmentCode) {
        std::string shaderCode;
        if (!readShaderSource(shaderPath, shaderCode)) return;
        const std::string vertexTag = "#vertex";
        const std::string fragmentTag = "#fragment";
        size_t vertexPos = shaderCode.find(vertexTag);
        size_t fragmentPos = shaderCode.find(fragmentTag);
        if (vertexPos != std::string::npos && fragmentPos != std::string::npos) {
            vertexCode = shaderCode.substr(vertexPos + vertexTag.length(), fragmentPos - (vertexPos + vertexTag.length()));
            fragmentCode = shaderCode.substr(fragmentPos + fragmentTag.length());
        }
        else {
            std::cerr << "Shader code format error: missing #vertex or #fragment tag." << std::endl;
        }
    }

    void compile(const char* vertexSource, const char* fragmentSource) {
        GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
        GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);
        checkCompileErrors(programID, "PROGRAM");
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    static GLuint compileShader(const char* source, GLenum type) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        #ifdef SHADER_DEBUG
        std::cout
            << "------------------------------------------------SOURCE BEGIN------------------------------------------------\n"
            << source << '\n'
            << "------------------------------------------------SOURCE  END ------------------------------------------------\n";
        #endif
        checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
        return shader;
    }

    static void checkCompileErrors(GLuint object, const std::string& type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(object, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(object, 1024, NULL, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else {
            glGetProgramiv(object, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(object, 1024, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};


#endif
