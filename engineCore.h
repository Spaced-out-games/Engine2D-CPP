#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <GL/glew.h>
#include <string>
#include <cstdlib>
#include <iostream> // For std::cerr

class Controller;

template <class state_t>
class Application
{
public:
    using app_callback = void (*)(void*);

    Application();
    ~Application();

    // Quits the application
    void quit();

    // Returns whether the engine is currently running
    bool isRunning();

    // Main run loop
    virtual int run();

    // Different tick functions for various subsystems
    virtual void graphics_tick();
    virtual void phys_tick();
    virtual void IO_tick();
    virtual void logic_tick();

    // Getters
    SDL_GLContext& getContext();
    double getDeltaTime();
    SDL_Window* getWindow();
    bool is_vsync_enabled();

    // Setters
    void enable_vsync();
    void disable_vsync();
    void setWindowDimensions(int newWidth, int newHeight);
    void setController(Controller* new_controller);
    void setCustomState(const state_t& state);

    void set_tick_function(app_callback func);
    void set_bootstrap_function(app_callback func);

    // State management
    state_t* getCustomState();

private:
    void init_SDL();

    SDL_Window* sdlWindow;
    SDL_GLContext glContext;
    SDL_Event sdlEvents;
    int mWidth;
    int mHeight;
    double deltaTime;
    bool vsync_enabled;
    bool running;
    bool init_success;
    bool mFullscreen;
    app_callback bootstrap;
    app_callback tick;

    std::string mTitle;
    Controller* controller;
    state_t custom_state;
};

// Implementation of the template functions
#include "Controller.h"

template <class state_t>
Application<state_t>::Application() :
    sdlWindow(nullptr),
    glContext(nullptr),
    mWidth(1920),
    mHeight(1080),
    deltaTime(0.0),
    vsync_enabled(false),
    running(false),
    init_success(false),
    mFullscreen(false),
    mTitle("Engine"),
    controller(nullptr),
    bootstrap(nullptr),
    tick(nullptr)
{
    #ifdef _DEBUG
    std::cout << "engineCore constructed\n";
    #endif
    init_SDL();
}

template <class state_t>
Application<state_t>::~Application()
{
    if (glContext)
    {
        SDL_GL_DeleteContext(glContext);
    }

    if (sdlWindow)
    {
        SDL_DestroyWindow(sdlWindow);
    }

    SDL_Quit();
}

template <class state_t>
void Application<state_t>::init_SDL()
{
    // Initialize SDL. In order
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Set OpenGL version (here we use OpenGL 3.3). In order
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // in order
    // Create SDL window
    sdlWindow = SDL_CreateWindow(mTitle.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        mWidth, mHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    // Create an SDL window. In order
    if (!sdlWindow) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Create OpenGL context
    glContext = SDL_GL_CreateContext(sdlWindow);
    if (!glContext) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Initialize GLEW. In order
    glewExperimental = GL_TRUE; // Enable experimental features for core profile
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }
    // Set the swap interval for the current OpenGL context. Present
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Set the viewport
    glViewport(0, 0, mWidth, mHeight);

    // Set the clear color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    std::cout << "1. Set up SDL and OpenGL context.\n";
}

template <class state_t>
void Application<state_t>::quit()
{
    running = false;
}

template <class state_t>
bool Application<state_t>::isRunning()
{
    return running;
}

template <class state_t>
void Application<state_t>::graphics_tick() {}

template <class state_t>
void Application<state_t>::phys_tick() {}

template <class state_t>
void Application<state_t>::IO_tick()
{
    if (controller)
    {
        controller->enablePollingEvents();
        while (SDL_PollEvent(&controller->events)) // This is where the fun happens
        {
            controller->inputEvent();
        }
        controller->disablePollingEvents();
    }
    else
    {
        std::cerr << "ERROR: No controller connected! Using fallback option (basic Controller) \n";
        controller = new Controller();
    }
}

template <class state_t>
void Application<state_t>::logic_tick() {}

template <class state_t>
int Application<state_t>::run()
{
    // bootstrap if we can, passing a pointer to the application
    if (bootstrap) {
        bootstrap(this);
    }

    #ifdef _DEBUG
    std::cout << "engineCore bootstrapped\n";
    #endif


    running = true;
    while (running)
    {


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Tick the engine, passing a pointer to the application
        tick(this);
        #ifdef _DEBUG
        std::cout << "engineCore tick\n";
        #endif
        SDL_GL_SwapWindow(sdlWindow);

        // logic_tick();
    }
    return 0;
}

template <class state_t>
SDL_GLContext& Application<state_t>::getContext()
{
    return glContext;
}

template <class state_t>
void Application<state_t>::enable_vsync()
{
    SDL_GL_SetSwapInterval(1);
    vsync_enabled = true;
}

template <class state_t>
void Application<state_t>::disable_vsync()
{
    SDL_GL_SetSwapInterval(0);
    vsync_enabled = false;
}

template <class state_t>
bool Application<state_t>::is_vsync_enabled()
{
    return vsync_enabled;
}

template <class state_t>
void Application<state_t>::setWindowDimensions(int newWidth, int newHeight)
{
    mWidth = newWidth;
    mHeight = newHeight;
    SDL_SetWindowSize(sdlWindow, mWidth, mHeight);
    glViewport(0, 0, mWidth, mHeight);
}

template <class state_t>
SDL_Window* Application<state_t>::getWindow()
{
    return sdlWindow;
}

template <class state_t>
void Application<state_t>::setController(Controller* new_controller)
{
    controller = new_controller;
}

template <class state_t>
void Application<state_t>::setCustomState(const state_t& state)
{
    custom_state = state;
}

template <class state_t>
state_t* Application<state_t>::getCustomState()
{
    return &custom_state;
}

template <class state_t>
void Application<state_t>::set_bootstrap_function(app_callback func)
{
    bootstrap = func;
}

template <class state_t>
void Application<state_t>::set_tick_function(app_callback func)
{
    tick = func;
}
/*
void render(model& m, glm::vec3 color, glm::mat4& transform) {

    glUseProgram(m.shaderProgram);

    GLint colorLoc = glGetUniformLocation(m.shaderProgram, "color");
    glUniform3f(colorLoc, color.r, color.g, color.b);

    GLint transformLoc = glGetUniformLocation(m.shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    m.draw(transform);
}

*/