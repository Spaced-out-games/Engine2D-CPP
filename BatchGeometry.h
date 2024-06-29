#ifndef BATCH_GEOMETRY_H
#define BATCH_GEOMETRY_H

#include "Geometry.h"

class batchGeometry
{
public:
	batchGeometry(Geometry geometry);
	~batchGeometry();
	glm::mat4* addInstance(const glm::mat4& initial_transform);
	glm::mat4* addInstance();
	void removeInstance(glm::mat4* ptr_to_instance)

private:
	Geometry geometry;
	std::vector <glm::mat4> transforms;
	void expand();
};

batchGeometry::batchGeometry(Geometry geometry): geometry(geometry)
{
	transforms.capacity(32);
}

batchGeometry::~batchGeometry()
{
}

glm::mat4 batchGeometry::addInstance(const glm::mat4& initial_transform)
{

}

glm::mat4 batchGeometry::addInstance()
{

}
void batchGeometry::removeInstance(glm::mat4* ptr_to_instance)
{
	if (ptr_to_instance >= transforms.data() && ptr_to_instance < transforms.data() + transforms.size())
	{
		auto index = ptr_to_instance - transforms.data();

		transforms.erase(transforms.begin() + index);
	}
}

void batchGeometry::expand()
{
	transforms.capacity(transforms.size() * 2);
}

#endif