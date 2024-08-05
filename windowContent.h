#pragma once

#include <vector>
#include "GUI_Node.h"
#include "Application.h"
#include "Shader.h"


//class VEditorController;

// Forward declaration if needed
// class Application;

struct windowContent
{
    std::vector<GUINode> nodes;
    size_t num_ticks;
    Shader shader;
    GUINode* last_node;
    GUINode* current_node;
    VEditorController* controller;

    // Default constructor
    windowContent();

    // Constructor that initializes all members
    windowContent(VEditorController* controller)
        : nodes(), shader(), controller(controller), last_node(nullptr), current_node(nullptr)
    {}
};

#include "VEditorController.h"

windowContent::windowContent()
    : nodes(), shader(), controller(nullptr), last_node(nullptr), current_node(nullptr)
{
    controller = new VEditorController();
}