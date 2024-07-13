//#define DEMO_DEBUG

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


// Function prototypes
//void initSDL(SDL_Window** window, SDL_GLContext* context);
//void initGL();
//tets
//#include "shader.h"
#include "Mesh2D.h"













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
    0.0f, 0.0f,  // Bottom-left
    1.0f, 0.0f,  // Bottom-right
    1.0f, -1.0f, // Top-right
    0.0f, -1.0f  // Top-left
    };

    GLuint indices[] = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };
    Shader shader_test = Shader::getDefaultShader();


    Mesh2D rect(vertices, 8, indices, 6);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
    transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));



    bool quit = false;
    SDL_Event event;

    float dt = 0.001f;
    glm::vec3 velocity(0.0005f, 0.0001f, 0.0);
    glm::mat4 transform2 = glm::mat4(1.0f);
    transform2 = glm::scale(transform2, glm::vec3(0.1f, 0.1f, 0.1f));
    //rect.ShaderProgram = ShaderProgram;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader_test.use();
        Shader::setUniform("transform", transform);
        Shader::setUniform("color", glm::vec3(1.0, 0.0, 0.0));
        rect.draw();
        transform2 = glm::rotate(transform2, glm::radians(dt), glm::vec3(0.0f, 0.0f, 1.0f));
        transform2 = glm::translate(transform2, velocity);

        //shader_test.use();
        Shader::setUniform("transform", transform2);
        Shader::setUniform("color", glm::vec3(1.0, 2.0, 0.0));
        rect.draw();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
#endif