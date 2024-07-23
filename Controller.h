#pragma once
#include <SDL.h>
#include <iostream>
#include "Camera.h"
class engineCore;

class Controller
{
public:
    Controller();
    ~Controller();
    SDL_Event events;
private:

    bool isPollingEvents;

    friend engineCore;

    void inputEvent();
    void enablePollingEvents();
    void disablePollingEvents();

    virtual void onQuit(SDL_Event event) { engineCore::quit(); };
    virtual void onKeyDown(SDL_Event event) { };
    virtual void onKeyUp(SDL_Event event) { };
    virtual void onMouseMove(SDL_Event event) { getNormalizedMousePos(); };
    virtual void onMouseButtonDown(SDL_Event event) {};
    virtual void onMouseButtonUp(SDL_Event event) { };
    virtual void onMouseWheelMove(SDL_Event event) {};
    virtual void onFileDrop(SDL_Event event) {  };
    virtual void onTextDrop(SDL_Event event) {  };
    virtual void onBeginDrop(SDL_Event event) { };
    virtual void onEndDrop(SDL_Event event) {  };
    virtual void onWindowResize(SDL_Event event){
        glViewport(0, 0, event.window.data1, event.window.data2);
    };
    glm::vec2 getMousePosition() {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return glm::vec2(static_cast<float>(x), static_cast<float>(y));
    }

    glm::vec2 getNormalizedMousePos()
    {
        int width;
        int height;
        SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
        glm::vec2 mousepos = getMousePosition() - glm::vec2(width / 2, height / 2);
        mousepos = (mousepos / glm::vec2(width, height));

        //std::cout <<'(' << mousepos.x << ", " << mousepos.y << ")\t";

        return mousepos;
    }
};

#include "engineCore.h"

Controller::Controller() : isPollingEvents(false)
{}

Controller::~Controller() {
};

void Controller::inputEvent()
{
    switch (events.type)
    {
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
/*

class CameraController : public Controller
{
public:
    CameraController()
        : camera(glm::vec3(0.0f, 0.0f, 3.0f)) // Initializing Camera with a default position
    {
        // Set initial projection matrix if needed
        camera.setProjection(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        camera.setRotation(0.0f, glm::radians(-90.0f));
        //camera.setUniformLocations(shaderProgramID); // You need to set shaderProgramID
    }

    void onKeyDown(SDL_Event event) override
    {
        if (event.key.keysym.sym == SDLK_d) {
            camera.setPosition(camera.getPosition() + camera.getRight() * cameraSpeed);
        }
        if (event.key.keysym.sym == SDLK_a) {
            camera.setPosition(camera.getPosition() - camera.getRight() * cameraSpeed);
        }
        if (event.key.keysym.sym == SDLK_w) {
            camera.setPosition(camera.getPosition() + camera.getFront() * cameraSpeed);
        }
        if (event.key.keysym.sym == SDLK_s) {
            camera.setPosition(camera.getPosition() - camera.getFront() * cameraSpeed);
        }
    }

    void updateCamera()
    {
        camera.updateUniforms();
    }
    Camera camera;
private:
    
    float cameraSpeed = 0.05f; // Adjust speed as needed
    GLuint shaderProgramID; // Set this to the actual shader program ID
};

*/