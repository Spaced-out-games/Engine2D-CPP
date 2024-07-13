#define DEMO_DEBUG

#ifndef DEMO_DEBUG
#include "ent_manager.h"
#include <iostream>
#include <random>
#include "engineCore.h"




int main(int argc, char* argv[]) {
    if (!engineCore::init_core(argc, argv)) {
        std::cerr << "Failed to initialize engine core." << std::endl;
        return EXIT_FAILURE;
    }

    // Directly create an instance of coreTest
    engineCore* testEngine = engineCore::getEngineCore();

    // If needed, initialize the instance or set it as the core engine instance
    // Note: Ensure you handle engineCore static instance management correctly
    //engineCore::getEngineCore() = testEngine; // This line is usually not needed if you don't need to replace the core engine instance
    //engineCore::bootstrap

    int result = testEngine->main();
    return result;
}
#endif

#ifdef DEMO_DEBUG
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 transform;
void main() {
    gl_Position = transform * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core

in vec3 vertexColor; // Input from vertex shader
out vec4 FragColor;

uniform vec3 color; // Color uniform

void main() {
    FragColor = vec4(color, 1.0); // Use the uniform color
}
)";

// Function prototypes
//void initSDL(SDL_Window** window, SDL_GLContext* context);
//void initGL();

// This works, do not fucking touch it, dumbass
struct model
{
    GLuint VBO, VAO, EBO;
    GLuint shaderProgram;

    model(GLfloat* vertices, size_t vertices_size, GLuint* indices, size_t indices_size, GLuint shaderProgram)
        : shaderProgram(shaderProgram)
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~model()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void draw(glm::mat4& transform)
    {
        GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }
};

GLuint compileShader(GLenum shaderType, const char* shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
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

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader Program Linking Error: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void render(model& m, glm::vec3 color, glm::mat4& transform) {
    
    glUseProgram(m.shaderProgram);

    GLint colorLoc = glGetUniformLocation(m.shaderProgram, "color");
    glUniform3f(colorLoc, color.r, color.g, color.b);
    
    GLint transformLoc = glGetUniformLocation(m.shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    m.draw(transform);
}

void initSDL(SDL_Window** window, SDL_GLContext* context) {
    // Initialize SDL. In order
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Set OpenGL version (here we use OpenGL 3.3). In order
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // in order
    // Create SDL window
    *window = SDL_CreateWindow("SDL OpenGL Rectangle",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    // Create an SDL window. In order
    if (!*window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Create OpenGL context
    *context = SDL_GL_CreateContext(*window);
    if (!*context) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Initialize GLEW. In order
    glewExperimental = GL_TRUE; // Enable experimental features for core profile
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }

    // Set the swap interval for the current OpenGL context. Present
    SDL_GL_SetSwapInterval(1); // Enable vsync
}

void initGL() {
    // Set the viewport
    glViewport(0, 0, 800, 600);

    // Set the clear color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}
int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_GLContext context;
    initSDL(&window, &context);
    initGL();


    GLfloat vertices[] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    model rect(vertices, 12, indices, 6, shaderProgram);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
    transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));

    bool quit = false;
    SDL_Event event;
    

    rect.shaderProgram = shaderProgram;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        render(rect, glm::vec3(1.0, 0.0f, 0.0f), transform);
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
#endif