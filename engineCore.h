#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "ent_manager.h"

class Controller;



class engineCore
{
public:


	// Quits the engine
	static void quit();

	// Whether or not this instance of the engine is running
	static bool isRunning();

	// Main function. Calls graphics_tick, phys_tick, logic_tick, and IO_tick in a while loop, basically
	int main();

	// Draws everything
	void graphics_tick();

	// handles physics for a tick
	void phys_tick();

	// Handles IO for a tick
	void IO_tick();

	static bool init_core(int argc, char* argv[]);

	// Gets a singleton instance of engineCore
	static engineCore* getEngineCore();
	//static engineCore* getEngineCore(const char* title, int width, int height);


	// Gets the OpenGL context of this instance of engineCore
	const SDL_GLContext& getContext();

	// Is called after initialization but before the main game loop. This spawns essential entities and whatnot to get the ball rolling. Feel free to override
	void bootstrap();


	// Get a reference to the manager of this instance
	ent_manager& getManager();

	double getDeltaTime();

	void enableVsync();

	void disableVsync();

	bool isVsyncEnabled();

	void setWindowDimensions(int newWidth, int newHeight);

	void registerController(const Controller& controller);




private:
	engineCore(const char* title, int width, int height);
	~engineCore();
	// Entity manager. Handles adding and removing entities to the scene.
	ent_manager EntityManager;

	SDL_Window* sdlWindow;

	SDL_GLContext glContext;

	SDL_Event sdlEvents;

	int windowWidth;

	int windowHeight;

	double deltaTime;

	bool vsync_enabled;

	bool running;

	bool init_success;

	const char* mTitle; // renamed to avoid conflicts


	Controller* controller;

	static engineCore* instance;

	void parse_arg(const char* argument);

};

#include "Controller.h"

engineCore* engineCore::instance = nullptr;

engineCore::engineCore(const char* title, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	vsync_enabled = false;
	mTitle = title;

	// Attempt to initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Initialization failed: SDL_Error: " << SDL_GetError() << "\n";
		init_success = false;
		goto fail;
	}

	// Let SDL know what kind of context we want
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	goto init; // C4102

	init:
		sdlWindow = SDL_CreateWindow(title, 0, 0, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
		if (!sdlWindow)
		{
			std::cerr << "Window failed to initialize. Error code: " << SDL_GetError();
			init_success = false;
			goto fail;
		}

		glContext = SDL_GL_CreateContext(sdlWindow);
		if (!glContext)
		{
			std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
			SDL_DestroyWindow(sdlWindow);
			init_success = false;
			goto fail;
		}

		// Since I want vsync to default to off, let's not deal with that, but let the game programmer do it via disableVsync and enableVsync

		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK) {
			std::cerr << "Error initializing GLEW! " << glewGetErrorString(glewError) << std::endl;
			init_success = false;
			goto fail;
		}
		init_success = true;
	
	fail:
		init_success = false;

		
}

engineCore::~engineCore()
{
	if (glContext)
	{
		SDL_GL_DeleteContext(glContext);
	}

	if (sdlWindow) { SDL_DestroyWindow(sdlWindow); }

}

void engineCore::quit() { getEngineCore()->running = false; }

bool engineCore::isRunning() { return getEngineCore()->running; }

void engineCore::graphics_tick()
{
	// buffering
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// TODO: scan through props that need rendered

	// buffer the window
	SDL_GL_SwapWindow(sdlWindow);

}
// Overwride this with your own logic. TODO: add a basic controller class that fires and recieves events and encapsulate it as such so that we aren't passing pointers around that we don't know the safety of. For all we know, the user is passing a void* as a SDL_Event* which can cause issues. This way, a controller with custom can be registered into the engine and the engine and interact with game props with its own logic
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
		std::cerr << "ERROR: No controller connected!";
	}
}

void engineCore::phys_tick() {}

/*
engineCore* engineCore::getEngineCore()
{
	if (!instance)
	{
		instance = new engineCore("Engine", 800, 600); // Example parameters
	}
	return instance;
}
*/


void engineCore::bootstrap() {}

int engineCore::main()
{
	bootstrap();
	while (running)
	{
		IO_tick();
		phys_tick();
		graphics_tick();
	}
	return 0;
}


bool engineCore::init_core(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		instance->parse_arg(argv[i]);
	}
	if (instance == nullptr)
	{
		instance = new engineCore("placeholder", 480, 480);
		return instance->init_success; // Return initialization success status
	}
	return false; // Instance already initialized
}

void engineCore::parse_arg(const char* arg)
{

}

// Gets the singleton instance without initializing
engineCore* engineCore::getEngineCore()
{
	return instance;
}