#ifndef WINDOW_CONTENT_H
#define WINDOW_CONTENT_H
struct windowContent
{
    std::vector<GUINode> nodes;
    size_t num_ticks;
    Shader shader;
    GUINode* last_node;
    VEditorController<windowContent> controller;

    windowContent()
        : nodes(), shader(), controller(nullptr), last_node(nullptr) // Initialize controller with app
    {}
    // Constructor that initializes all members
    windowContent(Application<windowContent>* app)
        : nodes(), shader(), controller(app), last_node(nullptr) // Initialize controller with app
    {}
};
#endif