#pragma once
#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>

#ifndef CONTROLLER_H
#define CONTROLLER_H

template <class Pawn>
class Controller
{
public:
    Controller(Pawn* pawn);
    ~Controller();

    void inputEvent();
    void enablePollingEvents();
    void disablePollingEvents();
    SDL_Event& getEvents() { return events; }

    virtual void onQuit(SDL_Event event) {};
    virtual void onKeyDown(SDL_Event event) {};
    virtual void onKeyUp(SDL_Event event) {};
    virtual void onMouseMove(SDL_Event event) { getNormalizedMousePos(); };
    virtual void onMouseButtonDown(SDL_Event event) {};
    virtual void onMouseButtonUp(SDL_Event event) {};
    virtual void onMouseWheelMove(SDL_Event event) {};
    virtual void onFileDrop(SDL_Event event) {};
    virtual void onTextDrop(SDL_Event event) {};
    virtual void onBeginDrop(SDL_Event event) {};
    virtual void onEndDrop(SDL_Event event) {};
    virtual void onWindowResize(SDL_Event event) {
        glViewport(0, 0, event.window.data1, event.window.data2);

        // Update window widths and heights when the window is resized
        windowWidth = static_cast<float>(event.window.data1);
        windowHeight = static_cast<float>(event.window.data2);

    };

    glm::vec2 getMousePosition();
    // Returns mouse position in(-1.0 - 1.0) range in window space
    glm::vec2 getNormalizedMousePos();
    Pawn* getPawn() { return mPawn; }

private:
    // Using floats since they are more precise to scale
    float windowWidth;
    float windowHeight;
    Pawn* mPawn;
    bool isPollingEvents;
    SDL_Event events;
};

// Define the template member functions in the same header file
template <class Pawn>
Controller<Pawn>::Controller(Pawn* pawn) : isPollingEvents(false), mPawn(pawn)
{
    int w, h;
    SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &w, &h);
    windowWidth = static_cast<float>(w);
    windowHeight = static_cast<float>(h);
}

template <class Pawn>
Controller<Pawn>::~Controller() {}

template <class Pawn>
void Controller<Pawn>::inputEvent() {
    while (SDL_PollEvent(&events)) {
        if (!isPollingEvents) return;

        switch (events.type) {
        case SDL_QUIT:
            onQuit(events);
            break;
        case SDL_KEYUP:
            onKeyUp(events);
            break;
        case SDL_KEYDOWN:
            onKeyDown(events);
            break;
        case SDL_MOUSEMOTION:
            onMouseMove(events);
            break;
        case SDL_MOUSEBUTTONDOWN:
            onMouseButtonDown(events);
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseButtonUp(events);
            break;
        case SDL_MOUSEWHEEL:
            onMouseWheelMove(events);
            break;
        case SDL_DROPFILE:
            onFileDrop(events);
            break;
        case SDL_DROPTEXT:
            onTextDrop(events);
            break;
        case SDL_DROPBEGIN:
            onBeginDrop(events);
            break;
        case SDL_DROPCOMPLETE:
            onEndDrop(events);
            break;
        case SDL_APP_WILLENTERBACKGROUND:
            std::cout << "App will enter background\n";
            break;
        case SDL_APP_DIDENTERBACKGROUND:
            std::cout << "App entered background\n";
            break;
        case SDL_APP_WILLENTERFOREGROUND:
            std::cout << "App will enter foreground\n";
            break;
        case SDL_APP_DIDENTERFOREGROUND:
            std::cout << "App entered foreground\n";
            break;
        case SDL_WINDOWEVENT_RESIZED:
            onWindowResize(events);
            std::cout << "Window resized\n";
            break;
        default:
            break;
        }
    }
}

template <class Pawn>
void Controller<Pawn>::enablePollingEvents() {
    isPollingEvents = true;
}

template <class Pawn>
void Controller<Pawn>::disablePollingEvents() {
    isPollingEvents = false;
}

template <class Pawn>
glm::vec2 Controller<Pawn>::getMousePosition() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return glm::vec2(static_cast<float>(x), static_cast<float>(y));
}
template <class Pawn>
glm::vec2 Controller<Pawn>::getNormalizedMousePos() {


    glm::vec2 mousepos = (getMousePosition() - glm::vec2(windowWidth / 2, windowHeight / 2));
    mousepos = (mousepos / glm::vec2(windowWidth / 2, windowHeight / 2));
    return mousepos;
}

#endif