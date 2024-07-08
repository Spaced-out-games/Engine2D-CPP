#pragma once
#include <SDL.h>
#include <iostream>
//if (events.type == SDL_QUIT || events.type == SDL) { engineCore::quit(); }
class engineCore;

class Controller
{
public:
    Controller();
    ~Controller();

private:
    // SDL event
    SDL_Event events;

    // Whether the controller is actively polling an event
    bool isPollingEvents;

    // Let engineCore access Controller's private members and methods
    friend engineCore;

    // This is called when the engine receives an event. 
    void inputEvent();

    // For inherited types, overwrite these methods
    virtual void onQuit() { engineCore::quit(); };
    virtual void onKeyDown() { };
    virtual void onKeyUp() {  };
    virtual void onMouseMove() { };
    virtual void onMouseButtonDown() {};
    virtual void onMouseButtonUp() { };
    virtual void onMouseWheelMove() {};
    virtual void onFileDrop() {  };
    virtual void onTextDrop() {  };
    virtual void onBeginDrop() { };
    virtual void onEndDrop() {  };
};

#include "engineCore.h"

Controller::Controller() : isPollingEvents(false) {}

Controller::~Controller() = default;

void Controller::inputEvent()
{
    switch (events.type)
    {
    case SDL_QUIT:
        onQuit();
        break; 

    case SDL_KEYUP:
        onKeyUp();
        break; 

    case SDL_KEYDOWN:
        onKeyDown();
        break; 

    case SDL_MOUSEMOTION:
        onMouseMove();
        break; 

    case SDL_MOUSEBUTTONDOWN:
        onMouseButtonDown();
        break; 

    case SDL_MOUSEBUTTONUP:
        onMouseButtonUp();
        break; 

    case SDL_MOUSEWHEEL:
        onMouseWheelMove();
        break; 

    case SDL_DROPFILE:
        onFileDrop();
        break; 

    case SDL_DROPTEXT:
        onTextDrop();
        break; 

    case SDL_DROPBEGIN:
        onBeginDrop();
        break; 

    case SDL_DROPCOMPLETE:
        onEndDrop();
        break; 

    case SDL_APP_WILLENTERBACKGROUND:
        std::cout << "App will enter background\n";
        //onApplicationWillEnterBackground();
        break;
    case SDL_APP_DIDENTERBACKGROUND:
        std::cout << "App entered background\n";

        //onApplicationDidEnterBackground();
        break;
    case SDL_APP_WILLENTERFOREGROUND:
        std::cout << "App will enter foreground\n";

        //onApplicationWillEnterForeground();
        break;
    case SDL_APP_DIDENTERFOREGROUND:
        std::cout << "App entered foreground\n";

        //onApplicationDidEnterForeground();
        break;
    default:
        //std::cerr << "Unsupported Event type: Ignoring..." << std::endl; // Added missing semicolon
        break; 
    }
}