#ifndef VEC2_H
#define VEC2_H

#pragma once
//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"

class vec2
{
public:
	float x, y;
	 vec2(float x = 0, float y = 0):  x(x), y(y) {}

	 vec2 operator+ (const vec2& other) const
	{
		return vec2(x + other.x, y + other.y);
	}

	vec2 operator * (float scalar) const
	{
		return vec2(x * scalar, y * scalar);
	}
	vec2& operator+=(const vec2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	vec2& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}
};


#endif