#ifndef DRAWABLE_H
#define DRAWABLE_H

#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <vector>
#include "vec2.h"

class Drawable
{
public:
	Drawable();
	~Drawable();

private:
	std::vector<float> vertices;
	GLuint VBO;
	GLuint VAO;
	float rotation;
	vec2 position;




};

#endif // !DRAWABLE_H
