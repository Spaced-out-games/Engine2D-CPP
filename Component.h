#ifndef COMPONENT_H
#define COMPONENT_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glm/gtx/quaternion.hpp>
#include "Geometry.h"
#include "transformInfo.h"
/*
Basic game component. Attach to Props or extend this class to use. 
*/
class Component
{
public:
	
	Component(Geometry* initialGeometry);
	~Component();
	void setPosition(const glm::vec3& position);
	glm::vec3 getPosition() const;
	void setRotation(const glm::quat & rotation);
	glm::quat getRotation() const;
	void draw();
	void onTick();
private:
	Geometry* geometry;
	transformInfo transform_info;
};
Component::Component(Geometry* initialGeometry) :

	geometry(initialGeometry)
{};
Component::~Component() = default;
void Component::setPosition(const glm::vec3& position) {
	// Direct write without overhead of transformInfo.setPosition. Use inline for further optimization
	transform_info.transform[3] = glm::vec4(position, 1.0f);
}

glm::vec3 Component::getPosition() const
{
	// Direct read
	return glm::vec3(tranform_info.transform[3]);
}

void Component::setRotation(const glm::quat& rotation) {
	// Convert the quaternion to a rotation matrix
	glm::mat3 rotationMatrix = glm::mat3_cast(rotation);
	// Copy the rotation matrix into the upper-left 3x3 part of the transformation matrix
	memcpy(&transform_info.transform[0][0], &rotationMatrix[0][0], sizeof(float) * 3); // Copy first column (X-axis)
	memcpy(&transform_info.transform[1][0], &rotationMatrix[1][0], sizeof(float) * 3); // Copy second column (Y-axis)
	memcpy(&transform_info.transform[2][0], &rotationMatrix[2][0], sizeof(float) * 3); // Copy third column (Z-axis)
}

glm::quat Component::getRotation() const {
	return glm::quat_cast(glm::mat3(transform_info.transform));
}
void Component::draw()
{
	//std::cout << "drawn";
	if (geometry) { geometry->draw(); }
}


#endif