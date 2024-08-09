#define DEMO_DEBUG


#ifdef DEMO_DEBUG
#define SDL_MAIN_HANDLED
#include <GL/glew.h>
#include <iostream>
#include "Application.h"
#include "shader.h"
#include "Mesh2D.h"



// Define the Mesh2D instance
typedef Mesh2D<GL_STATIC_DRAW, GL_TRIANGLES> StaticMesh2D;

struct windowContent
{
    Shader shader;
    StaticMesh2D rectangle;
};

// Bootstrap function (optional setup before the main loop)
void custom_bootstrap(void* input)
{
    Application<windowContent>& app = *((Application<windowContent>*)input);

    app.getCustomState()->shader.init(Shader::getDefaultVertexSource(), Shader::getDefaultFragmentSource());
    // Vertex and index data for a rectangle
    GLfloat rect_vertices[] = {
        0.5f,  0.5f,
       -0.5f,  0.5f,
       -0.5f, -0.5f,
        0.5f, -0.5f
    };

    GLuint rect_indices[] = {
        0, 1, 2, // First triangle
        2, 3, 0  // Second triangle
    };

    std::cout << "3. Prepare your vertex data, which can involve loading from a file or creating in memory.\n";

    // Ensure the shader is properly initialized
    CHECK_GL_ERROR();

    // Initialize Mesh2D and assign it to custom_state
    app.getCustomState()->rectangle.init(rect_vertices, sizeof(rect_vertices) / sizeof(GLfloat), rect_indices, sizeof(rect_indices) / sizeof(GLuint)); // Initialize Mesh2D

    app.getCustomState()->shader.use();

}


// Tick function (called every frame)
void custom_tick(void* input)
{
    Application<windowContent>& app = *((Application<windowContent>*)input);

   
    app.getCustomState()->rectangle.printStatus();
    //app->getCustomState()->shader.setUniform("color", glm::vec3(1, 1, 1)); // Set color uniform
    app.getCustomState()->rectangle.draw(); // Draw the rectangle
}

int main(int argc, char* argv[])
{


    Application<windowContent> app(argc, argv); // Initializes SDL window & context

    // Initialize windowContent with the Application instance
    //windowContent content;



    app.set_bootstrap_function(custom_bootstrap);
    app.set_tick_function(custom_tick);
    // Run the main loop
    app.run();

    return 0;
}


#endif