#include <string>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>         // Core GLM functionality
#include <glm/vec3.hpp>        // glm::vec3
#include <glm/mat4x4.hpp>      // glm::mat4


class shader
{
public:

    shader(GLuint program_ID, GLuint fragment_ID, GLuint vertex_ID) : mProgram_ID(program_ID) {}
    
    ~shader() { glDeleteProgram(mProgram_ID); }

    void use() const { glUseProgram(mProgram_ID); }

    void setUniform(const std::string& name, int value) {
        GLint location = glGetUniformLocation(mProgram_ID, name.c_str());
        if (location != -1) {
            glUniform1i(location, value);
        }
    }

    void setUniform(const std::string& name, float value) {
        GLint location = glGetUniformLocation(mProgram_ID, name.c_str());
        if (location != -1) {
            glUniform1f(location, value);
        }
    }

    void setUniform(const std::string& name, const glm::vec3& value) {
        GLint location = glGetUniformLocation(mProgram_ID, name.c_str());
        if (location != -1) {
            glUniform3fv(location, 1, &value[0]);
        }
    }

    void setUniform(const std::string& name, const glm::mat4& value) {
        GLint location = glGetUniformLocation(mProgram_ID, name.c_str());
        if (location != -1) {
            glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
        }
    }


private:
    GLuint mProgram_ID;


};
