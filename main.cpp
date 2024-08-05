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

#pragma once
#include "Application.h"
#include "GUI_Node.h"
#include "shader.h"
#include "shader_manager.h"
#include "VEditorController.h"
using glm::vec3;
using glm::mat4;
#include "windowContent.h"

//#include "windowContent.h"

void custom_bootstrap(void* input)
{
    // Cast to an application pointer
    Application<windowContent>* app = (Application<windowContent>*)input;

    // set the shader
    app->getCustomState()->shader = (Shader::getDefaultShader());


    // Create the geometry
    GUINode::init();
    app->getCustomState()->nodes.push_back(GUINode(vec3(0.0f,0.0f,0.0f),vec3(1.0f,1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), 1));
    
    app->getCustomState()->nodes[0].addChild(new GUINode(vec3(0.0f, 0.0f, 0.0f), vec3(0.5f, 0.5f, 0.5), vec3(0.0f, 1.0f, 1.0f), 1));
}


void custom_tick(void* input)
{

    // cast to application pointer
    Application<windowContent>* app = (Application<windowContent>*)input;

    auto a = app->getCustomState()->controller;


    while (
            SDL_PollEvent(
                nullptr
            )
        )
    {
        app->getCustomState()->controller->enablePollingEvents();
        app->getCustomState()->controller->inputEvent();
        app->getCustomState()->controller->disablePollingEvents();
    };

    // use the shader
    app->getCustomState()->shader.use(); 

    // draw the node
    app->getCustomState()->nodes[0].draw();

    glm::vec2 mouse_position = app->getCustomState()->controller->getNormalizedMousePos();
    glm::vec3 root_scale = app->getCustomState()->nodes[0].scaleFromMatrix(app->getCustomState()->nodes[0].get_nested_transform());
    glm::vec3 child_scale = app->getCustomState()->nodes[0].children[0]->scaleFromMatrix(app->getCustomState()->nodes[0].children[0]->get_nested_transform());
   

    //auto collision = app->getCustomState()->nodes[0].collides(mouse_position);

    if ((app->getCustomState()->num_ticks % 1000) == 0)
    {
        //std::cout << "app:\t\t" << app->getCustomState() << '\n';

    }

    app->getCustomState()->num_ticks++;
}

void collision_check(windowContent& content)
{
    // Have the controller call this onMouseMove
    // Also assign last_node and current_node onMouseButtonDown & onMouseButtonUp

    // Here, scan through each root node in the scene
    std::cout << "@collision check.\n";


}


#include <iostream>

int main(int argc, char* argv[])
{
    Application<windowContent> app (argc, argv); // initializes SDL window & context
    


    // Initialize windowContent with the Application instance
    windowContent content;
    app.setCustomState(content); // assigns state
    //app.getCustomState()->controller->owner = &app;

    // set but not call functions
    app.set_bootstrap_function(custom_bootstrap);
    app.set_tick_function(custom_tick);

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