#pragma once
#include "Controller.h"


template <class state_t>
class Application;

class GUINode;

// Define VEditorController as a template that inherits from Controller
template <class appstate_t>
class VEditorController : public Controller<Application<appstate_t>>
{
public:
    GUINode* target_node;
    // Constructor that passes the Application<appstate_t> pointer to the base Controller class
    VEditorController(Application<appstate_t>* app)
        : Controller<Application<appstate_t>>(app) {}


    void onMouseMove(SDL_Event event) override
    {
        this->getPawn();
        std::cout << "Mouse moves";

    }

    
};
