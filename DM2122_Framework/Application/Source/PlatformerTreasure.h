#ifndef PLATFORMER_TREASURE
#define PLATFORMER_TREASURE
#include "Vector3.h"
#include "AABB.h"

class PlatformerTreasure
{
public:
	Vector3 pos;
	AABB treasureAABB;
	bool pickedUp = false;
};

#endif