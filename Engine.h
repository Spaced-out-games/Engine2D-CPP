#ifndef ENGINE_H
#define ENGINE_H

#pragma once
#include <SDL.h>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>


// This was causing issues
#include "prop.h"


#define byte char
#define PROP_ATTRIBUTE_COUNT 9
#define MAX_PROPS 512

//#define ENGINE Engine::getGame()
//#define CANVAS Engine::getGame().getCanvas()

//class Prop;

class Engine
{
public:
	

	// Quits the game
	void quit();

	// Returns true if the engine is running
	bool isRunning();

	// Runs the main loop. Basically, it calls run() in a while loop and also finds delta_time
	int mainLoop();

	// Runs the engine for one tick
	void run();

	// Handles IO
	void handleIO();

	// Gets a singleton instance of the engine
	static Engine& getInstance();

	// Get a reference to the OpenGL context
	SDL_GLContext& getContext();

	// Would be interesting if I made this into a function pointer that external programs can overwrite, that way users of the library can create their own function calls remotely
	void on_init(); 


	bool addProp(Prop* new_prop);
	void removeProp(int index);
	void removeProp(Prop* old_prop);

	bool isValidProp(Prop* prop);

	
	

	
private:
	// Window / event handling
	//ShaderHandler shaders;
	Engine(const char* title, int width, int height);
	~Engine();
	SDL_Window* window = nullptr;
	//SDL_Surface* canvas = NULL;
	SDL_GLContext context;
	SDL_Event events;
	int window_width;
	int window_height;
	double delta_time;
	bool init_success;
	bool vsync_success;
	bool running = true;
	// Prop / component information

	Prop* props[MAX_PROPS] = {};
	int partition_index = 0; // Point to the last index
	//float attributes[MAX_PROPS * PROP_ATTRIBUTE_COUNT];
	

	
};

Engine::Engine(const char* title, int width, int height)
{
	bool init_success = true;

	// Initialize window	
	window_width = width;
	window_height = height;

	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Initialization failed: SDL_Error: " << SDL_GetError() << "\n";
		init_success = false;
		return;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	window = SDL_CreateWindow(title, 0, 0, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!window)
	{
		std::cerr << "Window failed to initialize. Error code: " << SDL_GetError();
		init_success = false;
		return;
	}

	context = SDL_GL_CreateContext(window);
	if (!context)
	{
		std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		init_success = false;
		return;
	}

	if (SDL_GL_SetSwapInterval(1) < 0) {
		std::cerr << "Warning: Unable to set VSync! SDL_Error: " << SDL_GetError() << std::endl;
		vsync_success = false;
	}
	else
	{
		vsync_success = true;
	}

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		std::cerr << "Error initializing GLEW! " << glewGetErrorString(glewError) << std::endl;
		init_success = false;
		return;
	}
	init_success = true;


}

Engine::~Engine() {
	if (context)
	{
		SDL_GL_DeleteContext(context);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}

void Engine::quit()
{
	running = false;
}

bool Engine::isRunning() { return running; }

void Engine::run()
{
	// buffering
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	

	glColor3b(0, 0, 0);
	// Rendering the Props
	for (size_t i = 0; i < MAX_PROPS; i++)
	{
		if (props[i])
		{
			props[i]->draw();
		}
	}
	SDL_GL_SwapWindow(window);
}

void Engine::handleIO()
{
	while (SDL_PollEvent(&events))
	{
		if (events.type == SDL_QUIT) { running = false; }
	}
}

int Engine::mainLoop()
{
	on_init();
	while (running)
	{
		handleIO();
		run();
	}

	return 0;
}

void Engine::on_init()
{
	// This is where you would spawn Props
}

Engine& Engine::getInstance() { static Engine instance = Engine("test", 1920, 1080); return instance; }


SDL_GLContext& Engine::getContext()
{
	return context;
}

bool Engine::addProp(Prop* added_prop)
{
	if (partition_index < MAX_PROPS)
	{
		props[partition_index] = added_prop;
		props[partition_index]->setID(partition_index);
		partition_index++;
		return 1;
	}
	return 0;
}
void Engine::removeProp(Prop* removed_prop)
{
	if (!removed_prop) { return; }
	// Get the prop's index
	int deleted_index = removed_prop->getID();

	// Delete the prop
	delete removed_prop;

	// Since this prop is now invalid, move the last valid prop here
	props[deleted_index] = props[partition_index];

	// Since we've moved the prop's pointer, that prop needs to update its ID
	props[deleted_index]->setID(deleted_index);

	// Since the last prop is pointed to twice in the array, set the original index to nullptr
	props[partition_index] = nullptr;

	// Since the original pointer is invalidated, advance the partition index backwards
	partition_index--;

}

bool Engine::isValidProp(Prop* prop)
{
	// This can probably be optimized further
	if (prop)
	{
		if (props[prop->getID()] == prop)
		{
			return 1;
		}
	}
	return false;
}


#endif