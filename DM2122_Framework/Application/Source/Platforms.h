#ifndef PLATFORMS_H
#define PLATFORMS_H
#include "Vector3.h"
#include "AABB.h"

class Platforms
{
public:
	Vector3 pos;
	int type;
	AABB platformAABB;
};

#endif