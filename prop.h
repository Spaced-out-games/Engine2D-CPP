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
#include "transformInfo.h"

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
	int partition_index = 0;
};
Prop::Prop() :

	// Automatically populate the prop_ID when it's created
	prop_ID(Engine::getInstance()->getNextSlot())

{}

Prop::Prop(int idx) :

	// Also set the prop_ID
	prop_ID(idx)

{};

Prop::~Prop() = default;

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