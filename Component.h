#ifndef COMPONENT_H
#define COMPONENT_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glm/gtx/quaternion.hpp>
#include "Geometry.h"
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
	glm::mat4 relative_transform;
	glm::vec3 relative_velocity;
	glm::vec3 relative_acceleration;

	glm::vec3 relative_rotational_velocity;
	glm::vec3 relative_rotational_acceleration;
};
Component::Component(Geometry* initialGeometry) :
	// Identity matrix. Sets position and rotation to 0, but scale to 1.0 along all axes
	relative_transform(1.0f),
	// No velocity
	relative_velocity(0.0f),
	// No acceleration
	relative_acceleration(0.0f),
	// No rotational velocity
	relative_rotational_velocity(0.0f),
	// No rotational acceleration
	relative_rotational_acceleration(0.0f),
	geometry(initialGeometry)
{};
Component::~Component() = default;
void Component::setPosition(const glm::vec3& position) {
	relative_transform[3] = glm::vec4(position, 1.0f);
}

glm::vec3 Component::getPosition() const
{
	return glm::vec3(relative_transform[3]);
}

void Component::setRotation(const glm::quat& rotation) {
	// Convert the quaternion to a rotation matrix
	glm::mat3 rotationMatrix = glm::mat3_cast(rotation);
	// Copy the rotation matrix into the upper-left 3x3 part of the transformation matrix
	memcpy(&relative_transform[0][0], &rotationMatrix[0][0], sizeof(float) * 3); // Copy first column (X-axis)
	memcpy(&relative_transform[1][0], &rotationMatrix[1][0], sizeof(float) * 3); // Copy second column (Y-axis)
	memcpy(&relative_transform[2][0], &rotationMatrix[2][0], sizeof(float) * 3); // Copy third column (Z-axis)
}

glm::quat Component::getRotation() const {
	return glm::quat_cast(glm::mat3(relative_transform));
}
void Component::draw()
{
	//std::cout << "drawn";
	if (geometry) { geometry->draw(); }
}
// Empty method overwritten by inherited classes
void Component::onTick()
{
}

#endif