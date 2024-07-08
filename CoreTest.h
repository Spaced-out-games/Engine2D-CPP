#pragma once
#include "engineCore.h"
#include "Mesh.h"
#include "MeshFactory.h"
#include "shader_manager.h"
class coreTest : public engineCore {
public:
    coreTest();
    ~coreTest() {}

    void bootstrap() override;
    void graphics_tick() override;
    void register_Meshes();
    GLuint shaderProgram;
private:
    Mesh testMesh;
    
};
#include <iostream>
coreTest::coreTest()// testMesh = Mesh();
{
    testMesh = Mesh();
}
void coreTest::register_Meshes()
{
    getManager().add_array<Mesh>();
    getManager().get_array<Mesh>().insert(MeshFactory::MeshFactory2D::createRectangle());

}


// Load or compile shaders as needed. This is a placeholder.
GLuint loadShaders() {
    // Replace this with actual shader compilation and linking code.
    return 0; // Placeholder shader program ID.
}

void coreTest::bootstrap() {
    // Initialize shaders
    shaderProgram = loadShaders();
    
    getShaderManager().load_shader("basic_shader.glsl");
    getShaderManager().setUniform("model", glm::mat4(1.0));
    getShaderManager().setUniform("view", glm::mat4(1.0));
    getShaderManager().setUniform("projection", glm::mat4(1.0));
    getShaderManager().setUniform("parentTransform", glm::mat4(1.0));
    // Create a mesh using the factory

    testMesh = MeshFactory::MeshFactory3D::createCube(0.5f); // Example: creating a cube with size 1.0

    // Set up OpenGL state as needed
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Background color
    Mesh::markInitialized();
}

void coreTest::graphics_tick() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    getShaderManager().use(shaderProgram);

    // Render the mesh
    testMesh.draw(glm::mat4(1.0)); // Assumes the Mesh class has a draw method

    // Swap buffers
    SDL_GL_SwapWindow(getWindow());
}
