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

#include "engineCore.h"
#include "GUI_Node.h"
#include "shader.h"
#include "shader_manager.h"
using glm::vec3;
using glm::mat4;

struct windowContent
{
    std::vector<GUINode> nodes;
    std::vector<std::string> argv;
    Shader shader;


    // Default constructor
    windowContent() = default; // Use default constructor for the default case
};

template <class state_t>
void custom_bootstrap(void* input)
{
    // Cast to an application pointer
    Application<state_t>* app = (Application<state_t>*)input;

    // set the shader
    app->getCustomState()->shader = (Shader::getDefaultShader());


    // Create the geometry
    GUINode::init();
    app->getCustomState()->nodes.push_back(GUINode(vec3(0.0f,0.0f,0.0f),vec3(1.0f,1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), 0));
}


template <class state_t>
void custom_tick(void* input)
{
    // cast to application pointer
    Application<state_t>* app = (Application<state_t>*)input;

    // use the shader
    app->getCustomState()->shader.use(); 

    // draw the node
    app->getCustomState()->nodes[0].draw();
}


#include <iostream>

int main(int argc, char* argv[])
{
    Application<windowContent> app (argc, argv); // initializes SDL window & context
    windowContent content; // Adds arguments to the state
    app.setCustomState(content); // assigns state



    // set but not call functions
    app.set_bootstrap_function(custom_bootstrap<windowContent>);
    app.set_tick_function(custom_tick<windowContent>);

    // Run the main loop
    app.run();
}



/*
int main(int argc, char* argv[])
{
    CommandLineToken token = gen_token(argv[1]);
    std::cout << token.content;
    windowContent content;
    Application<windowContent> app;
    app.setCustomState(content);
    app.set_bootstrap_function(custom_bootstrap<windowContent>);
    app.set_tick_function(custom_tick<windowContent>);

    app.run();

}*/
#endif