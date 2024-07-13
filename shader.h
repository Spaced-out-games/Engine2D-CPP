#include <GL/glew.h>       // For OpenGL functions and types
#include <iostream>        // For std::cerr
#include <string>          // For std::string (if you use strings for shader source)

#ifndef SHADER_H
#define SHADER_H
struct Shader
{
    // for now, everything is public
    GLuint ShaderProgram;
    Shader(const char* vertexShaderSource, const char* fragmentShaderSource) { ShaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource); }

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

    // Function to create shader program
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

    void use() const
    {
        glUseProgram(ShaderProgram);
    }
    static void setUniform(const char* uniform_ID, const glm::mat4& matrix4)
    {
        GLint program_int;
        glGetIntegerv(GL_CURRENT_PROGRAM, &program_int);
        GLuint program_uint = static_cast<GLuint>(program_int);
        GLint uniformCode = glGetUniformLocation(program_uint, uniform_ID);

        #ifdef _DEBUG
        if (uniformCode != -1) // Check if the uniform location is valid
        {
            glUniformMatrix4fv(uniformCode, 1, GL_FALSE, glm::value_ptr(matrix4));
        }
        else
        {
            std::cerr << "Warning: Uniform '" << uniform_ID << "' not found in shader program " << program_uint << std::endl;
        }
        #else
        // In release builds, just set the uniform without checking
        glUniformMatrix4fv(uniformCode, 1, GL_FALSE, glm::value_ptr(matrix4));
        #endif
    }

    static void setUniform(const char* uniform_ID, const glm::vec3& vector3)
    {
        GLint program_int;
        glGetIntegerv(GL_CURRENT_PROGRAM, &program_int);
        GLuint program_uint = static_cast<GLuint>(program_int);
        GLint uniformCode = glGetUniformLocation(program_uint, uniform_ID);

        #ifdef _DEBUG
        if (uniformCode != -1) // Check if the uniform location is valid
        {
            glUniform3f(uniformCode, vector3.x, vector3.y, vector3.z);
        }
        else
        {
            std::cerr << "Warning: Uniform '" << uniform_ID << "' not found in shader program " << program_uint << std::endl;
        }
        #else
        // In release builds, just set the uniform without checking
        glUniform3f(uniformCode, vector3.x, vector3.y, vector3.z);
        #endif
    }
    static Shader getDefaultShader() { static Shader DEFAULT_SHADER(DEFAULT_VTX_SHADER_SOURCE, DEFAULT_FRAG_SHADER_SOURCE); return DEFAULT_SHADER; }

private:
    static inline const char* DEFAULT_VTX_SHADER_SOURCE = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    uniform mat4 transform;
    void main() {
        gl_Position = transform * vec4(aPos, 1.0);
    }
    )";

    static inline const char* DEFAULT_FRAG_SHADER_SOURCE = R"(
        #version 330 core

        in vec3 vertexColor; // Input from vertex Shader
        out vec4 FragColor;

        uniform vec3 color; // Color uniform

        void main() {
            FragColor = vec4(color, 1.0); // Use the uniform color
        }
    )";

};




#endif