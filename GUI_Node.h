#pragma once
#include "glm\glm.hpp"
#include "glm\vec2.hpp"
#include "basic_array.h"
#include "pair2D.h"
#include <math.h>



// Have GUI_Nodes query a slot within an hb_manager upon construction. This allows all the hitboxes to be in parallel, which is good for performance
// Once this is the case, 

class GUINode
{
public:
	GUINode(GUINode* parent);
	GUINode();
	~GUINode();



	virtual rect get_bbox() { return rect(0, 0, 0, 0); };
	rect get_bbox_contains_children();
	virtual void draw() {};
	bool isLeaf();
	bool isRoot();
	int numChildren() const;
	int removeChild(int index);
	int addChild(GUINode* child);

	basic_array<GUINode*> getChildren();
	int numSlots() const;
	static void setContext(SDL_GLContext& context);
	static SDL_GLContext getContext();
	std::string representation(int depth)
	{
		std::string content;
		for (size_t i = 0; i < depth; i++)
		{
			content += "\t";
		}
		if (parent) { content = "hasParent\n"; }
		content += "occupied slots: " + occupied_slots + '\n';

		if (isLeaf()) { return content; }
		for (size_t i = 0; i < children.len(); i++)
		{
			content += children.get(i)->representation(depth + 1);
		}
	}

private:

	GUINode* parent;
	basic_array<GUINode*> children;
	int occupied_slots;
	static SDL_GLContext s_context;
};


#include "engineCore.h"

SDL_GLContext GUINode::s_context = SDL_GL_GetCurrentContext();

GUINode::GUINode(): children(0), parent(nullptr), occupied_slots(0){}

GUINode::GUINode(GUINode* parent) : children(0), parent(parent), occupied_slots(0) {}

GUINode::~GUINode()
{
	for (size_t i = 0; i < children.len(); i++)
	{
		delete children.get(i);
	}
}
// We are a leaf node if our array of children is nullptr.
bool GUINode::isLeaf() { return children.c_array() == nullptr; }
// We are a root node if we don't have a parent
bool GUINode::isRoot() { return parent == nullptr; }
// get the number of children
int  GUINode::numChildren() const { return occupied_slots; }

int GUINode::numSlots() const { return children.len(); }


int GUINode::addChild(GUINode* child)
{
	if (occupied_slots < children.len())
	{
		children.set(occupied_slots,child);
		occupied_slots++;
		return occupied_slots - 1;
	}
	else
	{
		return -1;
	}
}

int GUINode::removeChild(int index)
{
	if (index >= 0 && index < occupied_slots)
	{
		children.set(index, children.get(occupied_slots-1));
		children.set(occupied_slots - 1, nullptr);
		occupied_slots--;
		return occupied_slots;
	}
	return -1;
}

rect GUINode::get_bbox_contains_children()
{
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float maxY = std::numeric_limits<float>::lowest();
	rect child_bbox (minX,minY,maxX,maxY);
	for (size_t i = 0; i < occupied_slots; i++)
	{
		child_bbox = children.get(i)->get_bbox();
		float child_minX = child_bbox.location.as_float().x;
		float child_minY = child_bbox.location.as_float().y;
		float child_maxX = child_minX + child_bbox.dimensions.as_float().x;
		float child_maxY = child_minY + child_bbox.dimensions.as_float().y;

		minX = std::min(minX, child_minX);
		minY = std::min(minY, child_minY);
		maxX = std::max(maxX, child_maxX);
		maxY = std::max(maxY, child_maxY);
	}

	float width = maxX - minX;
	float height = maxY - minY;

	return rect(minX, minY, width, height);
}

void GUINode::setContext(SDL_GLContext& context)
{
	s_context = &context;
}

SDL_GLContext GUINode::getContext()
{
	return s_context;
}

basic_array<GUINode*> GUINode::getChildren()
{
	return children;
}