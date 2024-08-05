#pragma once

#include "Controller.h"
#include "Application.h"
//#include "windowContent.h"

class windowContent;

class GUINode;

// Define VEditorController as a template that inherits from Controller
class VEditorController : public Controller<Application<windowContent>> {
public:
    VEditorController() // Constructor
        : Controller<Application<windowContent>>() {
        // Initialization code...
    }

    void onMouseMove(SDL_Event event) override {
        // Implementation...
    }
};