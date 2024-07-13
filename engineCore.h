#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <cstdlib>
#include "ent_manager.h"
#include <iostream> // For std::cerr
#include "shader_manager.h"
#include "global_uniforms.h"
#include "Camera.h"
#include "Mesh2D.h"

#define compare_s(command, test) (strncmp(command, test, strlen(command)) == 0)

typedef void* (*FunctionPointer)(void*);
class CLArg;
class Controller;

void drawTriangleImmediateMode();
void drawTriangle(SDL_Window* window);
std::string readShaderFile(const std::string& filePath);
GLuint compileShader(const std::string& source, GLenum shaderType);
GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);

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
    //void registerController(Controller* controller);
    bool initialized() { return init_success; }
    SDL_Window* getWindow();
    shader_manager shaderManager;
protected:
    engineCore();
    virtual ~engineCore();
private:
    friend CLArg;

    void init_SDL();

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
    void setUniform(int s, const std::string& name, const glm::mat4& mat);
};

#include "CLArgument.h"
#include "Controller.h"

engineCore* engineCore::instance = nullptr;
//shader_manager* engineCore::shaderManager = shader_manager::getInstance();

engineCore::engineCore()
    : sdlWindow(nullptr), mWidth(1400), mHeight(600), deltaTime(0.0),
    vsync_enabled(false), running(false), init_success(false), mFullscreen(false), mTitle("Engine"), controller(nullptr)
{
    instance = this;


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



void engineCore::init_SDL() {
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
    sdlWindow = SDL_CreateWindow("SDL OpenGL Rectangle",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

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
    glViewport(0, 0, 800, 600);

    // Set the clear color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}


void engineCore::quit()
{
    getEngineCore()->running = false;
}

bool engineCore::isRunning()
{
    return getEngineCore()->running;
}
// glClearColor(0, 1, 0, 255);
void engineCore::graphics_tick()
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    

    SDL_GL_SwapWindow(sdlWindow);
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

void engineCore::IO_tick()
{

    if (controller)
    {
        controller->enablePollingEvents();


        
        while (SDL_PollEvent(controller->events)) // This is where the fun happens
        {
            controller->inputEvent();
        }
        controller->disablePollingEvents();
    }
    else
    {
        std::cerr << "ERROR: No controller connected! Using fallback option (basic Controller) \n";
        controller = new Controller();;
    }
}

void engineCore::phys_tick() {}

void engineCore::bootstrap()
{
    //EntityManager
}

void engineCore::logic_tick()
{

}

int engineCore::main()
{

    // last
    bootstrap();
    GLfloat vertices[] = {
0.0f, 0.0f,  // Bottom-left
1.0f, 0.0f,  // Bottom-right
1.0f, -1.0f, // Top-right
0.0f, -1.0f  // Top-left
    };

    GLuint indices[] = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };
    Shader shader_test = Shader::getDefaultShader();


    Mesh2D rect(vertices, 8, indices, 6);

    glm::mat4 transform(1.0f);

    glm::vec3 color(1.0, 0.0, 0.0);
    
    

    running = true;
    while (running)
    {
        // Maybe one thread for each?
        IO_tick();
        phys_tick();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader_test.use();
        Shader::setUniform("transform", transform);
        Shader::setUniform("color", color);
        rect.draw();
        SDL_GL_SwapWindow(sdlWindow);
        logic_tick();

        
 
    }
    return 0;
}

bool engineCore::init_core(int argc, char* argv[])
{
    // Simply creates an instance if it can
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
        instance->init_SDL();
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

//void engineCore::registerController(Controller* newController)
//{
//    controller = newController;
//}
SDL_Window* engineCore::getWindow()
{
    return sdlWindow;
}


