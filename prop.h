#ifndef PROP_H
#define PROP_H
#include "vec2.h"
#ifndef ENGINE_H
#include "Engine.h"
#endif

#define PROP_ATTRIBUTE_COUNT 9

class Prop
{
public:


	static constexpr short PROP_DEBUG = 1;
	
	Prop(int prop_idx);
	Prop();
	~Prop();
	void setPropIndex(int idx);
	bool isValid();
	void setValid(bool state);

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
	//float getRotation();

	// Sets a prop's rotation (in radians)
	//void setRotation(float new_rotation);



	// Gets a prop's rotational velocity (radians / second)
	//float getRotationalVelocity();

	// Sets a prop's rotational velocity (radians / second)
	//void setRotationalVelocity(float new_rotational_velocity);



	// Gets a prop's rotational velocity (radians / second^2)
	//float getRotationalAcceleration();

	// Sets a prop's rotational velocity (radians / second^2)
	//void setRotationalAcceleration(float new_rotational_velocity);


private:
	int prop_ID = 0;
	short flags;
};

Prop::Prop(int prop_idx)
{
	flags = PROP_DEBUG;
	prop_ID = prop_idx;
}

Prop::Prop()
{
	flags = PROP_DEBUG;
}

Prop::~Prop() = default;
void Prop::setPropIndex(int idx)
{
	prop_ID = idx;
}


void Prop::draw()
{
	//SDL_RenderDrawRect()
}

vec2 Prop::getPosition() {
	return {};
}

//void Prop::setPosition(vec2 new_position) { (Engine::game)->setPropPosition(prop_ID, new_position); }// { ENGINE.setPropPosition(prop_ID, new_position); }

//vec2 Prop::getVelocity() {};// { return ENGINE.getPropVelocity(prop_ID); }
//void Prop::setVelocity(vec2 new_velocity) {};// { ENGINE.setPropVelocity(prop_ID, new_velocity); }

//vec2 Prop::getAcceleration() {};// { return ENGINE.getPropAcceleration(prop_ID); }
//void Prop::setAcceleration(vec2 new_acceleration) {};// { ENGINE.setPropAcceleration(prop_ID, new_acceleration); }

#endif // PROP_H