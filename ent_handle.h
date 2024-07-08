#ifndef ENT_HANDLE_H
#define ENT_HANDLE_H
#include "bitset_512.h"


#define type_t bitset_size_t // for now until I have a better way to discern types
typedef struct ent_handle
{
	ent_handle(bitset_size_t entity_index) { type_ID = 0; block_ID = 0; ent_index = entity_index; }
	ent_handle(bitset_size_t type_index, bitset_size_t block_index, bitset_size_t entity_index) { type_ID = type_index; block_ID = block_index; ent_index = entity_index; }
	ent_handle() { type_ID = 0; block_ID = 0; ent_index = 0; }
	type_t type_ID;
	bitset_size_t block_ID;
	bitset_size_t ent_index;
} ent_handle;

#endif