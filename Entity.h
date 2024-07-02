#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <cstdint>
#include "ent_handle.h"

class Entity
{
	// It is very important that you get references directly from the Entities themselves so that their reference counts are up-to-date
	ent_handle getReference();

};



#endif