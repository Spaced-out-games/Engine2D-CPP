#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string> 

struct Shader
{
    GLuint ShaderProgram;

    Shader() : ShaderProgram(0) {

    }

    Shader(const char* vertexShaderSource, const char* fragmentShaderSource) {
        init(vertexShaderSource, fragmentShaderSource);
    }

    void init(const char* vertexShaderSource, const char* fragmentShaderSource)
    {
        ShaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
        std::cout << "2. Create a shader either from a file or from source code.\n";
    }

    ~Shader() {
        std::cout << "Destroyed shader with ID: " << ShaderProgram;
        if (ShaderProgram != 0) {
            glDeleteProgram(ShaderProgram);
        }
    }

    GLuint compileShader(GLenum ShaderType, const char* ShaderSource) {
        GLuint shader = glCreateShader(ShaderType);
        glShaderSource(shader, 1, &ShaderSource, nullptr);
        glCompileShader(shader);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader Compilation Error: " << infoLog << std::endl;
        }

        return shader;
    }

    GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::cerr << "Shader Program Linking Error: " << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    void use() const {
        #ifdef _DEBUG
        std::cout << "5. Use the shader associated with this Mesh3D by calling glUseProgram with the shader's program ID.\n";
        #endif
        glUseProgram(ShaderProgram);
    }

    void setUniform(const char* uniform_ID, const glm::mat4& matrix4) const {
        GLint uniformCode = glGetUniformLocation(ShaderProgram, uniform_ID);
        #ifdef _DEBUG
        if (uniformCode != -1) {
            std::cout << "6. Set any required uniforms.\n";
            glUniformMatrix4fv(uniformCode, 1, GL_FALSE, glm::value_ptr(matrix4));
        }
        else {
            std::cerr << "WARNING: Uniform '" << uniform_ID << "' not found in shader program " << ShaderProgram << std::endl;
        }
        #else
        glUniformMatrix4fv(uniformCode, 1, GL_FALSE, glm::value_ptr(matrix4));
        #endif
    }

    void setUniform(const char* uniform_ID, const glm::vec3& vector3) const {
        GLint uniformCode = glGetUniformLocation(ShaderProgram, uniform_ID);
        #ifdef _DEBUG
        if (uniformCode != -1) {
            std::cout << "6. Set any required uniforms.\n";
            glUniform3f(uniformCode, vector3.x, vector3.y, vector3.z);
        }
        else {
            std::cerr << "WARNING: Uniform '" << uniform_ID << "' not found in shader program " << ShaderProgram << std::endl;
        }
        #else
        glUniform3f(uniformCode, vector3.x, vector3.y, vector3.z);
        #endif
    }

    static inline const char* getDefaultVertexSource()
    {
        return DEFAULT_VTX_SHADER_SOURCE;
    }
    static inline const char* getDefaultFragmentSource()
    {
        return DEFAULT_FRAG_SHADER_SOURCE;
    }


private:
    static inline const char* DEFAULT_VTX_SHADER_SOURCE = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos; // Position attribute

    void main() {
        // Set gl_Position directly from the vertex position
        gl_Position = vec4(aPos, 0.0, 1.0); // 2D position with Z = 0.0 and W = 1.0
    }

    )";

    static inline const char* DEFAULT_FRAG_SHADER_SOURCE = R"(
    #version 330 core

    out vec4 FragColor; // Output color

    void main()
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // Set the color to white (RGBA)
    }


    )";
};

#endif
