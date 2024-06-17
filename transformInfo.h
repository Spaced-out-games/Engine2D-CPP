
#ifndef TRANSFORMINFO_H
#define TRANSFORMINFO_H
#pragma once



#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

/// <summary>
/// There's one of these classes for every Prop in the scene (then some for allocated spaces)
/// Moving the pointer of a Prop via insertions and deletions will move these to the appropriate index so each is properly paired up
/// If you want to add more contiguous data to the class, that's your perogative. The main goal of this is to compartmentalize
/// </summary>



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

// Helper funtion for getting the position from the transform
glm::vec3 transformInfo::getPosition() { return glm::vec3(transform[3]); }

// Helper function for setting the position from the transform
void transformInfo::setPosition(glm::vec3 new_position) { transform[3] = glm::vec4(new_position, 1.0f); }

// Helper function for setting the rotation of the transform
void transformInfo::setRotation(const glm::quat& rotation)
{
	// Convert the quaternion to a rotation matrix
	glm::mat3 rotationMatrix = glm::mat3_cast(rotation);

	// Copy the rotation matrix into the upper-left 3x3 part of the transformation matrix. 
	// This is the most efficient means of doing it. I think it's safe enough
	memcpy(&transform[0][0], &rotationMatrix[0][0], sizeof(float) * 3); // Copy first column (X-axis)
	memcpy(&transform[1][0], &rotationMatrix[1][0], sizeof(float) * 3); // Copy second column (Y-axis)
	memcpy(&transform[2][0], &rotationMatrix[2][0], sizeof(float) * 3); // Copy third column (Z-axis)

}

// Helper function for getting the rotation from the transform
glm::quat transformInfo::getRotation() const {
	return glm::quat_cast(glm::mat3(transform));
}

#endif