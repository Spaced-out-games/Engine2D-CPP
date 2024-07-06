#pragma once
#include <SDL.h>
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

	// Called when polling an event
	void inputEvent();

};




#include "engineCore.h"

Controller::Controller(): isPollingEvents(false) {}

Controller::~Controller() = default;


// Example game controller. This basic example provides bare-minimum functionality, exiting the game when pressing ESC
void Controller::inputEvent()
{
	if (events.type == SDL_QUIT) { engineCore::quit(); }
}
