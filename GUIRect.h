#pragma once
#include "GUI_Node.h"
#include <SDL.h>
#include <GL/gl.h>
#include "shader.h"

class GUIRect : public GUINode {
public:
    GUIRect(float x, float y, float width, float height, SDL_Color color);
    GUIRect(float x, float y, float width, float height);
    GUIRect();

    void draw() override;
    SDL_Color getColor() const;

private:
    SDL_Color color;
    pair2D location;
    float width;
    float height;
};

// Implementation
GUIRect::GUIRect(float x, float y, float width, float height, SDL_Color color)
    : GUINode(), location(x, y), width(width), height(height), color(color) {}

GUIRect::GUIRect(float x, float y, float width, float height)
    : GUIRect(x, y, width, height, { 128, 128, 128, 255 }) { // Default color grey
}

GUIRect::GUIRect()
    : GUIRect(0.0f, 0.0f, 100.0f, 100.0f) { // Default size and color
}

void GUIRect::draw() {
   
    SDL_GLContext context = getContext();
    //SDL_GL_GetCurrentContext()
    if (!context) {
        return; // No context available
    }
    
    glColor4f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f); // Set color with alpha
    glBegin(GL_QUADS);
    glVertex2f(location.as_float().x, location.as_float().y);
    glVertex2f(location.as_float().x + width, location.as_float().y);
    glVertex2f(location.as_float().x + width, location.as_float().y + height);
    glVertex2f(location.as_float().x, location.as_float().y + height);
    glEnd();

    // Draw all children
    for (int i = 0; i < numChildren(); ++i) {
        GUINode* child = getChildren().c_array()[i];
        if (child != nullptr)
        {
            if (GUIRect* rectChild = dynamic_cast<GUIRect*>(child)) {
                rectChild->draw(); // Recursively draw children
            }
        }
    }
}

SDL_Color GUIRect::getColor() const {
    return color;
}