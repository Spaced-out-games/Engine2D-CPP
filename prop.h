#ifndef PROP_H
#define PROP_H

#pragma once
#include "vec2.h"
#include "Engine.h"

// Throws 'name followed by "::" must be a class or namespace name' within kernel.cu when uncommented
//#include "kernel.cu"

#define PROP_ATTRIBUTE_COUNT 9


class Engine;

class Prop
{
public:
	
	
	static constexpr short PROP_DEBUG = 1;
	
	Prop(int prop_idx);
	Prop();
	~Prop();
	void setPropIndex(int idx);


	void draw();

	// Gets a prop's position in screen space
	vec2 getPosition();

	// Sets a prop's position in screen space
	void setPosition(vec2 new_position);



	// Gets a prop's velocity (in pixels / second)
	vec2 getVelocity();

	// Sets a prop's velocity (in pixels / second)
	void setVelocity(vec2 new_velocity);



	// Gets a prop's acceleration (in pixels / second^2)
	vec2 getAcceleration();

	// Sets a prop's acceleration (in pixels / second^2)
	void setAcceleration(vec2 new_velocity);



	// Gets a prop's rotation (in radians)
	float getRotation();

	// Sets a prop's rotation (in radians)
	void setRotation(float new_rotation);



	// Gets a prop's rotational velocity (radians / second)
	float getRotationalVelocity();

	// Sets a prop's rotational velocity (radians / second)
	void setRotationalVelocity(float new_rotational_velocity);



	// Gets a prop's rotational velocity (radians / second^2)
	float getRotationalAcceleration();

	// Sets a prop's rotational velocity (radians / second^2)
	void setRotationalAcceleration(float new_rotational_velocity);


private:
	int prop_ID = 0;
	short flags;

};

Prop::Prop(int prop_idx)
{
	flags = PROP_DEBUG;
	prop_ID = prop_idx;
}

Prop::Prop() { flags = PROP_DEBUG; }

Prop::~Prop() = default;
void Prop::setPropIndex(int idx) { prop_ID = idx; }


void Prop::draw()
{
	//SDL_RenderDrawRect()
}

vec2 Prop::getPosition() { return Engine::getInstance().getPropPosition(prop_ID); }

void Prop::setPosition(vec2 new_position) { return Engine::getInstance().setPropPosition(prop_ID,new_position); }


vec2 Prop::getVelocity() { return Engine::getInstance().getPropVelocity(prop_ID); }

void Prop::setVelocity(vec2 new_velocity) { Engine::getInstance().setPropVelocity(prop_ID, new_velocity); }




vec2 Prop::getAcceleration() { return Engine::getInstance().getPropAcceleration(prop_ID); }


void Prop::setAcceleration(vec2 new_velocity) { Engine::getInstance().setPropAcceleration(prop_ID, new_velocity); }


float Prop::getRotation() { return Engine::getInstance().getPropRotation(prop_ID); }


void Prop::setRotation(float new_rotation) { Engine::getInstance().setPropRotation(prop_ID, new_rotation); }


float Prop::getRotationalVelocity() { return Engine::getInstance().getPropRotationalVelocity(prop_ID); }


void Prop::setRotationalVelocity(float new_rotational_velocity) { Engine::getInstance().setPropRotationalVelocity(prop_ID, new_rotational_velocity); }

float Prop::getRotationalAcceleration() { return Engine::getInstance().getPropRotationalAcceleration(prop_ID); }


void Prop::setRotationalAcceleration(float new_rotational_velocity) { Engine::getInstance().setPropRotationalAcceleration(prop_ID, new_rotational_velocity); }

#endif // PROP_H