#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "BatchGeometry.h"
class Scene
{
public:
	Scene();
	~Scene();
	void render()

private:
	std::vector<batchGeometry> modelCollections;
	std::vector<Geometry> models;
};

Scene::Scene()
{
}

Scene::~Scene()
{
}





#endif