#ifndef ENGINE_H
#define ENGINE_H

#pragma once
#include <SDL.h>
#include <iostream>

#include "vec2.h"


// This was causing issues with causing 
//#include "prop.h"


#define byte char
#define PROP_ATTRIBUTE_COUNT 9
#define MAX_PROPS 512

//#define ENGINE Engine::getGame()
//#define CANVAS Engine::getGame().getCanvas()

class Prop;

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
	// Creates a singleton instance of the engine

	// Gets a singleton instance of the engine
	static Engine& getInstance();
	// Finds an unused slot for a new prop
	int find_slot();
	// Removes a prop by setting prop.flags[PROP_VALID] to zero
	void remove_prop(int idx);
	void add_prop(int idx, Prop* new_prop);

	// Gets a prop's position in screen space
	vec2 getPropPosition(int index);
	// Sets a prop's position in screen space
	void setPropPosition(int index, vec2 new_position);

	// Gets a prop's velocity (in pixels / second)
	vec2 getPropVelocity(int index);

	// Sets a prop's velocity (in pixels / second)
	void setPropVelocity(int index, vec2 new_velocity);

	// Gets a prop's acceleration (in pixels / second^2)
	vec2 getPropAcceleration(int index);
	// Sets a prop's acceleration (in pixels / second^2)
	void setPropAcceleration(int index, vec2 new_velocity);

	// Gets a prop's rotation (in radians)
	float getPropRotation(int index);
	// Sets a prop's rotation (in radians)
	void setPropRotation(int index, float new_rotation);

	// Gets a prop's rotational velocity (radians / second)
	float getPropRotationalVelocity(int index);
	// Sets a prop's rotational velocity (radians / second)
	void setPropRotationalVelocity(int index, float new_rotational_velocity);

	// Gets a prop's rotational velocity (radians / second^2)
	float getPropRotationalAcceleration(int index);
	// Sets a prop's rotational velocity (radians / second^2)
	void setPropRotationalAcceleration(int index, float new_rotational_velocity);

	bool propIsValid(int index);

	// Gets the size of the game's window
	vec2 getWindowSize();

	void setWindowSize(vec2 new_window_size);

	SDL_Surface& getCanvas();

	Prop* getProp(int index);

	
	

	
private:
	// Window / event handling
	
	Engine(const char* title, int width, int height);
	~Engine();
	SDL_Window* window = NULL;
	SDL_Surface* canvas = NULL;
	SDL_Renderer* renderer = nullptr;
	SDL_Event events;
	int window_width;
	int window_height;
	double delta_time;
	
	bool running = true;

	// Prop / component information

	Prop* props[MAX_PROPS] = {nullptr};

	/*
	For the sake of performance and ease of use, all transformation information will be stored here contiguously as a 2D array of floats:
	Each row represents one prop, and each column represents an atomic attribute for each prop (eg, all X values)
	* two for position
	* two for velocity
	* two for acceleration
	* one for rotation
	* one for rotational velocity
	* and one for rotational acceleration

	*/
	float attributes[MAX_PROPS * PROP_ATTRIBUTE_COUNT];
	
	/*
	These point into different places within attributes
	*/

	float* position_x =					attributes + 0 * MAX_PROPS; 
	float* position_y =					attributes + 1 * MAX_PROPS;
	float* velocity_x =					attributes + 2 * MAX_PROPS;
	float* velocity_y =					attributes + 3 * MAX_PROPS;
	float* acceleration_x =				attributes + 4 * MAX_PROPS;
	float* acceleration_y =				attributes + 5 * MAX_PROPS;
	float* rotation =					attributes + 6 * MAX_PROPS;
	float* rotational_velocity =		attributes + 7 * MAX_PROPS;
	float* rotational_acceleration =	attributes + 8 * MAX_PROPS;
	
};

Engine::Engine(const char* title, int width, int height)
{

	// Initialize window	
	window_width = width;
	window_height = height;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Initialization failed: SDL_Error: " << SDL_GetError() << "\n";
	}
	else
	{
		window = SDL_CreateWindow(title, 0, 0, width, height, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			std::cout << "Window failed to initialize. Error code: " << SDL_GetError();
		}
		else
		{
			SDL_SetWindowResizable(window, SDL_TRUE);
			canvas = SDL_GetWindowSurface(window);

			if (canvas == nullptr)
			{
				std::cout << "Window failed to return a window surface";
			}
			else
			{

			}
		}
	}
}

Engine::~Engine() {
	if (window) {
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}

void Engine::quit() { running = false; }

bool Engine::isRunning() { return running; }
void Engine::run()
{
	SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(window);
	handleIO();
}

void Engine::handleIO()
{
	while (SDL_PollEvent(&events))
	{
		if (events.type == SDL_QUIT) { quit(); }
	}
}

int Engine::mainLoop()
{
	while (running)
	{
		run();
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}



Engine& Engine::getInstance() { static Engine instance = Engine("test", 1920, 1080); return instance; }

int Engine::find_slot()
{
	for (int i = 0; i < MAX_PROPS; i++)
	{
		if (propIsValid(i))
		{
			return i;
		}
	}
	return -1;
}

void Engine::remove_prop(int idx)
{ props[idx] = nullptr; }


void Engine::add_prop(int idx, Prop* new_prop)
{
	props[idx] = new_prop;
}

vec2 Engine::getPropPosition(int index)
{
	return vec2(position_x[index], position_y[index]);
}

void Engine::setPropPosition(int index, vec2 new_position)
{
	position_x[index] = new_position.x;
	position_y[index] = new_position.y;
}

vec2 Engine::getPropVelocity(int index)
{
	return vec2(velocity_x[index], velocity_y[index]);
}

void Engine::setPropVelocity(int index, vec2 new_velocity)
{
	velocity_x[index] = new_velocity.x;
	velocity_y[index] = new_velocity.y;
}

vec2 Engine::getPropAcceleration(int index)
{
	return vec2(acceleration_x[index], acceleration_y[index]);
}

void Engine::setPropAcceleration(int index, vec2 new_acceleration)
{
	acceleration_x[index] = new_acceleration.x;
	acceleration_y[index] = new_acceleration.y;
}

float Engine::getPropRotation(int index)
{
	return rotation[index];
}

void Engine::setPropRotation(int index, float new_rotation)
{
	rotation[index] = new_rotation;
}

float Engine::getPropRotationalVelocity(int index)
{
	return rotational_velocity[index];
}

void Engine::setPropRotationalVelocity(int index, float new_rotational_velocity)
{
	rotational_velocity[index] = new_rotational_velocity;
}

float Engine::getPropRotationalAcceleration(int index)
{
	return rotational_acceleration[index];
}

void Engine::setPropRotationalAcceleration(int index, float new_rotational_acceleration)
{
	rotational_acceleration[index] = new_rotational_acceleration;
}

vec2 Engine::getWindowSize()
{
	return vec2((float)window_width, (float)window_height);
}

SDL_Surface& Engine::getCanvas()
{
	return *canvas;
}

Prop* Engine::getProp(int index)
{
	return props[index];
}
bool Engine::propIsValid(int index)
{
	return props[index] != nullptr;
}

void Engine::setWindowSize(vec2 new_window_size)
{
	window_width = new_window_size.x;
	window_height = new_window_size.y;
	//SDL_
}

#endif