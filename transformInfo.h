
#ifndef TRANSFORMINFO_H
#define TRANSFORMINFO_H
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class transformInfo
{
public:
	transformInfo();
	glm::mat4 transform;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec3 rotational_velocity;
	glm::vec3 rotational_acceleration;
	glm::vec3 getPosition();
	void setPosition(glm::vec3 new_position);
	glm::quat transformInfo::getRotation() const;
	void setRotation(const glm::quat& rotation);
};

transformInfo::transformInfo():
	// Identity matrix. Sets position and rotation to 0, but scale to 1.0 along all axes
	transform(1.0f),
	// No velocity
	velocity(0.0f),
	// No acceleration
	acceleration(0.0f),
	// No rotational velocity
	rotational_velocity(0.0f),
	// No rotational acceleration
	rotational_acceleration(0.0f)
{}

glm::vec3 transformInfo::getPosition() { return glm::vec3(transform[3]); }

void transformInfo::setPosition(glm::vec3 new_position) { transform[3] = glm::vec4(new_position, 1.0f); }

void transformInfo::setRotation(const glm::quat& rotation)
{
	// Convert the quaternion to a rotation matrix
	glm::mat3 rotationMatrix = glm::mat3_cast(rotation);
	// Copy the rotation matrix into the upper-left 3x3 part of the transformation matrix
	memcpy(&transform[0][0], &rotationMatrix[0][0], sizeof(float) * 3); // Copy first column (X-axis)
	memcpy(&transform[1][0], &rotationMatrix[1][0], sizeof(float) * 3); // Copy second column (Y-axis)
	memcpy(&transform[2][0], &rotationMatrix[2][0], sizeof(float) * 3); // Copy third column (Z-axis)
}
glm::quat transformInfo::getRotation() const {
	return glm::quat_cast(glm::mat3(transform));
}
#endif