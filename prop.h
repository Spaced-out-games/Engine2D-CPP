#ifndef PROP_H
#define PROP_H

#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glm/gtx/quaternion.hpp>
#include "Engine.h"
#include <vector>
#include "Geometry.h"
#include "Component.h"

// Throws 'name followed by "::" must be a class or namespace name' within kernel.cu when uncommented
//#include "kernel.cu"

#define PROP_ATTRIBUTE_COUNT 9



class Prop
{
public:
	Prop(int prop_idx);
	Prop();
	~Prop();
	void draw();
	void setPosition(const glm::vec3& position);
	glm::vec3 getPosition() const;
	void setRotation(const glm::quat& rotation);
	glm::quat getRotation() const;
	void onTick();

	void setID(int ID);
	int getID() const;

	bool addComponent(Component* added_component);
	void removeComponent(Component* removed_component);


	//int find_slot();
	//void cleanup(); // Optimizes memory layout with all occupied component slots on the right and empty ones on the left. This allows lots of optimizations to be made. This will be handled at a later date.


private:
	std::vector<Component*> components;
	int prop_ID;
	glm::mat4 transform;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec3 rotational_velocity;
	glm::vec3 rotational_acceleration;
	int partition_index = 0;
};
Prop::Prop():

	// Identity matrix. Sets position and rotation to 0, but scale to 1.0 along all axes
	transform(1.0f),
	// No velocity
	velocity(0.0f),
	// No acceleration
	acceleration(0.0f),
	// No rotational velocity
	rotational_velocity(0.0f),
	// No rotational acceleration
	rotational_acceleration(0.0f),

	prop_ID(0) // Replace with Engine::find_slot

{}

Prop::Prop(int idx) :

	// Identity matrix. Sets position and rotation to 0, but scale to 1.0 along all axes
	transform(1.0f),
	// No velocity
	velocity(0.0f),
	// No acceleration
	acceleration(0.0f),
	// No rotational velocity
	rotational_velocity(0.0f),
	// No rotational acceleration
	rotational_acceleration(0.0f),
	// Also set the prop_ID
	prop_ID(idx)
	// Set the geometry to the appropriate index

{};

Prop::~Prop() = default;


void Prop::setPosition(const glm::vec3& position) {
	transform[3] = glm::vec4(position, 1.0f);
}

glm::vec3 Prop::getPosition() const
{
	return glm::vec3(transform[3]);
}

void Prop::setRotation(const glm::quat& rotation) {
	// Convert the quaternion to a rotation matrix
	//glm::mat3 rotationMatrix = glm::toMat3(rotation);
	glm::mat3 rotationMatrix = glm::mat3_cast(rotation);
	// Copy the rotation matrix into the upper-left 3x3 part of the transformation matrix
	memcpy(&transform[0][0], &rotationMatrix[0][0], sizeof(float) * 3); // Copy first column (X-axis)
	memcpy(&transform[1][0], &rotationMatrix[1][0], sizeof(float) * 3); // Copy second column (Y-axis)
	memcpy(&transform[2][0], &rotationMatrix[2][0], sizeof(float) * 3); // Copy third column (Z-axis)
}

glm::quat Prop::getRotation() const {
	return glm::quat_cast(glm::mat3(transform));
}
void Prop::onTick()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->onTick();
	}
}
void Prop::draw()
{
	for (size_t i = 0; i < partition_index; i++)
	{
		if (components[i])
		{
			components[i]->draw();
		}
	}
}

void Prop::setID(int ID)
{
	prop_ID = ID;
}
int Prop::getID() const
{
	return prop_ID;
}
bool Prop::addComponent(Component* added_component)
{
	if (partition_index < components.size())
	{
		components[partition_index] = added_component;
		partition_index++;
	}
	else
	{
		components.push_back(added_component);
		partition_index++;
	}
	return 0;
}
void Prop::removeComponent(Component* removed_component)
{
	if (removed_component==nullptr) { return; } // Check for nullptr first
	// Get the prop's index. 
	int deleted_index = -1;
	for (size_t i = 0; i < partition_index; i++)
	{
		if (components[i] == removed_component)
		{
			deleted_index = i; break;
		}
	}
	if (deleted_index == -1) { return; } // If the requested component to remove is invalid, exit. Need to make this check in Engine, too

	// Delete the prop
	delete removed_component;

	// Since this prop is now invalid, move the last valid prop here
	components[deleted_index] = components[partition_index];

	// Since the last prop is pointed to twice in the array, set the original index to nullptr
	components[partition_index] = nullptr;

	// Since the original pointer is invalidated, advance the partition index backwards
	partition_index--;

}

#endif // PROP_H