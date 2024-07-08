#include <SDL.h>
#include <GL/glew.h>
#include <iostream>

// Function prototypes
void initSDL(SDL_Window** window, SDL_GLContext* context);
void initGL();
void render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Define the rectangle vertices
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    // Define the rectangle indices
    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Create and bind a Vertex Array Object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create and bind a Vertex Buffer Object (VBO)
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and bind an Element Buffer Object (EBO)
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Define the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Use the VAO to draw the rectangle
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Delete the VBO, VAO, and EBO
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}
int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_GLContext context;

    // Initialize SDL and OpenGL
    initSDL(&window, &context);
    initGL();

    // Main loop
    bool running = true;
    SDL_Event event;

    while (running) {
        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Render the scene
        render();

        // Swap buffers
        SDL_GL_SwapWindow(window);
    }

    // Clean up and exit
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void initSDL(SDL_Window** window, SDL_GLContext* context) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Set OpenGL version (here we use OpenGL 3.3)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create SDL window
    *window = SDL_CreateWindow("SDL OpenGL Rectangle",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Enable experimental features for core profile
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }

    // Set the swap interval for the current OpenGL context
    SDL_GL_SetSwapInterval(1); // Enable vsync
}

void initGL() {
    // Set the viewport
    glViewport(0, 0, 800, 600);

    // Set the clear color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}
