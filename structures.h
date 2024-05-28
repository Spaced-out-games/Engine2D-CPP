#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

/*
	This defines whatever global structures the engine requires

*/


typedef struct Color
{
	short R;
	short G;
	short B;
};

template <typename T>
typedef struct CArray
{
	T* data;
	int length;
};

#endif