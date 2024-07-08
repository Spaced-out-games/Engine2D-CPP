#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <cstdlib>
#include "ent_manager.h"
#include <iostream> // For std::cerr
#include "test.h"
#include "shader_manager.h"

#define compare_s(command, test) (strncmp(command, test, strlen(command)) == 0)

typedef void* (*FunctionPointer)(void*);
class CLArg;
class Controller;

class engineCore
{
public:
    // temporary testing


    static void quit();
    static bool isRunning();
    virtual int main();
    virtual void graphics_tick();
    virtual void phys_tick();
    virtual void IO_tick();
    virtual void logic_tick();
    static bool init_core(int argc, char* argv[]);
    static engineCore*& getEngineCore();
    SDL_GLContext& getContext();
    virtual void bootstrap();
    ent_manager& getManager();
    double getDeltaTime() const;
    void enableVsync();
    void disableVsync();
    bool isVsyncEnabled() const;
    void setWindowDimensions(int newWidth, int newHeight);
    void registerController(Controller* controller);
    static shader_manager& getShaderManager() { return shaderManager; }
    bool initialized() { return init_success; }
    SDL_Window* getWindow();
protected:
    engineCore();
    virtual ~engineCore();
private:
    friend CLArg;
    static shader_manager shaderManager;

    bool init_SDL();

    ent_manager EntityManager;
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
    std::string mTitle;
    Controller* controller;

    static engineCore* instance;

    static void parse_arg(char* arg, char* target_command, CLArg& CLArgument);
};

#include "CLArgument.h"
#include "Controller.h"

engineCore* engineCore::instance = nullptr;
shader_manager engineCore::shaderManager = shader_manager::getInstance();

engineCore::engineCore()
    : sdlWindow(nullptr), glContext(nullptr), mWidth(800), mHeight(600), deltaTime(0.0),
    vsync_enabled(false), running(false), init_success(false), mFullscreen(false), mTitle("Engine"), controller(nullptr)
{
 //   shaderManager = shader_manager::getInstance()
}

engineCore::~engineCore()
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



bool engineCore::init_SDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Initialization failed: SDL_Error: " << SDL_GetError() << "\n";
        return false;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);  // 4x MSAA

    sdlWindow = SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (!sdlWindow)
    {
        std::cerr << "Window failed to initialize. Error code: " << SDL_GetError();
        return false;
        exit(1);
    }

    glContext = SDL_GL_CreateContext(sdlWindow);
    if (!glContext)
    {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
        exit(1);
    }

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        std::cerr << "Error initializing GLEW! " << glewGetErrorString(glewError) << std::endl;
        return false;
    }
    /*glViewport(0, 0, mWidth, mHeight);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Enable multisampling
    glEnable(GL_MULTISAMPLE);
    */
    SDL_GL_SetSwapInterval(1);
    // Set the viewport
    glViewport(0, 0, 800, 600);

    // Set the clear color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    return true;
}

void engineCore::quit()
{
    getEngineCore()->running = false;
}

bool engineCore::isRunning()
{
    return getEngineCore()->running;
}

void engineCore::graphics_tick()
{
    glClear(GL_COLOR_BUFFER_BIT);
    shaderManager.use(0);
    // Draw everything
    
}

void engineCore::IO_tick()
{
    if (controller)
    {
        controller->isPollingEvents = true;
        while (SDL_PollEvent(&(controller->events)))
        {
            controller->inputEvent();
        }
        controller->isPollingEvents = false;
    }
    else
    {
        std::cerr << "ERROR: No controller connected! Using fallback option (basic Controller) \n";
        Controller* c = new Controller();
        controller = c;
    }
}

void engineCore::phys_tick() {}

void engineCore::bootstrap()
{


}

void engineCore::logic_tick()
{

}

int engineCore::main()
{
    //std::cout << test_mesh.VAO;
    bootstrap();
    running = true;
    while (running)
    {
        // Maybe one thread for each?
        IO_tick();
        phys_tick();
        graphics_tick();
        logic_tick();

        //SDL_GL_SwapWindow(sdlWindow);
        
    }
    return 0;
}

bool engineCore::init_core(int argc, char* argv[])
{
    if (instance == nullptr)
    {
        // Create an instance if we dont have one
        instance = new engineCore();

        // Run Command-Line arguments
        for (int i = 1; i < argc; i++)
        {
            CLArg argument;
            parse_arg(argv[i], "title", argument);
            
            if (argument.isValid()) {
                argument.execute(instance);
            }
        }

        //Try to initialize SDL
        if (!instance->init_SDL())
        {
            delete instance; // Clean up the instance if SDL initialization fails
            instance = nullptr;
            return false;
        }

        // Let us know we successfully initialized the engine
        instance->init_success = true;
        return true;
    }
    return false;
}

void engineCore::parse_arg(char* arg, char* target_command, CLArg& CLArgument)
{
    if (strlen(arg) < 2 || target_command == nullptr || arg == nullptr) return;

    if (strlen(arg) == 2 && arg[0] == '-')
    {
        CLArgument.mCommand = arg + 1;
        CLArgument.mArgument = nullptr;
        return;
    }

    if (strlen(arg) > 2 && arg[0] == '-' && arg[1] == '-')
    {
        char* command = arg + 2;
        if (strncmp(command, target_command, strlen(target_command)) == 0)
        {
            char* argument = command + strlen(target_command);
            CLArgument.mCommand = target_command;
            CLArgument.mArgument = argument;
            return;
            // CLArgument = CLArg(target_command, argument);
        }
    }
}

engineCore*& engineCore::getEngineCore()
{
    return instance;
}

SDL_GLContext& engineCore::getContext()
{
    return glContext;
}

ent_manager& engineCore::getManager()
{
    return EntityManager;
}

double engineCore::getDeltaTime() const
{
    return deltaTime;
}

void engineCore::enableVsync()
{
    vsync_enabled = true;
    SDL_GL_SetSwapInterval(1);
}

void engineCore::disableVsync()
{
    vsync_enabled = false;
    SDL_GL_SetSwapInterval(0);
}

bool engineCore::isVsyncEnabled() const
{
    return vsync_enabled;
}

void engineCore::setWindowDimensions(int newWidth, int newHeight)
{
    mWidth = newWidth;
    mHeight = newHeight;
    SDL_SetWindowSize(sdlWindow, mWidth, mHeight);
}

void engineCore::registerController(Controller* newController)
{
    controller = newController;
}
SDL_Window* engineCore::getWindow()
{
    return sdlWindow;
}



