#pragma once
#include <SDL.h>
#include <iostream>

class engineCore;

class Controller
{
public:
    Controller();
    ~Controller();

private:
    SDL_Event* events;
    bool isPollingEvents;

    friend engineCore;

    void inputEvent();
    void enablePollingEvents();
    void disablePollingEvents();

    virtual void onQuit() { engineCore::quit(); };
    virtual void onKeyDown() { };
    virtual void onKeyUp() { };
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

Controller::Controller() : isPollingEvents(false), events(nullptr) 
{
    events = new SDL_Event();
}

Controller::~Controller() {
    delete events;
}

void Controller::inputEvent()
{
    switch (events->type)
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
    default:
        break;
    }
}

void Controller::enablePollingEvents()
{
    isPollingEvents = true;
}

void Controller::disablePollingEvents()
{
    isPollingEvents = false;
}